#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);
    loadSettingsToUi();
    ui->testatus->setReadOnly(true);
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

// Загрузка конфигурации из JSON файла
SettingsWindow::DbConfig SettingsWindow::loadConfig()
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
        qDebug() << "Invalid JSON format, using default values";
        config.host = "localhost";
        config.port = 228;
        return config;
    }

    QJsonObject obj = doc.object();

    config.host = obj["host"].toString("localhost");
    config.port = obj["port"].toInt(228);

    return config;
}

// Сохранение конфигурации в JSON файл
bool SettingsWindow::saveConfig(const DbConfig &config)
{
    QString configPath = QCoreApplication::applicationDirPath() + "/config.json";

    QJsonObject obj;
    obj["host"] = config.host;
    obj["port"] = config.port;
    QJsonDocument doc(obj);

    QFile file(configPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    file.write(doc.toJson());
    file.close();
    return true;
}

// Проверка соединения с БД
bool SettingsWindow::testDbConnection(const DbConfig &config)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(config.host);
    db.setPort(config.port);
    db.setDatabaseName(DB_NAME);
    db.setUserName(DB_USERNAME_LOGIN);
    db.setPassword(DB_PASSWORD_LOGIN);

    if (!db.open()) {
        return false;
    }

    db.close();
    return true;
}

// Загрузка настроек из конфига в UI
void SettingsWindow::loadSettingsToUi()
{
    DbConfig config = loadConfig();
    ui->lehostname->setText(config.host);
    ui->leport->setText(QString::number(config.port));
}

// Обработчик кнопки проверки подключения
void SettingsWindow::on_btncheckconn_clicked()
{
    // Получаем данные из UI
    DbConfig newConfig;
    newConfig.host = ui->lehostname->text().trimmed();
    newConfig.port = ui->leport->text().toInt();

    // Проверяем валидность порта
    if (newConfig.port <= 0 || newConfig.port > 65535) {
        ui->testatus->setText("Ошибка: Некорректный номер порта (1-65535)");
        ui->testatus->setStyleSheet("color: red;");
        return;
    }

    if (newConfig.host.isEmpty()) {
        ui->testatus->setText("Ошибка: Имя хоста не может быть пустым");
        ui->testatus->setStyleSheet("color: red;");
        return;
    }

    // Проверяем подключение
    if (testDbConnection(newConfig)) {
        // Если подключение успешно - сохраняем конфиг
        if (saveConfig(newConfig)) {
            ui->testatus->setText("Успешно: Подключение к серверу установлено!\n"
                                  "Настройки сохранены.\n\n"
                                  "Хост: " + newConfig.host + "\n"
                                                     "Порт: " + QString::number(newConfig.port) + "\n");
            ui->testatus->setStyleSheet("color: green;");
        } else {
            ui->testatus->setText("Ошибка: Не удалось сохранить файл конфигурации");
            ui->testatus->setStyleSheet("color: red;");
        }
    } else {
        // Если подключение не удалось - не меняем конфиг
        ui->testatus->setText("Ошибка: Не удалось подключиться к серверу.\n"
                              "Проверьте хост и порт.\n\n"
                              "Настройки НЕ сохранены.");
        ui->testatus->setStyleSheet("color: red;");
    }
}
