#include "printwindow.h"
#include "ui_printwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QDir>
#include <QDate>
#include <QApplication>

PrintWindow::PrintWindow(const QString &htmlContent, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PrintWindow)
    , m_htmlContent(htmlContent)
{
    ui->setupUi(this);

    setWindowTitle("Предварительный просмотр отчета");

    ui->teexample->setHtml(m_htmlContent);
    ui->teexample->setReadOnly(true);
}

PrintWindow::~PrintWindow()
{
    delete ui;
}

bool PrintWindow::saveToPdf(const QString &filename)
{
    QTextDocument doc;
    doc.setHtml(m_htmlContent);

    doc.setPageSize(QSizeF(210 * 72 / 25.4, 297 * 72 / 25.4));

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filename);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageMargins(QMarginsF(15, 15, 15, 15), QPageLayout::Millimeter);
    doc.print(&printer);

    return true;
}

void PrintWindow::on_btnprint_clicked()
{
    QString defaultFileName = QString("Отчет_%1.pdf")
                                  .arg(QDate::currentDate().toString("yyyy-MM-dd"));
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Сохранить отчет в PDF",
                                                    QDir::homePath() + "/" + defaultFileName,
                                                    "PDF Documents (*.pdf);;All Files (*.*)");

    if (fileName.isEmpty()) {
        return;
    }

    if (!fileName.endsWith(".pdf", Qt::CaseInsensitive)) {
        fileName += ".pdf";
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);

    bool success = saveToPdf(fileName);

    QApplication::restoreOverrideCursor();

    if (success) {
        QMessageBox::information(this, "Успех",
                                 QString("Отчет успешно сохранен!\n\nФайл: %1").arg(fileName));
    } else {
        QMessageBox::critical(this, "Ошибка",
                              "Не удалось создать отчет.\nПроверьте права доступа к выбранной папке.");
    }
}

void PrintWindow::on_btnback_clicked()
{
    this->close();
}
