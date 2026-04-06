#ifndef CONTROLLERWINDOW_H
#define CONTROLLERWINDOW_H

#include <QWidget>

namespace Ui {
class ControllerWindow;
}

class ControllerWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ControllerWindow(QString &login, QWidget *parent = nullptr);
    ~ControllerWindow();
private slots:
    void on_btnlogout_clicked();
private:
    Ui::ControllerWindow *ui;
    QString m_login;
};

#endif // CONTROLLERWINDOW_H
