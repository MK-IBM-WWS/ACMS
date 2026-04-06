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

LoginWindow::DbConfig LoginWindow::loadConfig()
{
    DbConfig config;

    QString configPath = QCoreApplication::applicationDirPath() + "/config.json";

    QFile file(configPath);
    if (!file.exists()) {
        qDebug() << "Config file not found, using default values";
        config.host = "localhost";
        config.port = 228;
        return config;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Cannot open config file";
        config.host = "localhost";
        config.port = 228;
        return config;
    }

    QByteArray jsonData = file.readAll();
    file.close();
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);

    if (doc.isNull()) {
        qDebug() << "Invalid config, using default values";
        config.host = "localhost";
        config.port = 228;
        return config;
    }

    QJsonObject obj = doc.object();

    config.host = obj["host"].toString("localhost");
    config.port = obj["port"].toInt(228);

    return config;
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
bool LoginWindow::authenticateUser(const QString &login, const QString &password, QString &role)
{
    DbConfig config = loadConfig();

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
        role = query.value(0).toString();
        result = true;
    }

    // Важно: сначала закрываем, потом удаляем
    query.finish(); // Завершаем запрос
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
    DbConfig config = loadConfig();

    if (!testDbConnection(config)) {
        QMessageBox::critical(this, "Ошибка соединения",
                              "Не удалось подключиться к серверу базы данных.\n"
                              "Проверьте настройки подключения\n\n"
                              "Хост: " + config.host + "\n"
                                                  "Порт: " + QString::number(config.port) + "\n");
        return;
    }

    // Аутентификация пользователя
    QString role;
    if (!authenticateUser(login, password, role)) {
        QMessageBox::warning(this, "Ошибка авторизации",
                             "Неверный логин или пароль");
        ui->lepass->clear();
        ui->lepass->setFocus();
        return;
    }

    // Успешная авторизация - открываем соответствующее окно
    QWidget *roleWindow = nullptr;

    if (role == "Admin") {
        roleWindow = new AdminWindow(login, nullptr);
    }
    else if (role == "HR") {
        roleWindow = new HrWindow(login, nullptr);
    }
    else if (role == "Controller") {
        roleWindow = new ControllerWindow(login, nullptr);
    }
    else if (role == "PD") {
        roleWindow = new PdWindow(login, nullptr);
    }
    else {
        QMessageBox::critical(this, "Ошибка",
                              "Неизвестная роль пользователя: " + role);
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
