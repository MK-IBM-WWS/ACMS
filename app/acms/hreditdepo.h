#ifndef HREDITDEPO_H
#define HREDITDEPO_H

#include <QWidget>
#include <QString>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QMessageBox>
#include <QCompleter>
#include <QStringListModel>
#include <QTextDocument>
#include <QPrinter>
#include <QPageSize>
#include <QFileDialog>
#include <QDesktopServices>
#include <QUrl>
#include <QDateTime>
#include <QApplication>
#include <QDir>
#include <hreditstaff.h>
#include <hrwindow.h>
#include "printwindow.h"


class HrWindow;

namespace Ui {
class HrEditDepo;
}

class HrEditDepo : public QWidget
{
    Q_OBJECT

public:
    explicit HrEditDepo(HrWindow *hrWindow, QString m_login, QString m_password, QWidget *parent = nullptr);
    ~HrEditDepo();

private slots:
    void on_btnlogout_clicked();
    void on_btnback_clicked();
    void on_btnforward_clicked();
    void on_btnupdate_clicked();
    void on_btnsearch_clicked();
    void on_btnsave_clicked();
    void on_btndelete_clicked();
    void on_btnabout_clicked();
    void on_btnsavereport_clicked();

private:
    bool ensureDatabaseConnection();
    void loadDepartmentsList();
    bool loadDepartmentData(int departmentId);
    void loadStaffData(int departmentId);
    void updateDepartmentFields();
    void clearDepartmentFields();
    bool saveDepartmentChanges();
    bool deleteDepartment();
    void setupSearchCompleter();
    bool moveToDepartment(int index);
    QString generateDepartmentReportHtml();

    Ui::HrEditDepo *ui;
    QString login;
    QString password;
    QSqlDatabase db;

    QList<int> departmentIds;
    int currentDepartmentIndex;
    int currentDepartmentId;

    QStringList searchSuggestions;
    QCompleter *searchCompleter;
    QStringListModel *completerModel;
    HrWindow *mainHrWindow;
};

#endif // HREDITDEPO_H
