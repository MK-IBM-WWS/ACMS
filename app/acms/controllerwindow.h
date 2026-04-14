#ifndef CONTROLLERWINDOW_H
#define CONTROLLERWINDOW_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDateTime>
#include <QDebug>
#include <QCompleter>
#include <QStringListModel>
#include <cogroupconsists.h>

QT_BEGIN_NAMESPACE
namespace Ui { class ControllerWindow; }
QT_END_NAMESPACE

class ControllerWindow : public QWidget
{
    Q_OBJECT

public:
    ControllerWindow(QString &login, QString &password, QWidget *parent = nullptr);
    ~ControllerWindow();

private slots:
    void on_btnlogout_clicked();
    void on_lenow_clicked();
    void on_btnregister_clicked();
    void on_lecheckgroup_clicked();
    void on_btngroupconsist_clicked();

    void on_lepass_textChanged(const QString &text);
    void on_lecontrol_textChanged(const QString &text);
    void on_lecontrol_editingFinished();

private:
    Ui::ControllerWindow *ui;
    QString m_login;
    QString m_password;
    QSqlDatabase db;

    const QString DB_USERNAME = "control";

    QCompleter *m_controllerCompleter;
    QStringListModel *m_controllerModel;

    bool connectToDatabase();
    void setupComboBoxes();
    void setupControllerAutoComplete();
    bool insertAccessFact(int passId, const QString &objectName, const QString &address,
                          const QString &accessType, const QString &solution, const QDateTime &accessTime);
    bool checkPassExists(int passId);
    bool getControllerIdByNameAndAddress(const QString &objectName, const QString &address, int &controllerId);
    void loadPassGroups(int passId);
    QString getStaffNameByPassId(int passId);
    void clearForm();
};

#endif // CONTROLLERWINDOW_H
