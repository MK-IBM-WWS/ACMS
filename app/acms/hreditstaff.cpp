#include "hreditstaff.h"
#include "ui_hreditstaff.h"
#include "hrwindow.h"
#include "dbconfig.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>

HrEditStaff::HrEditStaff(HrWindow *hrWindow, QString &m_login, QString &m_password, int staff_id, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HrEditStaff)
    , login(m_login)
    , password(m_password)
    , currentStaffId(-1)
    , mainHrWindow(hrWindow)
    , currentIndex(-1)
    , depCompleter(nullptr)
    , depModel(nullptr)
    , searchCompleter(nullptr)
    , searchModel(nullptr)
{
    ui->setupUi(this);

    // Отображаем логин
    ui->llogin->setText(login);

    // Настраиваем ComboBox для статуса работы
    ui->cbiswork->addItem("Работает", true);
    ui->cbiswork->addItem("Уволен", false);

    if (mainHrWindow) {
        db = mainHrWindow->getDatabase();
    }
    if (!ensureDatabaseConnection()) {
        QMessageBox::warning(this, "Ошибка", "Нет подключения к базе данных");
    }

    // Загружаем список отделов для автодополнения
    loadDepartments();

    // Загружаем список сотрудников для навигации
    loadStaffList();

    // Загружаем подсказки для поиска
    loadSearchSuggestions();

    // Подключаем сигналы
    connect(ui->ledepo, &QLineEdit::textChanged, this, &HrEditStaff::on_ledepo_textChanged);
    connect(ui->lesearch, &QLineEdit::textChanged, this, &HrEditStaff::on_lesearch_textChanged);

    // Загружаем данные
    if (staff_id != -1) {
        // Если передан ID, загружаем эту запись
        loadStaffData(staff_id);
        // Находим индекс в списке
        currentIndex = staffIds.indexOf(staff_id);
    } else {
        // Иначе загружаем первую запись
        if (!staffIds.isEmpty()) {
            currentIndex = 0;
            loadStaffData(staffIds[currentIndex]);
        }
    }
}

HrEditStaff::~HrEditStaff()
{
    delete ui;
}

bool HrEditStaff::ensureDatabaseConnection()
{
    if (!db.isOpen()) {
        QMessageBox::StandardButton reply = QMessageBox::question(this,
                                                                  "Нет подключения",
                                                                  "Отсутствует подключение к базе данных.\n"
                                                                  "Попробовать переподключиться?",
                                                                  QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes && mainHrWindow) {
            return mainHrWindow->reconnectToDatabase();
        }
        return false;
    }
    return true;
}

void HrEditStaff::loadDepartments()
{
    QSqlQuery query(db);
    QStringList departments;

    QString sql = "SELECT department_name, address FROM departments ORDER BY department_name";

    if (query.exec(sql)) {
        while (query.next()) {
            QString depName = query.value(0).toString();
            QString address = query.value(1).toString();
            departments << depName + " | " + address;
        }

        depModel = new QStringListModel(departments, this);
        depCompleter = new QCompleter(depModel, this);
        depCompleter->setCaseSensitivity(Qt::CaseInsensitive);
        depCompleter->setFilterMode(Qt::MatchContains);
        ui->ledepo->setCompleter(depCompleter);
    } else {
        qDebug() << "Error loading departments:" << query.lastError().text();
    }
}

void HrEditStaff::loadStaffList()
{
    QSqlQuery query(db);
    QString sql = "SELECT staff_id FROM staff ORDER BY staff_id";

    staffIds.clear();

    if (query.exec(sql)) {
        while (query.next()) {
            staffIds.append(query.value(0).toInt());
        }
    } else {
        qDebug() << "Error loading staff list:" << query.lastError().text();
    }
}

void HrEditStaff::loadSearchSuggestions()
{
    QSqlQuery query(db);
    QStringList suggestions;

    QString sql = "SELECT fio, phone FROM staff ORDER BY fio";

    if (query.exec(sql)) {
        while (query.next()) {
            QString fio = query.value(0).toString();
            QString phone = query.value(1).toString();
            suggestions << fio + " | " + phone;
        }

        searchModel = new QStringListModel(suggestions, this);
        searchCompleter = new QCompleter(searchModel, this);
        searchCompleter->setCaseSensitivity(Qt::CaseInsensitive);
        searchCompleter->setFilterMode(Qt::MatchContains);
        ui->lesearch->setCompleter(searchCompleter);
    } else {
        qDebug() << "Error loading search suggestions:" << query.lastError().text();
    }
}

void HrEditStaff::loadStaffData(int staffId)
{
    QSqlQuery query(db);
    query.prepare("SELECT s.fio, s.phone, s.position_name, s.is_work, "
                  "d.department_name, d.address, s.department_id "
                  "FROM staff s "
                  "JOIN departments d ON s.department_id = d.department_id "
                  "WHERE s.staff_id = :id");
    query.bindValue(":id", staffId);

    if (query.exec() && query.next()) {
        currentStaffId = staffId;

        ui->lefio->setText(query.value(0).toString());
        ui->lephone->setText(query.value(1).toString());
        ui->leposition->setText(query.value(2).toString());

        bool isWork = query.value(3).toBool();
        ui->cbiswork->setCurrentIndex(isWork ? 0 : 1);

        QString depString = query.value(4).toString() + " | " + query.value(5).toString();
        ui->ledepo->setText(depString);

        // Обновляем состояние кнопок навигации
        ui->btnback->setEnabled(currentIndex > 0);
        ui->btforward->setEnabled(currentIndex < staffIds.size() - 1);

        // Сбрасываем стиль поля отдела
        ui->ledepo->setStyleSheet("");
    } else {
        QMessageBox::warning(this, "Ошибка", "Не удалось загрузить данные сотрудника");
    }
}

int HrEditStaff::getDepartmentId(const QString &depoText)
{
    if (depoText.isEmpty()) {
        return -1;
    }

    QStringList parts = depoText.split(" | ");
    if (parts.size() != 2) {
        return -1;
    }

    QString depName = parts[0].trimmed();
    QString address = parts[1].trimmed();

    QSqlQuery query(db);
    query.prepare("SELECT department_id FROM departments WHERE department_name = :name AND address = :address");
    query.bindValue(":name", depName);
    query.bindValue(":address", address);

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }

    return -1;
}

QString HrEditStaff::getDepartmentString(int departmentId)
{
    QSqlQuery query(db);
    query.prepare("SELECT department_name, address FROM departments WHERE department_id = :id");
    query.bindValue(":id", departmentId);

    if (query.exec() && query.next()) {
        return query.value(0).toString() + " | " + query.value(1).toString();
    }

    return QString();
}

void HrEditStaff::navigateToStaff(int direction)
{
    int newIndex = currentIndex + direction;

    if (newIndex >= 0 && newIndex < staffIds.size()) {
        currentIndex = newIndex;
        loadStaffData(staffIds[currentIndex]);
    }
}

void HrEditStaff::on_ledepo_textChanged(const QString &text)
{
    if (!text.isEmpty() && !text.contains(" | ")) {
        ui->ledepo->setStyleSheet("QLineEdit { background-color: #FFFACD; }");
    } else {
        ui->ledepo->setStyleSheet("");
    }
}

void HrEditStaff::on_lesearch_textChanged(const QString &text)
{
    // Просто для визуального контроля
    if (!text.isEmpty() && !text.contains(" | ")) {
        ui->lesearch->setStyleSheet("QLineEdit { background-color: #FFFACD; }");
    } else {
        ui->lesearch->setStyleSheet("");
    }
}

void HrEditStaff::on_btnback_clicked()
{
    navigateToStaff(-1);
}

void HrEditStaff::on_btforward_clicked()
{
    navigateToStaff(1);
}

void HrEditStaff::on_btnupdate_clicked()
{
    if (currentStaffId != -1) {
        loadStaffData(currentStaffId);
        QMessageBox::information(this, "Обновление", "Данные обновлены");
    }
}

void HrEditStaff::on_btnsave_clicked()
{
    if (currentStaffId == -1) {
        QMessageBox::warning(this, "Предупреждение", "Нет загруженных данных для сохранения");
        return;
    }

    // Проверяем заполнение полей
    if (ui->lefio->text().isEmpty()) {
        QMessageBox::warning(this, "Предупреждение", "Введите ФИО сотрудника");
        ui->lefio->setFocus();
        return;
    }

    if (ui->lephone->text().isEmpty()) {
        QMessageBox::warning(this, "Предупреждение", "Введите номер телефона");
        ui->lephone->setFocus();
        return;
    }

    if (ui->ledepo->text().isEmpty()) {
        QMessageBox::warning(this, "Предупреждение", "Выберите отдел");
        ui->ledepo->setFocus();
        return;
    }

    // Получаем ID отдела
    int departmentId = getDepartmentId(ui->ledepo->text());
    if (departmentId == -1) {
        QMessageBox::warning(this, "Предупреждение",
                             "Отдел не найден в базе данных.\n"
                             "Используйте формат: Название отдела | Адрес\n"
                             "Выберите отдел из выпадающего списка");
        return;
    }

    // Подготавливаем запрос на обновление
    QSqlQuery query(db);
    query.prepare("UPDATE staff SET fio = :fio, phone = :phone, "
                  "position_name = :position, is_work = :is_work, "
                  "department_id = :dep_id "
                  "WHERE staff_id = :id");

    query.bindValue(":fio", ui->lefio->text().trimmed());
    query.bindValue(":phone", ui->lephone->text().trimmed());
    query.bindValue(":position", ui->leposition->text().trimmed());
    query.bindValue(":is_work", ui->cbiswork->currentData().toBool());
    query.bindValue(":dep_id", departmentId);
    query.bindValue(":id", currentStaffId);

    if (!query.exec()) {
        QString errorMsg = query.lastError().text();

        if (errorMsg.contains("unique") && errorMsg.contains("phone")) {
            QMessageBox::critical(this, "Ошибка",
                                  "Сотрудник с таким номером телефона уже существует");
        } else {
            QMessageBox::critical(this, "Ошибка",
                                  "Не удалось обновить данные:\n" + errorMsg);
        }
        return;
    }

    QMessageBox::information(this, "Успех", "Данные сотрудника обновлены");

    // Обновляем список для поиска
    loadSearchSuggestions();

    // Перезагружаем данные
    loadStaffData(currentStaffId);
}

void HrEditStaff::on_btndelete_clicked()
{
    if (currentStaffId == -1) {
        QMessageBox::warning(this, "Предупреждение", "Нет загруженных данных для удаления");
        return;
    }

    // Запрашиваем подтверждение с ФИО сотрудника
    QString fio = ui->lefio->text();
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Подтверждение удаления",
                                                              QString("Вы уверены, что хотите удалить сотрудника '%1'?").arg(fio),
                                                              QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QSqlQuery query(db);
        query.prepare("DELETE FROM staff WHERE staff_id = :id");
        query.bindValue(":id", currentStaffId);

        if (query.exec()) {
            QMessageBox::information(this, "Успех", "Сотрудник удален");

            // Обновляем список сотрудников
            loadStaffList();
            loadSearchSuggestions();

            // Переходим к следующей или предыдущей записи
            if (!staffIds.isEmpty()) {
                if (currentIndex < staffIds.size()) {
                    // Загружаем запись на той же позиции (следующая сдвинулась)
                    loadStaffData(staffIds[currentIndex]);
                } else if (currentIndex > 0) {
                    // Если удалили последнюю запись, переходим к предыдущей
                    currentIndex--;
                    loadStaffData(staffIds[currentIndex]);
                }
            } else {
                // Список пуст, очищаем поля
                currentStaffId = -1;
                currentIndex = -1;
                ui->lefio->clear();
                ui->lephone->clear();
                ui->leposition->clear();
                ui->ledepo->clear();
                ui->cbiswork->setCurrentIndex(0);
                ui->btnback->setEnabled(false);
                ui->btforward->setEnabled(false);
            }
        } else {
            // Проверяем на ограничения внешнего ключа
            QString errorMsg = query.lastError().text();
            if (errorMsg.contains("foreign key") || errorMsg.contains("constraint")) {
                QMessageBox::critical(this, "Ошибка",
                                      "Невозможно удалить сотрудника, так как существуют связанные записи\n"
                                      "(пропуска, назначения и т.д.)");
            } else {
                QMessageBox::critical(this, "Ошибка",
                                      "Не удалось удалить сотрудника:\n" + errorMsg);
            }
        }
    }
}

void HrEditStaff::on_btnsearch_clicked()
{
    QString searchText = ui->lesearch->text().trimmed();

    if (searchText.isEmpty()) {
        QMessageBox::warning(this, "Предупреждение", "Введите данные для поиска");
        return;
    }

    QStringList parts = searchText.split(" | ");
    if (parts.size() != 2) {
        QMessageBox::warning(this, "Предупреждение",
                             "Используйте формат: ФИО | Телефон\n"
                             "Выберите сотрудника из выпадающего списка");
        return;
    }

    QString fio = parts[0].trimmed();
    QString phone = parts[1].trimmed();

    QSqlQuery query(db);
    query.prepare("SELECT staff_id FROM staff WHERE fio = :fio AND phone = :phone");
    query.bindValue(":fio", fio);
    query.bindValue(":phone", phone);

    if (query.exec() && query.next()) {
        int foundId = query.value(0).toInt();
        int foundIndex = staffIds.indexOf(foundId);

        if (foundIndex != -1) {
            currentIndex = foundIndex;
            loadStaffData(foundId);
            ui->lesearch->clear();
            ui->lesearch->setStyleSheet("");
        } else {
            QMessageBox::warning(this, "Ошибка", "Сотрудник найден, но не в списке навигации");
        }
    } else {
        QMessageBox::warning(this, "Не найдено",
                             QString("Сотрудник с ФИО '%1' и телефоном '%2' не найден").arg(fio, phone));
    }
}

void HrEditStaff::on_btnlogout_clicked()
{
    // Проверяем, не открыто ли уже окно HrWindow
    QWidget *existingHrWindow = nullptr;
    for (QWidget *widget : QApplication::topLevelWidgets()) {
        if (widget->objectName() == "HrWindow" || widget->inherits("HrWindow")) {
            existingHrWindow = widget;
            break;
        }
    }

    if (!existingHrWindow) {
        // Создаем новое окно HrWindow
        HrWindow *hrWindow = new HrWindow(login, password);
        hrWindow->setAttribute(Qt::WA_DeleteOnClose);
        hrWindow->show();
    } else {
        // Если окно уже открыто, просто активируем его
        existingHrWindow->activateWindow();
        existingHrWindow->raise();
    }

    // Закрываем текущее окно
    this->close();
}
