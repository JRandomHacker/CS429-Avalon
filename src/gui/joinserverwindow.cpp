#include "joinserverwindow.h"
#include "connectwindow.h"
#include "ui_joinserverwindow.h"
#include "globals.hpp"

JoinServerWindow::JoinServerWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::JoinServerWindow)
{
    ui->setupUi(this);
    
    ui->fieldPortNum->insert(QString(std::to_string(DEFAULT_PORT).c_str()));
    ui->fieldServerAddr->insert(QString("localhost"));
}

JoinServerWindow::~JoinServerWindow()
{
    delete ui;
}

void JoinServerWindow::on_buttonJoinServer_clicked()
{
    std::string addr = ui->fieldServerAddr->text().toStdString();
    int port = ui->fieldPortNum->text().toInt();
    ConnectWindow w(this, addr, port);
    w.exec();
    
    close();
}
