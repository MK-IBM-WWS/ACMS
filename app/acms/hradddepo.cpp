#include "hradddepo.h"
#include "ui_hradddepo.h"

HrAddDepo::HrAddDepo(HrWindow *hrWindow, QString m_login, QString m_password, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HrAddDepo)
    , login(m_login)
    , mainHrWindow(hrWindow)
    , password(m_password)
{
    ui->setupUi(this);

    ui->llogin->setText(login);
    if (mainHrWindow) {
        db = mainHrWindow->getDatabase();
    }
}

HrAddDepo::~HrAddDepo()
{
    delete ui;
}

bool HrAddDepo::ensureDatabaseConnection()
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

bool HrAddDepo::addDepartment(const QString &name, const QString &address,
                              const QString &fio, const QString &activity)
{
    if (!ensureDatabaseConnection()) {
        QMessageBox::warning(this, "Ошибка", "Нет подключения к базе данных");
        return false;
    }

    QSqlQuery query(db);
    query.prepare("INSERT INTO departments (department_name, address, chief_fio, activity_type) "
                  "VALUES (:name, :address, :fio, :activity)");

    query.bindValue(":name", name);
    query.bindValue(":address", address);
    query.bindValue(":fio", fio);
    query.bindValue(":activity", activity);

    if (!query.exec()) {
        QString errorText = query.lastError().text();

        if (query.lastError().nativeErrorCode() == "23505") {
            // Проверяем, какое именно ограничение нарушено
            if (errorText.contains("unique_department", Qt::CaseInsensitive)) {
                QMessageBox::warning(this, "Дубликат отдела",
                                     QString("Отдел с названием '%1' и адресом '%2' уже существует в базе данных.\n"
                                             "Проверьте введенные данные или обратитесь к администратору.")
                                         .arg(name, address));
            } else {
                QMessageBox::warning(this, "Ошибка уникальности",
                                     "Запись с такими данными уже существует в базе данных.");
            }
        } else {
            QMessageBox::critical(this, "Ошибка базы данных",
                                  QString("Не удалось добавить отдел в базу данных.\n\n"
                                          "Код ошибки: %1\n"
                                          "Текст ошибки: %2")
                                      .arg(query.lastError().nativeErrorCode())
                                      .arg(errorText));
        }
        return false;
    }
    return true;
}

void HrAddDepo::clearFields()
{
    ui->ledeponame->clear();
    ui->leaddress->clear();
    ui->lefio->clear();
    ui->lework->clear();
}

void HrAddDepo::on_btnadd_clicked()
{
    // Проверяем, что все поля заполнены
    QString name = ui->ledeponame->text().trimmed();
    QString address = ui->leaddress->text().trimmed();
    QString fio = ui->lefio->text().trimmed();
    QString activity = ui->lework->text().trimmed();

    if (name.isEmpty()) {
        QMessageBox::warning(this, "Предупреждение", "Введите название отдела");
        ui->ledeponame->setFocus();
        return;
    }

    if (address.isEmpty()) {
        QMessageBox::warning(this, "Предупреждение", "Введите адрес отдела");
        ui->leaddress->setFocus();
        return;
    }

    if (fio.isEmpty()) {
        QMessageBox::warning(this, "Предупреждение", "Введите ФИО начальника отдела");
        ui->lefio->setFocus();
        return;
    }

    if (activity.isEmpty()) {
        QMessageBox::warning(this, "Предупреждение", "Введите род деятельности отдела");
        ui->lework->setFocus();
        return;
    }

    // Добавляем отдел в базу данных
    if (addDepartment(name, address, fio, activity)) {
        QMessageBox::information(this, "Успех", "Отдел успешно добавлен");
        clearFields();
        ui->ledeponame->setFocus();
    }
}

void HrAddDepo::on_btnlogout_clicked()
{
    // Проверяем, не открыто ли уже окно HrWindow
    QWidgetList topLevelWidgets = QApplication::topLevelWidgets();
    HrWindow *existingHrWindow = nullptr;

    for (QWidget *widget : topLevelWidgets) {
        existingHrWindow = qobject_cast<HrWindow*>(widget);
        if (existingHrWindow) {
            break;
        }
    }

    if (!existingHrWindow) {
        // Создаем новое окно HrWindow
        existingHrWindow = new HrWindow(login, password);
        existingHrWindow->show();
    } else {
        // Если окно уже существует, активируем его
        existingHrWindow->raise();
        existingHrWindow->activateWindow();
    }

    // Закрываем текущее окно
    this->close();
}
