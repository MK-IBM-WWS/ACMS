#include "adminwindow.h"
#include "ui_adminwindow.h"
#include "loginwindow.h"
#include "dbconfig.h"

AdminWindow::AdminWindow(QString &login, QString &password, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AdminWindow)
    , m_login(login), m_password(password)
{
    ui->setupUi(this);
    ui->llogin->setText(login);
}

AdminWindow::~AdminWindow()
{
    delete ui;
}

void AdminWindow::on_btnlogout_clicked()
{
    LoginWindow *loginWindow = new LoginWindow(nullptr);
    loginWindow->setAttribute(Qt::WA_DeleteOnClose);
    loginWindow->show();

    this->close();
}
