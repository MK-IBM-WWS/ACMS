#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class LoginWindow; }
QT_END_NAMESPACE

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private slots:
    void on_btnlogin_clicked();
    void on_btnsettings_clicked();

private:
    Ui::LoginWindow *ui;

    struct DbConfig {
        QString host;
        int port;
    };

    const QString DB_USERNAME_LOGIN = "loginuser";
    const QString DB_PASSWORD_LOGIN = "CV6";
    const QString DB_NAME = "acms";

    DbConfig loadConfig();
    bool testDbConnection(const DbConfig &config);
    bool authenticateUser(const QString &login, const QString &password, QString &role);
};

#endif // LOGINWINDOW_H
