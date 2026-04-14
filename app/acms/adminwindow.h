#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QWidget>

namespace Ui {
class AdminWindow;
}

class AdminWindow : public QWidget
{
    Q_OBJECT

public:
    explicit AdminWindow(QString &login, QString &password, QWidget *parent = nullptr);
    ~AdminWindow();

private slots:
    void on_btnlogout_clicked();
private:
    Ui::AdminWindow *ui;
    QString m_login;
    QString m_password;
};

#endif // ADMINWINDOW_H
