#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDebug>
#include <QCoreApplication>
#include <dbconfig.h>

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = nullptr);
    ~SettingsWindow();

private slots:
    void on_btncheckconn_clicked();

private:
    Ui::SettingsWindow *ui;

    const QString DB_USERNAME_LOGIN = "loginuser";
    const QString DB_PASSWORD_LOGIN = "CV6";
    const QString DB_NAME = "acms";

    bool saveConfig(const DbConfig &config);
    bool testDbConnection(const DbConfig &config);
    void loadSettingsToUi();
};

#endif // SETTINGSWINDOW_H
