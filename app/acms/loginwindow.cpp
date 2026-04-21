#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QDir>
#include <QCoreApplication>
#include <QUuid>
#include <adminwindow.h>
#include <controllerwindow.h>
#include <hrwindow.h>
#include <pdwindow.h>
#include <settingswindow.h>

LoginWindow::LoginWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::LoginWindow)
    , m_passwordVisible(false)
{
    ui->setupUi(this);
    ui->lepass->setEchoMode(QLineEdit::Password);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

bool LoginWindow::testDbConnection(const DbConfig &config)
{
    QString connectionName = QString("test_%1").arg(QUuid::createUuid().toString());

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", connectionName);
    db.setHostName(config.host);
    db.setPort(config.port);
    db.setDatabaseName(DB_NAME);
    db.setUserName(DB_USERNAME_LOGIN);
    db.setPassword(DB_PASSWORD_LOGIN);

    bool result = db.open();

    if (result) {
        db.close();
    }

    QSqlDatabase::removeDatabase(connectionName);
    return result;
}

bool LoginWindow::authenticateUser(const QString &login, const QString &password)
{
    DbConfig config;
    config.loadConfig();

    QString connectionName = QString("auth_%1").arg(QUuid::createUuid().toString());

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL", connectionName);
    db.setHostName(config.host);
    db.setPort(config.port);
    db.setDatabaseName(DB_NAME);
    db.setUserName(DB_USERNAME_LOGIN);
    db.setPassword(DB_PASSWORD_LOGIN);

    if (!db.open()) {
        QMessageBox::critical(this, "Ошибка БД",
                              "Не удалось подключиться к базе данных:\n" + db.lastError().text());
        QSqlDatabase::removeDatabase(connectionName);
        return false;
    }

    QSqlQuery query(db);
    query.prepare("SELECT user_role FROM users WHERE login = :login AND passphrase = :password");
    query.bindValue(":login", login);
    query.bindValue(":password", password);

    if (!query.exec()) {
        QMessageBox::critical(this, "Ошибка запроса",
                              "Ошибка выполнения запроса:\n" + query.lastError().text());
        db.close();
        QSqlDatabase::removeDatabase(connectionName);
        return false;
    }

    bool result = false;
    if (query.next()) {
        this->role = query.value(0).toString();
        result = true;
    }

    query.finish();
    db.close();
    QSqlDatabase::removeDatabase(connectionName);

    return result;
}

void LoginWindow::on_btnlogin_clicked()
{
    QString login = ui->lelogin->text().trimmed();
    QString password = ui->lepass->text();

    if (login.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка ввода",
                             "Пожалуйста, введите логин и пароль");
        return;
    }

    DbConfig config;
    config.loadConfig();

    if (!testDbConnection(config)) {
        QMessageBox::critical(this, "Ошибка соединения",
                              "Не удалось подключиться к серверу базы данных.\n"
                              "Проверьте настройки подключения\n\n"
                              "Хост: " + config.host + "\n"
                                                  "Порт: " + QString::number(config.port) + "\n");
        return;
    }

    if (!authenticateUser(login, password)) {
        QMessageBox::warning(this, "Ошибка авторизации",
                             "Неверный логин или пароль");
        ui->lepass->clear();
        ui->lepass->setFocus();
        return;
    }

    QWidget *roleWindow = nullptr;

    if (this->role == "Admin") {
        roleWindow = new AdminWindow(login, password, nullptr);
    }
    else if (this->role == "HR") {
        roleWindow = new HrWindow(login, password, nullptr);
    }
    else if (this->role == "Controller") {
        roleWindow = new ControllerWindow(login, password, nullptr);
    }
    else if (this->role == "PD") {
        roleWindow = new PdWindow(login, password, nullptr);
    }
    else {
        QMessageBox::critical(this, "Ошибка",
                              "Неизвестная роль пользователя: " + this->role);
        return;
    }

    if (roleWindow) {
        roleWindow->setAttribute(Qt::WA_DeleteOnClose);
        roleWindow->show();
        this->close();
    }
}

void LoginWindow::on_btnshowpass_clicked()
{
    m_passwordVisible = !m_passwordVisible;

    if (m_passwordVisible) {
        ui->lepass->setEchoMode(QLineEdit::Normal);
    } else {
        ui->lepass->setEchoMode(QLineEdit::Password);
    }
}

void LoginWindow::on_btnsettings_clicked(){
    SettingsWindow *settingsWindow = new SettingsWindow(nullptr);
    settingsWindow->setAttribute(Qt::WA_DeleteOnClose);
    settingsWindow->show();
}
