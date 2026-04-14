#ifndef HRWINDOW_H
#define HRWINDOW_H

#include <QWidget>
#include <QString>
#include <QtSql/QSqlDatabase>
#include "hradddepo.h"
#include "hreditdepo.h"
#include "hraddstaff.h"
#include "hreditstaff.h"
#include "printwindow.h"

namespace Ui {
class HrWindow;
}

class HrWindow : public QWidget
{
    Q_OBJECT

public:
    explicit HrWindow(QString &login, QString &password, QWidget *parent = nullptr);
    ~HrWindow();

    // Методы для работы с БД (публичные, чтобы дочерние окна могли их использовать)
    QSqlDatabase getDatabase() const { return db; }
    bool isDatabaseConnected() const { return db.isOpen(); }
    QString getLogin() const { return m_login; }
    QString getPassword() const { return m_password; }

    // Метод для переподключения к БД (если соединение потеряно)
    bool reconnectToDatabase();

private slots:
    void on_btnlogout_clicked();
    void on_btnaddstaff_clicked();
    void on_btneditstaff_clicked();
    void on_btnadddepo_clicked();
    void on_btneditdepo_clicked();
    void on_btnpositionreport_clicked();
    void on_btnaccessreport_clicked();

private:
    QString generatePositionReportHtml(const QString &position);
    QString generateAccessReportHtml(const QDate &startDate, const QDate &endDate);
    QString translateAccessType(const QString &type);
    bool connectToDatabase();
    void setDefaultDates();
    bool validateDate(const QString &date, const QString &fieldName);

    Ui::HrWindow *ui;
    QString m_login;
    QString m_password;
    QSqlDatabase db;  // Центральное соединение с БД
};

#endif // HRWINDOW_H
