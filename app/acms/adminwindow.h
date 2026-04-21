#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QWidget>
#include <QString>
#include <QSqlDatabase>
#include <QDate>
#include <printwindow.h>
#include <amaddcontrol.h>
#include <amadduser.h>
#include <ameditcontrol.h>
#include <amedituser.h>

namespace Ui {
class AdminWindow;
}

class AdminWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AdminWindow(QString &login,
                         QString &password,
                         QWidget *parent = nullptr);
    ~AdminWindow();

    QSqlDatabase& getDatabase() { return db; }
    QString getLogin() const { return m_login; }
    QString getPassword() const { return m_password; }
    bool reconnectToDatabase();

private slots:
    void on_btnlogout_clicked();
    void on_btnaddcontrol_clicked();
    void on_btneditcontrol_clicked();
    void on_btnadduser_clicked();
    void on_btnedituser_clicked();
    void on_btncontrolrepo_clicked();
    void on_btndelete_clicked();

private:
    Ui::AdminWindow *ui;
    QSqlDatabase db;
    QString m_login;
    QString m_password;

    bool connectToDatabase();
    void setDefaultDate();
    QString generateControllersReport();
    bool deleteOldAccessFacts(const QDate &endDate);
};

#endif // ADMINWINDOW_H
