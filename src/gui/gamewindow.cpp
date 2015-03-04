#include "gamewindow.h"
#include "ui_gamewindow.h"

GameWindow::GameWindow(QWidget *parent, ClientController* controller, Model * model) :
    QDialog(parent),
    ui(new Ui::GameWindow)
{
    this->control = controller;
    this->model = model;
    ui->setupUi(this);
    
    control->processActions();
}

GameWindow::~GameWindow()
{
    delete ui;
}
