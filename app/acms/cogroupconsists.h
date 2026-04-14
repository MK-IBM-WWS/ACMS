#ifndef COGROUPCONSISTS_H
#define COGROUPCONSISTS_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDate>
#include <QMessageBox>
#include <QTableWidgetItem>

namespace Ui {
class CoGroupConsists;
}

class CoGroupConsists : public QWidget
{
    Q_OBJECT

public:
    explicit CoGroupConsists(int groupId, QWidget *parent = nullptr);
    ~CoGroupConsists();

private slots:
    void on_btnok_clicked();

private:
    Ui::CoGroupConsists *ui;
    int m_groupId;

    void loadGroupInfo();
    void loadGroupConsists();
};

#endif // COGROUPCONSISTS_H
