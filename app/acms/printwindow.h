#ifndef PRINTWINDOW_H
#define PRINTWINDOW_H

#include <QWidget>
#include <QString>
#include <QTextDocument>
#include <QPrinter>

namespace Ui {
class PrintWindow;
}

class PrintWindow : public QWidget
{
    Q_OBJECT

public:
    explicit PrintWindow(const QString &htmlContent, QWidget *parent = nullptr);
    ~PrintWindow();

private slots:
    void on_btnprint_clicked();
    void on_btnback_clicked();

private:
    bool saveToPdf(const QString &filename);

    Ui::PrintWindow *ui;
    QString m_htmlContent;
};

#endif // PRINTWINDOW_H
