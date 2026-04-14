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
{
    ui->setupUi(this);
    ui->lepass->setEchoMode(QLineEdit::Password);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

// Проверка соединения с БД
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

// Аутентификация пользователя
bool LoginWindow::authenticateUser(const QString &login, const QString &password)
{
    DbConfig config;
    config.loadConfig();

    // Используем уникальное имя для соединения
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
    query.prepare("SELECT user_role, group_pass FROM users WHERE login = :login AND passphrase = :password");
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
        this->groupPass = query.value(1).toString();
        result = true;
    }

    query.finish();
    db.close();
    QSqlDatabase::removeDatabase(connectionName);

    return result;
}

// Обработчик кнопки входа
void LoginWindow::on_btnlogin_clicked()
{
    QString login = ui->lelogin->text().trimmed();
    QString password = ui->lepass->text();

    // Проверка на пустые поля
    if (login.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка ввода",
                             "Пожалуйста, введите логин и пароль");
        return;
    }

    // Загружаем конфиг и проверяем соединение
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

    // Успешная авторизация - открываем соответствующее окно
    QWidget *roleWindow = nullptr;

    if (this->role == "Admin") {
        roleWindow = new AdminWindow(login, password, nullptr);
    }
    else if (this->role == "HR") {
        roleWindow = new HrWindow(login, password, nullptr);
    }
    else if (this->role == "Controller") {
        roleWindow = new ControllerWindow(login, this->groupPass, nullptr);
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

// Обработчик кнопки настроек
void LoginWindow::on_btnsettings_clicked(){
    SettingsWindow *settingsWindow = new SettingsWindow(nullptr);
    settingsWindow->setAttribute(Qt::WA_DeleteOnClose);
    settingsWindow->show();
}
