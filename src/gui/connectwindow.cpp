#include "connectwindow.h"
#include "gamewindow.h"
#include "ui_connectwindow.h"

ConnectWindow::ConnectWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectWindow)
{
    ui->setupUi(this);
}

ConnectWindow::~ConnectWindow()
{
    delete ui;
}

void ConnectWindow::on_pushButton_clicked()
{
    GameWindow g;
    g.setModal(true);
    g.exec();
}
