#ifndef AMEDITCONTROL_H
#define AMEDITCONTROL_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QRegularExpression>
#include <QCompleter>
#include <QStringListModel>

class AdminWindow;

namespace Ui {
class AmEditControl;
}

class AmEditControl : public QWidget
{
    Q_OBJECT

public:
    explicit AmEditControl(AdminWindow *amWindow, QString login, QString password, QWidget *parent = nullptr);
    ~AmEditControl();

private slots:
    void on_btnlogout_clicked();
    void on_btnback_clicked();
    void on_btnforward_clicked();
    void on_btnupdate_clicked();
    void on_btnsave_clicked();
    void on_btndelete_clicked();
    void on_btnsearch_clicked();

private:
    Ui::AmEditControl *ui;
    AdminWindow *m_adminWindow;
    QString m_login;
    QString m_password;
    int m_currentControllerId;
    int m_minControllerId;
    int m_maxControllerId;
    QCompleter *m_completer;
    QStringListModel *m_completerModel;

    void setupStatusComboBox();
    bool validateIPv4(const QString &ip);
    void loadControllerData(int controllerId);
    void loadControllerIds();
    void updateCompleterModel();
    bool checkUniqueConstraint(const QString &objectName, const QString &address, int excludeControllerId);
    void updateNavigationButtons();
};

#endif // AMEDITCONTROL_H
