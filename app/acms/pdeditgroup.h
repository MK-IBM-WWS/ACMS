#ifndef PDEDITGROUP_H
#define PDEDITGROUP_H

#include <QWidget>
#include <QSqlDatabase>
#include <QString>
#include <QDate>
#include <QCompleter>
#include "pdwindow.h"

class PdWindow;

namespace Ui {
class PdEditGroup;
}

class PdEditGroup : public QWidget
{
    Q_OBJECT

public:
    explicit PdEditGroup(PdWindow *pdWindow, const QString &login, const QString &password, QWidget *parent = nullptr);
    ~PdEditGroup();

private slots:
    void on_btnlogout_clicked();
    void on_btnsearch_clicked();
    void on_btnback_clicked();
    void on_btnforward_clicked();
    void on_btnupdate_clicked();
    void on_btnsave_clicked();
    void on_btndelete_clicked();
    void on_btnaddcontrol_clicked();
    void on_btnremovecontrol_clicked();

private:
    Ui::PdEditGroup *ui;
    PdWindow *m_pdWindow;
    QString m_login;
    QString m_password;

    int m_currentGroupId;
    QList<int> m_groupIds;
    int m_currentIndex;

    void loadGroupData(int groupId);
    void loadGroupIds();
    void loadControllersForGroup(int groupId);
    void navigateToGroup(int direction);
    bool validateDate(const QString &date, const QString &fieldName);
    bool saveGroupData(int groupId, const QString &groupName);
    bool deleteGroup(int groupId);
    bool removeControllerFromGroup(int positionId);
    bool addControllerToGroup(int groupId, int controllerId);
    void updateGroupChangeDate(int groupId);
    void setupSearchCompleter();
    void showErrorMessage(const QString &message);
    void showInfoMessage(const QString &message);
    bool ensureDatabaseConnection();
};

#endif // PDEDITGROUP_H
