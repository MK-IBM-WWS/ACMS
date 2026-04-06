#include "controllerwindow.h"
#include "ui_controllerwindow.h"
#include "loginwindow.h"

ControllerWindow::ControllerWindow(QString &login, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ControllerWindow)
    , m_login(login)
{
    ui->setupUi(this);
    ui->llogin->setText(login);
}

ControllerWindow::~ControllerWindow()
{
    delete ui;
}

void ControllerWindow::on_btnlogout_clicked()
{
    LoginWindow *loginWindow = new LoginWindow(nullptr);
    loginWindow->setAttribute(Qt::WA_DeleteOnClose);
    loginWindow->show();

    this->close();
}
