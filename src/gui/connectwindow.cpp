#include "connectwindow.h"
#include "gamewindow.h"
#include "ui_connectwindow.h"
#include "clientController.hpp"
#include "model.hpp"

ConnectWindow::ConnectWindow(QWidget *parent, std::string ip, int port) :
    QDialog(parent),
    ui(new Ui::ConnectWindow)
{
    ui->setupUi(this);
    
    Model m;
    ClientController controller( &m, ip, port );
    //controller.processActions( );
    
    GameWindow g(this, &controller, &m);
    g.setModal(true);
    g.exec();
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
