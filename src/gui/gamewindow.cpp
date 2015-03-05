#include "gamewindow.h"
#include "ui_gamewindow.h"

GameWindow::GameWindow(QWidget *parent, ClientController* controller, Model * model) :
    QDialog(parent),
    ui(new Ui::GameWindow)
{
    this->control = controller;
    this->model = model;
    ui->setupUi(this);
    
	pthread_t controlThread;    
    if( pthread_create( &controlThread, NULL, &controlThreadFn, this->control ) != 0 )
    {
        std::cerr << "Unable to start controller thread" << std::endl;
        exit( EXIT_THREAD_ERROR );
    }	
}

GameWindow::~GameWindow()
{
    delete ui;
}

void* GameWindow::controlThreadFn(void* data)
{   
    ((ClientController*) data)->processActions();
    return NULL;
}