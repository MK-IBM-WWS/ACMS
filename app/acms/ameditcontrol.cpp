#include "ameditcontrol.h"
#include "ui_ameditcontrol.h"
#include "adminwindow.h"

AmEditControl::AmEditControl(AdminWindow *amWindow, QString login, QString password, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AmEditControl)
    , m_adminWindow(amWindow)
    , m_login(login)
    , m_password(password)
    , m_currentControllerId(-1)
    , m_minControllerId(-1)
    , m_maxControllerId(-1)
{
    ui->setupUi(this);
    ui->llogin->setText(m_login);
    setupStatusComboBox();

    m_completerModel = new QStringListModel(this);
    m_completer = new QCompleter(m_completerModel, this);
    m_completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->lesearch->setCompleter(m_completer);
    ui->lesearch->setPlaceholderText("Объект | Адрес");

    loadControllerIds();

    if (m_minControllerId != -1) {
        loadControllerData(m_minControllerId);
    }

    updateCompleterModel();
}

AmEditControl::~AmEditControl()
{
    delete ui;
}

void AmEditControl::setupStatusComboBox()
{
    ui->cbstatus->addItem("Online");
    ui->cbstatus->addItem("Offline");
    ui->cbstatus->addItem("Restarting");
}

void AmEditControl::loadControllerIds()
{
    QSqlDatabase db = m_adminWindow->getDatabase();

    if (!db.isOpen()) {
        QMessageBox::critical(this, "Ошибка", "Отсутствует подключение к базе данных!");
        return;
    }

    QSqlQuery query(db);
    query.prepare("SELECT MIN(controller_id), MAX(controller_id) FROM access_controller");

    if (!query.exec()) {
        QMessageBox::critical(this, "Ошибка",
                              "Не удалось получить ID контроллеров: " + query.lastError().text());
        return;
    }

    if (query.next()) {
        m_minControllerId = query.value(0).toInt();
        m_maxControllerId = query.value(1).toInt();
    }

    updateNavigationButtons();
}

void AmEditControl::updateCompleterModel()
{
    QSqlDatabase db = m_adminWindow->getDatabase();

    if (!db.isOpen()) {
        return;
    }

    QSqlQuery query(db);
    query.prepare("SELECT object_name, address, controller_id FROM access_controller ORDER BY object_name, address");

    if (!query.exec()) {
        return;
    }

    QStringList items;
    while (query.next()) {
        QString objectName = query.value(0).toString();
        QString address = query.value(1).isNull() ? "" : query.value(1).toString();
        int controllerId = query.value(2).toInt();

        QString displayText = QString("%1 | %2").arg(objectName, address);
        items.append(displayText);
    }

    m_completerModel->setStringList(items);
}

void AmEditControl::loadControllerData(int controllerId)
{
    QSqlDatabase db = m_adminWindow->getDatabase();

    if (!db.isOpen()) {
        QMessageBox::critical(this, "Ошибка", "Отсутствует подключение к базе данных!");
        return;
    }

    QSqlQuery query(db);
    query.prepare("SELECT object_name, address, work_shedule, status, model, network_address "
                  "FROM access_controller WHERE controller_id = :controller_id");
    query.bindValue(":controller_id", controllerId);

    if (!query.exec()) {
        QMessageBox::critical(this, "Ошибка",
                              "Не удалось загрузить данные контроллера: " + query.lastError().text());
        return;
    }

    if (query.next()) {
        m_currentControllerId = controllerId;

        ui->leobjectname->setText(query.value(0).toString());
        ui->leaddress->setText(query.value(1).isNull() ? "" : query.value(1).toString());
        ui->leshedule->setText(query.value(2).isNull() ? "" : query.value(2).toString());

        QString status = query.value(3).toString();
        int statusIndex = ui->cbstatus->findText(status);
        if (statusIndex != -1) {
            ui->cbstatus->setCurrentIndex(statusIndex);
        }

        ui->lemodel->setText(query.value(4).isNull() ? "" : query.value(4).toString());
        ui->leipv4->setText(query.value(5).toString());

        updateNavigationButtons();
    } else {
        QMessageBox::warning(this, "Предупреждение",
                             QString("Контроллер с ID %1 не найден!").arg(controllerId));
    }
}

void AmEditControl::updateNavigationButtons()
{
    ui->btnback->setEnabled(m_currentControllerId > m_minControllerId);
    ui->btnforward->setEnabled(m_currentControllerId < m_maxControllerId);
    ui->btnsave->setEnabled(m_currentControllerId != -1);
    ui->btndelete->setEnabled(m_currentControllerId != -1);
}

bool AmEditControl::validateIPv4(const QString &ip)
{
    QRegularExpression ipRegex("^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}"
                               "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");

    QRegularExpressionMatch match = ipRegex.match(ip);
    return match.hasMatch();
}

bool AmEditControl::checkUniqueConstraint(const QString &objectName, const QString &address, int excludeControllerId)
{
    QSqlDatabase db = m_adminWindow->getDatabase();

    QSqlQuery query(db);
    query.prepare("SELECT COUNT(*) FROM access_controller "
                  "WHERE object_name = :object_name AND address = :address "
                  "AND controller_id != :exclude_id");
    query.bindValue(":object_name", objectName);
    query.bindValue(":address", address);
    query.bindValue(":exclude_id", excludeControllerId);

    if (!query.exec()) {
        QMessageBox::critical(this, "Ошибка",
                              "Ошибка при проверке уникальности: " + query.lastError().text());
        return false;
    }

    query.next();
    return query.value(0).toInt() == 0;
}

void AmEditControl::on_btnback_clicked()
{
    if (m_currentControllerId > m_minControllerId) {
        QSqlDatabase db = m_adminWindow->getDatabase();
        QSqlQuery query(db);
        query.prepare("SELECT MAX(controller_id) FROM access_controller WHERE controller_id < :current_id");
        query.bindValue(":current_id", m_currentControllerId);

        if (query.exec() && query.next()) {
            int prevId = query.value(0).toInt();
            if (prevId > 0) {
                loadControllerData(prevId);
            }
        }
    }
}

void AmEditControl::on_btnforward_clicked()
{
    if (m_currentControllerId < m_maxControllerId) {
        QSqlDatabase db = m_adminWindow->getDatabase();
        QSqlQuery query(db);
        query.prepare("SELECT MIN(controller_id) FROM access_controller WHERE controller_id > :current_id");
        query.bindValue(":current_id", m_currentControllerId);

        if (query.exec() && query.next()) {
            int nextId = query.value(0).toInt();
            if (nextId > 0) {
                loadControllerData(nextId);
            }
        }
    }
}

void AmEditControl::on_btnupdate_clicked()
{
    if (m_currentControllerId != -1) {
        loadControllerData(m_currentControllerId);
        QMessageBox::information(this, "Обновлено", "Данные контроллера обновлены из базы данных!");
    }
}

void AmEditControl::on_btnsave_clicked()
{
    if (m_currentControllerId == -1) {
        QMessageBox::warning(this, "Предупреждение", "Нет открытой записи для сохранения!");
        return;
    }

    QString objectName = ui->leobjectname->text().trimmed();
    QString address = ui->leaddress->text().trimmed();
    QString workSchedule = ui->leshedule->text().trimmed();
    QString status = ui->cbstatus->currentText();
    QString model = ui->lemodel->text().trimmed();
    QString networkAddress = ui->leipv4->text().trimmed();

    if (objectName.isEmpty()) {
        QMessageBox::warning(this, "Предупреждение", "Поле 'Объект' не может быть пустым!");
        ui->leobjectname->setFocus();
        return;
    }

    if (networkAddress.isEmpty()) {
        QMessageBox::warning(this, "Предупреждение", "Поле 'Сетевой адрес' не может быть пустым!");
        ui->leipv4->setFocus();
        return;
    }

    if (!validateIPv4(networkAddress)) {
        QMessageBox::warning(this, "Предупреждение",
                             "Некорректный формат IPv4 адреса!\n"
                             "Ожидаемый формат: XXX.XXX.XXX.XXX\n"
                             "Каждый октет должен быть от 0 до 255");
        ui->leipv4->setFocus();
        ui->leipv4->selectAll();
        return;
    }

    if (!checkUniqueConstraint(objectName, address, m_currentControllerId)) {
        QMessageBox::warning(this, "Предупреждение",
                             "Контроллер с таким объектом и адресом уже существует!");
        return;
    }

    QSqlDatabase db = m_adminWindow->getDatabase();

    if (!db.transaction()) {
        QMessageBox::critical(this, "Ошибка", "Не удалось начать транзакцию!");
        return;
    }

    QSqlQuery query(db);
    query.prepare("UPDATE access_controller SET "
                  "object_name = :object_name, "
                  "address = :address, "
                  "work_shedule = :work_shedule, "
                  "status = :status, "
                  "model = :model, "
                  "network_address = :network_address "
                  "WHERE controller_id = :controller_id");

    query.bindValue(":object_name", objectName);
    query.bindValue(":address", address.isEmpty() ? QVariant() : address);
    query.bindValue(":work_shedule", workSchedule.isEmpty() ? QVariant() : workSchedule);
    query.bindValue(":status", status);
    query.bindValue(":model", model.isEmpty() ? QVariant() : model);
    query.bindValue(":network_address", networkAddress);
    query.bindValue(":controller_id", m_currentControllerId);

    if (!query.exec()) {
        db.rollback();
        QMessageBox::critical(this, "Ошибка",
                              "Не удалось сохранить изменения: " + query.lastError().text());
        return;
    }

    if (!db.commit()) {
        QMessageBox::critical(this, "Ошибка", "Не удалось подтвердить транзакцию!");
        return;
    }

    QMessageBox::information(this, "Успех", "Изменения успешно сохранены!");

    updateCompleterModel();
    loadControllerData(m_currentControllerId);
}

void AmEditControl::on_btndelete_clicked()
{
    if (m_currentControllerId == -1) {
        QMessageBox::warning(this, "Предупреждение", "Нет открытой записи для удаления!");
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Подтверждение удаления",
                                                              QString("Вы действительно хотите удалить контроллер?\n"
                                                                      "Объект: %1\n"
                                                                      "Адрес: %2")
                                                                  .arg(ui->leobjectname->text(),
                                                                       ui->leaddress->text()),
                                                              QMessageBox::Yes | QMessageBox::No);

    if (reply != QMessageBox::Yes) {
        return;
    }

    QSqlDatabase db = m_adminWindow->getDatabase();

    if (!db.transaction()) {
        QMessageBox::critical(this, "Ошибка", "Не удалось начать транзакцию!");
        return;
    }

    QSqlQuery query(db);
    query.prepare("DELETE FROM access_controller WHERE controller_id = :controller_id");
    query.bindValue(":controller_id", m_currentControllerId);

    if (!query.exec()) {
        db.rollback();
        QMessageBox::critical(this, "Ошибка",
                              "Не удалось удалить контроллер: " + query.lastError().text());
        return;
    }

    if (!db.commit()) {
        QMessageBox::critical(this, "Ошибка", "Не удалось подтвердить транзакцию!");
        return;
    }

    QMessageBox::information(this, "Успех", "Контроллер успешно удален!");

    updateCompleterModel();
    loadControllerIds();

    if (m_minControllerId != -1) {
        if (m_currentControllerId >= m_minControllerId) {
            QSqlQuery nextQuery(db);
            nextQuery.prepare("SELECT MIN(controller_id) FROM access_controller WHERE controller_id > :current_id");
            nextQuery.bindValue(":current_id", m_currentControllerId);

            if (nextQuery.exec() && nextQuery.next()) {
                int nextId = nextQuery.value(0).toInt();
                if (nextId > 0) {
                    loadControllerData(nextId);
                } else {
                    QSqlQuery lastQuery(db);
                    lastQuery.prepare("SELECT MAX(controller_id) FROM access_controller");
                    if (lastQuery.exec() && lastQuery.next()) {
                        int lastId = lastQuery.value(0).toInt();
                        if (lastId > 0) {
                            loadControllerData(lastId);
                        } else {
                            ui->leobjectname->clear();
                            ui->leaddress->clear();
                            ui->leshedule->clear();
                            ui->lemodel->clear();
                            ui->leipv4->clear();
                            m_currentControllerId = -1;
                            updateNavigationButtons();
                        }
                    }
                }
            }
        } else {
            loadControllerData(m_minControllerId);
        }
    }
}

void AmEditControl::on_btnsearch_clicked()
{
    QString searchText = ui->lesearch->text().trimmed();

    if (searchText.isEmpty()) {
        QMessageBox::warning(this, "Предупреждение", "Введите критерии поиска в формате: Объект | Адрес");
        return;
    }

    QStringList parts = searchText.split('|');
    if (parts.size() != 2) {
        QMessageBox::warning(this, "Предупреждение",
                             "Неверный формат поиска!\n"
                             "Используйте формат: Объект | Адрес");
        return;
    }

    QString objectName = parts[0].trimmed();
    QString address = parts[1].trimmed();

    QSqlDatabase db = m_adminWindow->getDatabase();

    QSqlQuery query(db);
    query.prepare("SELECT controller_id FROM access_controller "
                  "WHERE object_name = :object_name AND address = :address");
    query.bindValue(":object_name", objectName);
    query.bindValue(":address", address);

    if (!query.exec()) {
        QMessageBox::critical(this, "Ошибка",
                              "Ошибка при поиске контроллера: " + query.lastError().text());
        return;
    }

    if (query.next()) {
        int controllerId = query.value(0).toInt();
        loadControllerData(controllerId);
        ui->lesearch->clear();
    } else {
        QMessageBox::information(this, "Не найдено",
                                 QString("Контроллер с объектом '%1' и адресом '%2' не найден!")
                                     .arg(objectName, address));
    }
}

void AmEditControl::on_btnlogout_clicked()
{
    if (!m_adminWindow->isVisible()) {
        AdminWindow *newAdminWindow = new AdminWindow(m_login, m_password);
        newAdminWindow->show();
    }

    this->close();
}
