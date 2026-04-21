#ifndef PDEDITPASS_H
#define PDEDITPASS_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QCompleter>
#include <QStringListModel>
#include <QDate>

class PdWindow;

namespace Ui {
class PdEditPass;
}

class PdEditPass : public QWidget
{
    Q_OBJECT

public:
    explicit PdEditPass(PdWindow *pdWindow, const QString &login, const QString &password, QWidget *parent = nullptr);
    ~PdEditPass();

private slots:
    void on_btnlogout_clicked();
    void on_btnsearch_clicked();
    void on_btnforward_clicked();
    void on_btnback_clicked();
    void on_btnupdate_clicked();
    void on_btnsetend_clicked();
    void on_btndelete_clicked();
    void on_btnaddgroup_clicked();
    void on_btnremovegroup_clicked();
    void on_btnprintpass_clicked();

private:
    Ui::PdEditPass *ui;
    PdWindow *m_pdWindow;
    QString m_login;
    QString m_password;
    QSqlDatabase m_db;

    int m_currentPassId;
    QList<int> m_passIds;
    int m_currentIndex;

    QCompleter *m_searchCompleter;
    QStringListModel *m_completerModel;

    void loadPassData(int passId);
    void loadAccessGroups(int passId);
    void updateNavigationButtons();
    void updateSearchCompleter();
    bool validateDate(const QString &dateStr);
    QString generatePassHtml();
    void clearForm();
    bool executeQuery(QSqlQuery &query);
};

#endif // PDEDITPASS_H
