#include "optionswindow.h"
#include "ui_optionswindow.h"
#include <unistd.h>
#include <stdio.h>

OptionsWindow::OptionsWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::OptionsWindow)
{
    ui->setupUi(this);
    
    connect(ui->serverSectionCreateButton, SIGNAL(released()), this, SLOT(createServer()));
}

OptionsWindow::~OptionsWindow()
{
    delete ui;
}


OptionsWindow::createServer()
{
    fprintf(stdout, "Fork!  If I could figure out how...");
    /*int child = fork();
    if(child == 0)
    {
        // child
        exec("./server.exe --players " + ui->serverSectionPlayerAmount->value());
    }
    else
    {
        cout << "Server Created.";
    }*/
  }