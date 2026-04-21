#ifndef AMEDITUSER_H
#define AMEDITUSER_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QCompleter>
#include <QStringListModel>
#include "adminwindow.h"

class AdminWindow;

namespace Ui {
class AmEditUser;
}

class AmEditUser : public QWidget
{
    Q_OBJECT

public:
    explicit AmEditUser(AdminWindow *amWindow, const QString &login, const QString &password, QWidget *parent = nullptr);
    ~AmEditUser();

private slots:
    void on_btnlogout_clicked();
    void on_btnsearch_clicked();
    void on_btnback_clicked();
    void on_btforward_clicked();
    void on_btnupdate_clicked();
    void on_btnsave_clicked();
    void on_btndelete_clicked();
    void on_btnshowpass_clicked();

private:
    Ui::AmEditUser *ui;
    AdminWindow *m_adminWindow;
    QString m_currentLogin;
    QString m_currentPassword;
    QList<int> m_userIds;
    int m_currentIndex;
    bool m_passwordVisible;

    void loadUserList();
    void loadUserData(int userId);
    void updateNavigationButtons();
    void setupCompleter();
    QString getRoleDisplayName(const QString &roleDb);
    bool updateUserPassword(int userId, const QString &newPassword);
    bool deleteDatabaseUser(const QString &login);
    void showErrorMessage(const QString &title, const QString &message);
    void showSuccessMessage(const QString &title, const QString &message);
};

#endif // AMEDITUSER_H
