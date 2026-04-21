#include "amaddcontrol.h"
#include "ui_amaddcontrol.h"
#include "adminwindow.h"

AmAddControl::AmAddControl(AdminWindow *amWindow, QString login, QString password, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AmAddControl)
    , m_adminWindow(amWindow)
    , m_login(login)
    , m_password(password)
{
    ui->setupUi(this);
    ui->llogin->setText(m_login);

    setupStatusComboBox();
}

AmAddControl::~AmAddControl()
{
    delete ui;
}

void AmAddControl::setupStatusComboBox()
{
    ui->cbstatus->addItem("Online");
    ui->cbstatus->addItem("Offline");
    ui->cbstatus->addItem("Restarting");
    ui->cbstatus->setCurrentText("Offline");
}

bool AmAddControl::validateIPv4(const QString &ip)
{
    QRegularExpression ipRegex("^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}"
                               "(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");

    QRegularExpressionMatch match = ipRegex.match(ip);
    return match.hasMatch();
}

void AmAddControl::clearFields()
{
    ui->leobjectname->clear();
    ui->leaddress->clear();
    ui->leshedule->clear();
    ui->cbstatus->setCurrentText("Offline");
    ui->lemodel->clear();
    ui->leipv4->clear();
}

void AmAddControl::on_btnadd_clicked()
{
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

    QSqlDatabase db = m_adminWindow->getDatabase();

    if (!db.isOpen()) {
        QMessageBox::critical(this, "Ошибка", "Отсутствует подключение к базе данных!");
        return;
    }

    QSqlQuery checkQuery(db);
    checkQuery.prepare("SELECT COUNT(*) FROM access_controller "
                       "WHERE object_name = :object_name AND address = :address");
    checkQuery.bindValue(":object_name", objectName);
    checkQuery.bindValue(":address", address);

    if (!checkQuery.exec()) {
        QMessageBox::critical(this, "Ошибка",
                              "Ошибка при проверке уникальности: " + checkQuery.lastError().text());
        return;
    }

    checkQuery.next();
    if (checkQuery.value(0).toInt() > 0) {
        QMessageBox::warning(this, "Предупреждение",
                             "Контроллер с таким объектом и адресом уже существует!");
        return;
    }

    if (!db.transaction()) {
        QMessageBox::critical(this, "Ошибка", "Не удалось начать транзакцию!");
        return;
    }

    QSqlQuery insertQuery(db);
    insertQuery.prepare("INSERT INTO access_controller "
                        "(object_name, address, work_shedule, network_address, model, status) "
                        "VALUES (:object_name, :address, :work_shedule, :network_address, :model, :status)");

    insertQuery.bindValue(":object_name", objectName);
    insertQuery.bindValue(":address", address);
    insertQuery.bindValue(":work_shedule", workSchedule.isEmpty() ? QVariant() : workSchedule);
    insertQuery.bindValue(":network_address", networkAddress);
    insertQuery.bindValue(":model", model.isEmpty() ? QVariant() : model);
    insertQuery.bindValue(":status", status);

    if (!insertQuery.exec()) {
        db.rollback();
        QMessageBox::critical(this, "Ошибка",
                              "Не удалось добавить контроллер: " + insertQuery.lastError().text());
        return;
    }

    if (!db.commit()) {
        QMessageBox::critical(this, "Ошибка", "Не удалось подтвердить транзакцию!");
        return;
    }

    QMessageBox::information(this, "Успех", "Контроллер доступа успешно добавлен!");

    clearFields();
}

void AmAddControl::on_btnlogout_clicked()
{
    if (!m_adminWindow->isVisible()) {
        AdminWindow *newAdminWindow = new AdminWindow(m_login, m_password);
        newAdminWindow->show();
    }

    this->close();
}
