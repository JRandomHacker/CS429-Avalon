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
    this->setFixedSize(frameSize());
    
    Model m;
    ClientController controller( &m, ip, port );
    
    GameWindow g(this, &controller, &m);
    g.exec();
    
    close();
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
