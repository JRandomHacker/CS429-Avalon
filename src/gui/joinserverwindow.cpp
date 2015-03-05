#include "joinserverwindow.h"
#include "connectwindow.h"
#include "ui_joinserverwindow.h"
#include "globals.hpp"

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
    std::string addr = ui->fieldServerAddr->text().toStdString();
    ConnectWindow w(this, addr, DEFAULT_PORT);
    w.setModal(true);
    w.exec();
}
