#ifndef PDADDPASS_H
#define PDADDPASS_H

#include <QWidget>
#include <QSqlDatabase>
#include <QString>
#include <QDate>
#include <QCompleter>
#include <QMap>
#include "pdwindow.h"

class PdWindow;

namespace Ui {
class PdAddPass;
}

class PdAddPass : public QWidget
{
    Q_OBJECT

public:
    explicit PdAddPass(PdWindow *pdWindow, const QString &login, const QString &password, QWidget *parent = nullptr);
    ~PdAddPass();

private slots:
    void on_btnadd_clicked();
    void on_btnlogout_clicked();

private:
    Ui::PdAddPass *ui;
    PdWindow *m_pdWindow;
    QString m_login;
    QString m_password;

    QMap<QString, int> m_staffMap;
    QMap<QString, int> m_departmentMap;

    void setupStaffCompleter();
    void setupDepartmentCompleter();
    bool validateDate(const QString &date, const QString &fieldName);
    bool addPass(int staffId, const QDate &issueDate, const QDate &expirationDate, int departmentId);
    void clearFields();
    void showErrorMessage(const QString &message);
    void showInfoMessage(const QString &message);
    bool ensureDatabaseConnection();
};

#endif // PDADDPASS_H
