#ifndef HRADDSTAFF_H
#define HRADDSTAFF_H

#include <QWidget>
#include <QSqlDatabase>
#include <QCompleter>
#include <QStringListModel>
#include "hrwindow.h"

class HrWindow;

namespace Ui {
class HrAddStaff;
}

class HrAddStaff : public QWidget
{
    Q_OBJECT

public:
    explicit HrAddStaff(HrWindow *hrWindow, QString &m_login, QString &m_password, QWidget *parent = nullptr);
    ~HrAddStaff();

private slots:
    void on_btnadd_clicked();
    void on_btnlogout_clicked();
    void on_ledepo_textChanged(const QString &text);

private:
    Ui::HrAddStaff *ui;
    QSqlDatabase db;
    QString login;
    QString password;
    QCompleter *depCompleter;
    QStringListModel *depModel;
    HrWindow *mainHrWindow;

    bool ensureDatabaseConnection();
    void loadDepartments();
    int getDepartmentId(const QString &depoText);
    void clearFields();
};

#endif // HRADDSTAFF_H
