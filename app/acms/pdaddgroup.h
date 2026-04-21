#ifndef PDADDGROUP_H
#define PDADDGROUP_H

#include <QWidget>
#include <QSqlDatabase>
#include <QString>
#include <QDate>
#include "pdwindow.h"

class PdWindow;

namespace Ui {
class PdAddGroup;
}

class PdAddGroup : public QWidget
{
    Q_OBJECT

public:
    explicit PdAddGroup(PdWindow *pdWindow, const QString &login, const QString &password, QWidget *parent = nullptr);
    ~PdAddGroup();

private slots:
    void on_btnadd_clicked();
    void on_btnlogout_clicked();

private:
    Ui::PdAddGroup *ui;
    PdWindow *m_pdWindow;
    QString m_login;
    QString m_password;

    bool validateDate(const QString &date, const QString &fieldName);
    bool addAccessGroup(const QString &groupName, const QDate &changeDate);
    void showErrorMessage(const QString &message);
    void showInfoMessage(const QString &message);
};

#endif // PDADDGROUP_H
