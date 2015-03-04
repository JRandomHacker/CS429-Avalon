#include "createserverwindow.h"
#include "connectwindow.h"
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

void CreateServerWindow::on_buttonCreateServer_clicked()
{
    ConnectWindow w;
    w.setModal(true);
    w.exec();
}
