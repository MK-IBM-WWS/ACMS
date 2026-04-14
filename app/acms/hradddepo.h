#ifndef HRADDDEPO_H
#define HRADDDEPO_H

#include <QWidget>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include "hrwindow.h"

class HrWindow;

namespace Ui {
class HrAddDepo;
}

class HrAddDepo : public QWidget
{
    Q_OBJECT

public:
    explicit HrAddDepo(HrWindow *hrWindow, QString m_login, QString m_password, QWidget *parent = nullptr);
    ~HrAddDepo();

private slots:
    void on_btnadd_clicked();
    void on_btnlogout_clicked();

private:
    bool ensureDatabaseConnection();
    bool addDepartment(const QString &name, const QString &address,
                       const QString &fio, const QString &activity);
    void clearFields();

    Ui::HrAddDepo *ui;
    QString login;
    QString password;
    QSqlDatabase db;
    HrWindow *mainHrWindow;
};

#endif // HRADDDEPO_H
