#ifndef AMADDCONTROL_H
#define AMADDCONTROL_H

#include <QWidget>
#include <QSqlDatabase>
#include <QRegularExpression>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

class AdminWindow;

namespace Ui {
class AmAddControl;
}

class AmAddControl : public QWidget
{
    Q_OBJECT

public:
    explicit AmAddControl(AdminWindow *amWindow, QString login, QString password, QWidget *parent = nullptr);
    ~AmAddControl();

private slots:
    void on_btnadd_clicked();
    void on_btnlogout_clicked();

private:
    Ui::AmAddControl *ui;
    AdminWindow *m_adminWindow;
    QString m_login;
    QString m_password;

    bool validateIPv4(const QString &ip);
    void clearFields();
    void setupStatusComboBox();
};

#endif // AMADDCONTROL_H
