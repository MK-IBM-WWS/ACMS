#ifndef PDWINDOW_H
#define PDWINDOW_H

#include <QWidget>

namespace Ui {
class PdWindow;
}

class PdWindow : public QWidget
{
    Q_OBJECT

public:
    explicit PdWindow(QString &login, QWidget *parent = nullptr);
    ~PdWindow();
private slots:
    void on_btnlogout_clicked();
private:
    Ui::PdWindow *ui;
    QString m_login;
};

#endif // PDWINDOW_H
