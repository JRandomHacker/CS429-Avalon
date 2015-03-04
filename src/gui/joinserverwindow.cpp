#include "joinserverwindow.h"
#include "connectwindow.h"
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

void JoinServerWindow::on_buttonJoinServer_clicked()
{
    ConnectWindow w;
    w.setModal(true);
    w.exec();
}
