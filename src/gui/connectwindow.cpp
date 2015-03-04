#include "connectwindow.h"
#include "gamewindow.h"
#include "ui_connectwindow.h"
#include "clientController.hpp"

ConnectWindow::ConnectWindow(QWidget *parent, std::string ip, int port) :
    QDialog(parent),
    ui(new Ui::ConnectWindow)
{
    ui->setupUi(this);
    
    joinServer(ip, port);
}

ConnectWindow::~ConnectWindow()
{
    delete ui;
}

void ConnectWindow::on_dummyConnect_clicked()
{
    GameWindow g;
    g.setModal(true);
    g.exec();
}

void ConnectWindow::joinServer( std::string host, int port ) {
    ClientController controller( NULL, host, port );
    controller.processActions( );
}

