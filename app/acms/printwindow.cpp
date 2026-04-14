#include "printwindow.h"
#include "ui_printwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QDir>
#include <QDate>
#include <QApplication>
#include <QDebug>

PrintWindow::PrintWindow(const QString &htmlContent, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PrintWindow)
    , m_htmlContent(htmlContent)
{
    ui->setupUi(this);

    setWindowTitle("Предварительный просмотр отчета");

    // Устанавливаем HTML содержимое в QTextEdit
    ui->teexample->setHtml(m_htmlContent);

    // Настройка QTextEdit для режима только чтения
    ui->teexample->setReadOnly(true);

    qDebug() << "PrintWindow создано, HTML загружен";
}

PrintWindow::~PrintWindow()
{
    delete ui;
    qDebug() << "PrintWindow уничтожено";
}

bool PrintWindow::saveToPdf(const QString &filename)
{
    // Создаем документ из HTML
    QTextDocument doc;
    doc.setHtml(m_htmlContent);

    // Настройка размера страницы A4
    doc.setPageSize(QSizeF(210 * 72 / 25.4, 297 * 72 / 25.4));

    // Настройка принтера для PDF
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filename);
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageMargins(QMarginsF(15, 15, 15, 15), QPageLayout::Millimeter);

    // Печатаем документ
    doc.print(&printer);

    return true;
}

void PrintWindow::on_btnprint_clicked()
{
    // Формируем имя файла по умолчанию
    QString defaultFileName = QString("Отчет_%1.pdf")
                                  .arg(QDate::currentDate().toString("yyyy-MM-dd"));

    // Диалог сохранения файла
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Сохранить отчет в PDF",
                                                    QDir::homePath() + "/" + defaultFileName,
                                                    "PDF Documents (*.pdf);;All Files (*.*)");

    if (fileName.isEmpty()) {
        return;
    }

    // Добавляем расширение .pdf если его нет
    if (!fileName.endsWith(".pdf", Qt::CaseInsensitive)) {
        fileName += ".pdf";
    }

    // Устанавливаем курсор ожидания
    QApplication::setOverrideCursor(Qt::WaitCursor);

    // Сохраняем PDF
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
    qDebug() << "Закрытие окна предварительного просмотра";
    this->close();
}
