#ifndef HREDITSTAFF_H
#define HREDITSTAFF_H

#include <QWidget>
#include <QSqlDatabase>
#include <QCompleter>
#include <QStringListModel>
#include <QList>
#include "hrwindow.h"

class HrWindow;

namespace Ui {
class HrEditStaff;
}

class HrEditStaff : public QWidget
{
    Q_OBJECT

public:
    explicit HrEditStaff(HrWindow *hrWindow, QString &m_login, QString &m_password, int staff_id = -1, QWidget *parent = nullptr);
    ~HrEditStaff();

private slots:
    void on_btnback_clicked();
    void on_btforward_clicked();
    void on_btnupdate_clicked();
    void on_btnsave_clicked();
    void on_btndelete_clicked();
    void on_btnsearch_clicked();
    void on_btnlogout_clicked();
    void on_ledepo_textChanged(const QString &text);
    void on_lesearch_textChanged(const QString &text);

private:
    Ui::HrEditStaff *ui;
    QSqlDatabase db;
    QString login;
    QString password;
    int currentStaffId;
    QList<int> staffIds;
    int currentIndex;

    QCompleter *depCompleter;
    QStringListModel *depModel;
    QCompleter *searchCompleter;
    QStringListModel *searchModel;
    HrWindow *mainHrWindow;

    bool ensureDatabaseConnection();
    void loadDepartments();
    void loadStaffList();
    void loadStaffData(int staffId);
    int getDepartmentId(const QString &depoText);
    QString getDepartmentString(int departmentId);
    void navigateToStaff(int direction);
    void loadSearchSuggestions();
};

#endif // HREDITSTAFF_H
