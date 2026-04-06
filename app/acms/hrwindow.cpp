#include "hrwindow.h"
#include "ui_hrwindow.h"
#include "loginwindow.h"

HrWindow::HrWindow(QString &login, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HrWindow)
    , m_login(login)
{
    ui->setupUi(this);
    ui->llogin->setText(login);
}

HrWindow::~HrWindow()
{
    delete ui;
}

void HrWindow::on_btnlogout_clicked()
{
    LoginWindow *loginWindow = new LoginWindow(nullptr);
    loginWindow->setAttribute(Qt::WA_DeleteOnClose);
    loginWindow->show();

    this->close();
}
