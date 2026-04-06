#include "pdwindow.h"
#include "ui_pdwindow.h"
#include "loginwindow.h"

PdWindow::PdWindow(QString &login, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PdWindow)
    , m_login(login)
{
    ui->setupUi(this);
    ui->llogin->setText(login);
}

PdWindow::~PdWindow()
{
    delete ui;
}

void PdWindow::on_btnlogout_clicked()
{
    LoginWindow *loginWindow = new LoginWindow(nullptr);
    loginWindow->setAttribute(Qt::WA_DeleteOnClose);
    loginWindow->show();

    this->close();
}
