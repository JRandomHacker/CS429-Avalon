#include "joinserverwindow.h"
#include "ui_joinserverwindow.h"

JoinServerWindow::JoinServerWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::JoinServerWindow)
{
    ui->setupUi(this);
}

JoinServerWindow::~JoinServerWindow()
{
    delete ui;
}
