#include "controllerwindow.h"
#include "ui_controllerwindow.h"
#include "loginwindow.h"
#include "dbconfig.h"

ControllerWindow::ControllerWindow(QString &login, QString &password, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ControllerWindow)
    , m_login(login), m_password(password)
    , m_controllerCompleter(nullptr)
    , m_controllerModel(nullptr)
{
    ui->setupUi(this);
    ui->llogin->setText(login);

    // Подключаемся к базе данных
    if (!connectToDatabase()) {
        QMessageBox::critical(this, "Ошибка", "Не удалось подключиться к базе данных!");
        return;
    }

    // Настраиваем комбобоксы
    setupComboBoxes();

    // Настраиваем автодополнение для контроллера
    setupControllerAutoComplete();

    // Настраиваем таблицу для отображения групп
    ui->twgroups->setColumnCount(4);
    ui->twgroups->setHorizontalHeaderLabels(QStringList() << "Название группы" << "Дата назначения" << "Дата окончания" << "ID группы");
    ui->twgroups->horizontalHeader()->setStretchLastSection(true);
    ui->twgroups->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->twgroups->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->twgroups->setColumnHidden(3, true);

    // Настраиваем подсказки для полей ввода
    ui->lepass->setPlaceholderText("Введите номер пропуска");
    ui->lecontrol->setPlaceholderText("Объект | Адрес");
    ui->lecheckpass->setPlaceholderText("Введите номер пропуска");
}

ControllerWindow::~ControllerWindow()
{
    if (db.isOpen()) {
        db.close();
    }
    delete ui;
}

bool ControllerWindow::connectToDatabase()
{
    DbConfig config;
    config.loadConfig();

    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(config.host);
    db.setPort(config.port);
    db.setDatabaseName("acms");
    db.setUserName(DB_USERNAME);
    db.setPassword(m_password);

    if (!db.open()) {
        qDebug() << "Database error:" << db.lastError().text();
        return false;
    }

    return true;
}

void ControllerWindow::setupComboBoxes()
{
    // Настройка комбобокса типа доступа
    ui->cbaccesstype->addItem("Вход", "IN");
    ui->cbaccesstype->addItem("Выход", "OUT");

    // Настройка комбобокса решения
    ui->cbsolution->addItem("Ожидает", "Pending");
    ui->cbsolution->addItem("Принят", "Accepted");
    ui->cbsolution->addItem("Отклонен", "Rejected");

    // Устанавливаем текущее время в поле letime
    ui->letime->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
}

void ControllerWindow::setupControllerAutoComplete()
{
    // Загружаем все контроллеры из БД
    QSqlQuery query("SELECT object_name, address FROM access_controller ORDER BY object_name");

    QStringList controllerList;
    while (query.next()) {
        QString objectName = query.value(0).toString();
        QString address = query.value(1).toString();
        // Форматируем для отображения: "Объект | Адрес"
        controllerList << QString("%1 | %2").arg(objectName).arg(address);
    }

    m_controllerModel = new QStringListModel(controllerList, this);
    m_controllerCompleter = new QCompleter(m_controllerModel, this);
    m_controllerCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    m_controllerCompleter->setFilterMode(Qt::MatchContains);
    ui->lecontrol->setCompleter(m_controllerCompleter);
}

bool ControllerWindow::getControllerIdByNameAndAddress(const QString &objectName, const QString &address, int &controllerId)
{
    QSqlQuery query;
    query.prepare("SELECT controller_id FROM access_controller "
                  "WHERE object_name = :object_name AND address = :address");
    query.bindValue(":object_name", objectName);
    query.bindValue(":address", address);

    if (query.exec() && query.next()) {
        controllerId = query.value(0).toInt();
        return true;
    }

    return false;
}

bool ControllerWindow::insertAccessFact(int passId, const QString &objectName, const QString &address,
                                        const QString &accessType, const QString &solution, const QDateTime &accessTime)
{
    QDateTime utcTime = accessTime;
    utcTime.setTimeSpec(Qt::UTC);
    int controllerId;
    if (!getControllerIdByNameAndAddress(objectName, address, controllerId)) {
        qDebug() << "Controller not found:" << objectName << address;
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO access_facts (solution, access_time, access_type, pass_id, controller_id) "
                  "VALUES (:solution, :access_time, :access_type, :pass_id, :controller_id)");
    query.bindValue(":solution", solution);
    query.bindValue(":access_time", utcTime);
    query.bindValue(":access_type", accessType);
    query.bindValue(":pass_id", passId);
    query.bindValue(":controller_id", controllerId);

    if (!query.exec()) {
        qDebug() << "Insert error:" << query.lastError().text();
        return false;
    }

    return true;
}

bool ControllerWindow::checkPassExists(int passId)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM pass WHERE pass_id = :pass_id");
    query.bindValue(":pass_id", passId);

    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }

    return false;
}

QString ControllerWindow::getStaffNameByPassId(int passId)
{
    QSqlQuery query;
    query.prepare("SELECT s.fio FROM staff s "
                  "JOIN pass p ON s.staff_id = p.staff_id "
                  "WHERE p.pass_id = :pass_id");
    query.bindValue(":pass_id", passId);

    if (query.exec() && query.next()) {
        return query.value(0).toString();
    }

    return QString();
}

void ControllerWindow::loadPassGroups(int passId)
{
    QSqlQuery query;
    query.prepare("SELECT ag.group_id, ag.group_name, p.issue_date, p.expiration_date "
                  "FROM purpose p "
                  "JOIN access_groups ag ON p.group_id = ag.group_id "
                  "WHERE p.pass_id = :pass_id "
                  "ORDER BY p.issue_date DESC");
    query.bindValue(":pass_id", passId);

    if (!query.exec()) {
        QMessageBox::warning(this, "Ошибка", "Не удалось загрузить группы доступа: " + query.lastError().text());
        return;
    }

    ui->twgroups->clearContents();
    ui->twgroups->setRowCount(0);

    int row = 0;
    while (query.next()) {
        ui->twgroups->insertRow(row);

        int groupId = query.value(0).toInt();
        QString groupName = query.value(1).toString();
        QString issueDate = query.value(2).toDate().toString("dd.MM.yyyy");
        QString expirationDate = query.value(3).toDate().toString("dd.MM.yyyy");

        ui->twgroups->setItem(row, 0, new QTableWidgetItem(groupName));
        ui->twgroups->setItem(row, 1, new QTableWidgetItem(issueDate));
        ui->twgroups->setItem(row, 2, new QTableWidgetItem(expirationDate));
        ui->twgroups->setItem(row, 3, new QTableWidgetItem(QString::number(groupId)));

        row++;
    }

    if (row == 0) {
        ui->twgroups->setRowCount(1);
        ui->twgroups->setItem(0, 0, new QTableWidgetItem("Нет групп доступа"));
        ui->twgroups->setItem(0, 1, new QTableWidgetItem(""));
        ui->twgroups->setItem(0, 2, new QTableWidgetItem(""));
        ui->twgroups->setItem(0, 3, new QTableWidgetItem(""));
    }

    ui->twgroups->resizeColumnsToContents();
}

void ControllerWindow::clearForm()
{
    ui->lepass->clear();
    ui->lecontrol->clear();
    ui->letime->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    ui->cbaccesstype->setCurrentIndex(0);
    ui->cbsolution->setCurrentIndex(0);
    ui->lepass->setToolTip("");
    ui->lecontrol->setToolTip("");
}

void ControllerWindow::on_lenow_clicked()
{
    ui->letime->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
}

void ControllerWindow::on_lepass_textChanged(const QString &text)
{
    if (!text.isEmpty()) {
        bool ok;
        int passId = text.toInt(&ok);
        if (ok) {
            QString staffName = getStaffNameByPassId(passId);
            if (!staffName.isEmpty()) {
                ui->lepass->setToolTip(QString("Сотрудник: %1").arg(staffName));
            } else {
                ui->lepass->setToolTip("Пропуск не найден");
            }
        } else {
            ui->lepass->setToolTip("Введите числовой ID пропуска");
        }
    } else {
        ui->lepass->setToolTip("");
    }
}

void ControllerWindow::on_lecontrol_textChanged(const QString &text)
{
    // Показываем подсказку при вводе
    if (!text.isEmpty()) {
        // Разделяем по символу |
        QStringList parts = text.split("|");

        if (parts.size() >= 2) {
            QString objectName = parts[0].trimmed();
            QString address = parts.mid(1).join("|").trimmed();

            // Проверяем существует ли такой контроллер
            int controllerId;
            if (getControllerIdByNameAndAddress(objectName, address, controllerId)) {
                ui->lecontrol->setToolTip(QString("✓ Контроллер найден (ID: %1)").arg(controllerId));
                ui->lecontrol->setStyleSheet("QLineEdit { background-color: #d4ffd4; }");
            } else {
                ui->lecontrol->setToolTip("✗ Контроллер не найден");
                ui->lecontrol->setStyleSheet("QLineEdit { background-color: #ffd4d4; }");
            }
        } else {
            ui->lecontrol->setToolTip("Введите в формате: 'Объект | Адрес'");
            ui->lecontrol->setStyleSheet("");
        }
    } else {
        ui->lecontrol->setToolTip("");
        ui->lecontrol->setStyleSheet("");
    }
}

void ControllerWindow::on_lecontrol_editingFinished()
{
    // Дополнительная проверка при завершении редактирования
    QString text = ui->lecontrol->text();
    if (!text.isEmpty()) {
        QStringList parts = text.split("|");

        if (parts.size() >= 2) {
            QString objectName = parts[0].trimmed();
            QString address = parts.mid(1).join("|").trimmed();

            if (!objectName.isEmpty() && !address.isEmpty()) {
                int controllerId;
                if (!getControllerIdByNameAndAddress(objectName, address, controllerId)) {
                    QMessageBox::warning(this, "Предупреждение",
                                         QString("Контроллер '%1' по адресу '%2' не найден в базе данных!\n"
                                                 "Пожалуйста, выберите существующий контроллер из списка.")
                                             .arg(objectName).arg(address));
                }
            }
        }
    }
}

void ControllerWindow::on_btnregister_clicked()
{
    // Получаем данные из формы
    QString passText = ui->lepass->text();
    QString controlText = ui->lecontrol->text();
    QString accessTimeStr = ui->letime->text();
    QString accessType = ui->cbaccesstype->currentData().toString();
    QString solution = ui->cbsolution->currentData().toString();

    // Проверяем заполненность полей
    if (passText.isEmpty() || controlText.isEmpty() || accessTimeStr.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, заполните все поля!");
        return;
    }

    // Проверяем корректность ID пропуска
    bool passOk;
    int passId = passText.toInt(&passOk);
    if (!passOk) {
        QMessageBox::warning(this, "Ошибка", "Номер пропуска должен быть числом!");
        return;
    }

    // Проверяем существование пропуска
    if (!checkPassExists(passId)) {
        QMessageBox::warning(this, "Ошибка", "Пропуск с таким номером не существует!");
        return;
    }

    // Разбираем строку контроллера на объект и адрес (разделитель |)
    QStringList parts = controlText.split("|");
    if (parts.size() < 2) {
        QMessageBox::warning(this, "Ошибка",
                             "Неверный формат контроллера!\n"
                             "Используйте формат: 'Объект | Адрес'");
        return;
    }

    QString objectName = parts[0].trimmed();
    QString address = parts.mid(1).join("|").trimmed();

    if (objectName.isEmpty() || address.isEmpty()) {
        QMessageBox::warning(this, "Ошибка",
                             "Название объекта и адрес не могут быть пустыми!\n"
                             "Используйте формат: 'Объект | Адрес'");
        return;
    }

    // Проверяем существование контроллера
    int controllerId;
    if (!getControllerIdByNameAndAddress(objectName, address, controllerId)) {
        QMessageBox::warning(this, "Ошибка",
                             QString("Контроллер '%1' по адресу '%2' не найден в базе данных!")
                                 .arg(objectName).arg(address));
        return;
    }

    // Преобразуем строку времени в QDateTime
    QDateTime accessTime = QDateTime::fromString(accessTimeStr, "yyyy-MM-dd hh:mm:ss");
    if (!accessTime.isValid()) {
        accessTime = QDateTime::currentDateTime();
    }

    // Добавляем запись в базу данных
    if (insertAccessFact(passId, objectName, address, accessType, solution, accessTime)) {
        QMessageBox::information(this, "Успех", "Запись о доступе успешно добавлена!");
        clearForm();
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось добавить запись о доступе!");
    }
}

void ControllerWindow::on_lecheckgroup_clicked()
{
    QString passText = ui->lecheckpass->text();

    if (passText.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите номер пропуска!");
        return;
    }

    bool ok;
    int passId = passText.toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Ошибка", "Номер пропуска должен быть числом!");
        return;
    }

    if (!checkPassExists(passId)) {
        QMessageBox::information(this, "Результат", "Пропуск с таким номером не существует!");
        ui->twgroups->clearContents();
        ui->twgroups->setRowCount(1);
        ui->twgroups->setItem(0, 0, new QTableWidgetItem("Пропуск не найден"));
        ui->twgroups->setItem(0, 1, new QTableWidgetItem(""));
        ui->twgroups->setItem(0, 2, new QTableWidgetItem(""));
        ui->twgroups->setItem(0, 3, new QTableWidgetItem(""));
        return;
    }

    loadPassGroups(passId);
}

void ControllerWindow::on_btngroupconsist_clicked()
{
    // Получаем выбранную строку в таблице
    int currentRow = ui->twgroups->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, выберите группу доступа в таблице!");
        return;
    }

    // Проверяем, что это не сообщение об отсутствии групп
    QTableWidgetItem *item = ui->twgroups->item(currentRow, 0);
    if (!item || item->text() == "Нет групп доступа" || item->text() == "Пропуск не найден") {
        QMessageBox::warning(this, "Ошибка", "Невозможно получить информацию о выбранной группе!");
        return;
    }

    // Получаем ID группы из скрытого столбца
    QTableWidgetItem *idItem = ui->twgroups->item(currentRow, 3);
    if (!idItem) {
        QMessageBox::warning(this, "Ошибка", "Не удалось получить ID группы!");
        return;
    }

    int groupId = idItem->text().toInt();
    QString groupName = ui->twgroups->item(currentRow, 0)->text();

    // Открываем окно с составом группы
    CoGroupConsists *groupWindow = new CoGroupConsists(groupId, nullptr);
    groupWindow->setAttribute(Qt::WA_DeleteOnClose);
    groupWindow->show();
}

void ControllerWindow::on_btnlogout_clicked()
{
    LoginWindow *loginWindow = new LoginWindow(nullptr);
    loginWindow->setAttribute(Qt::WA_DeleteOnClose);
    loginWindow->show();

    this->close();
}
