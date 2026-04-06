#ifndef HRWINDOW_H
#define HRWINDOW_H

#include <QWidget>

namespace Ui {
class HrWindow;
}

class HrWindow : public QWidget
{
    Q_OBJECT

public:
    explicit HrWindow(QString &login, QWidget *parent = nullptr);
    ~HrWindow();

private slots:
    void on_btnlogout_clicked();

private:
    Ui::HrWindow *ui;
    QString m_login;
};

#endif // HRWINDOW_H
