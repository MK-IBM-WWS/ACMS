#ifndef PDWINDOW_H
#define PDWINDOW_H

#include <QWidget>
#include <QString>
#include <QDate>
#include <QSqlDatabase>
#include <printwindow.h>
#include "pdaddpass.h"
#include "pdeditpass.h"
#include "pdaddgroup.h"
#include "pdeditgroup.h"

namespace Ui {
class PdWindow;
}

class DbConfig;

class PdWindow : public QWidget
{
    Q_OBJECT

public:
    explicit PdWindow(QString &login, QString &password, QWidget *parent = nullptr);
    ~PdWindow();

    QSqlDatabase getDatabase() const { return m_db; }
    bool isDatabaseConnected() const { return m_db.isOpen(); }
    QString getLogin() const { return m_login; }
    QString getPassword() const { return m_password; }
    bool reconnectToDatabase();

private slots:
    void on_btnlogout_clicked();
    void on_btnnewpass_clicked();
    void on_btneditpass_clicked();
    void on_btnaddgroup_clicked();
    void on_btneditgroup_clicked();
    void on_btnobjectsreport_clicked();
    void on_btnpassreport_clicked();

private:
    void initDateFields();
    bool connectToDatabase();
    QString generateObjectsReport();
    QString generatePassReport(const QDate &startDate, const QDate &endDate);

private:
    Ui::PdWindow *ui;
    QSqlDatabase m_db;
    QString m_login;
    QString m_password;
    DbConfig *m_dbConfig;
};

#endif // PDWINDOW_H
