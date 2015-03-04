#include "createserverwindow.h"
#include "ui_createserverwindow.h"

CreateServerWindow::CreateServerWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateServerWindow)
{
    ui->setupUi(this);
}

CreateServerWindow::~CreateServerWindow()
{
    delete ui;
}
