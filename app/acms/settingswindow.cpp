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
    DbConfig config;
    config.loadConfig();
    ui->lehostname->setText(config.host);
    ui->leport->setText(QString::number(config.port));
}

// Обработчик кнопки проверки подключения
void SettingsWindow::on_btncheckconn_clicked()
{
    DbConfig newConfig;
    newConfig.host = ui->lehostname->text().trimmed();
    newConfig.port = ui->leport->text().toInt();

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

    if (testDbConnection(newConfig)) {
        if (newConfig.saveConfig()) {
            ui->testatus->setText("Успешно: Подключение к серверу установлено! Настройки сохранены.\n\n"
                                  "Хост: " + newConfig.host + "\n"
                                                     "Порт: " + QString::number(newConfig.port));
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
