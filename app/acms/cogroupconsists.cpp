#include "cogroupconsists.h"
#include "ui_cogroupconsists.h"

CoGroupConsists::CoGroupConsists(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CoGroupConsists)
{
    ui->setupUi(this);
}

CoGroupConsists::~CoGroupConsists()
{
    delete ui;
}
