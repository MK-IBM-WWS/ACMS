#ifndef AMADDUSER_H
#define AMADDUSER_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include "adminwindow.h"

class AdminWindow;

namespace Ui {
class AmAddUser;
}

class AmAddUser : public QWidget
{
    Q_OBJECT

public:
    explicit AmAddUser(AdminWindow *amWindow, QString login, QString password, QWidget *parent = nullptr);
    ~AmAddUser();

private slots:
    void on_btnadd_clicked();
    void on_btnlogout_clicked();

private:
    Ui::AmAddUser *ui;
    AdminWindow *m_adminWindow;
    QString m_currentLogin;
    QString m_currentPassword;

    void setupComboBox();
    bool addUserToTable(const QString &login, const QString &password, const QString &roleDb);
    bool createDatabaseUser(const QString &login, const QString &password, const QString &roleDb);
    void showErrorMessage(const QString &title, const QString &message);
    void showSuccessMessage(const QString &title, const QString &message);
};

#endif // AMADDUSER_H
