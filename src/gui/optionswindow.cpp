#include "globals.hpp"
#include "clientController.hpp"
#include "optionswindow.h"
#include "connectwindow.h"
#include "gamewindow.h"
#include "joinserverwindow.h"
#include "createserverwindow.h"
#include "ui_optionswindow.h"
#include <unistd.h>
#include <iostream>
#include <string>

#ifdef _WIN32
	#include <windows.h>
#endif

/*
 *  Possible args:
 *  --players=x (This one is REQUIRED)
 *  --port=x
 *  --merlin
 *  --percival
 *  --morgana
 *  --mordred
 *  --oberon
 *  --assassin
 */
#define MAX_ARGS 8

OptionsWindow::OptionsWindow( QWidget *parent ) :
    QMainWindow( parent ),
    ui( new Ui::OptionsWindow ) {

    ui->setupUi(this);
}

OptionsWindow::~OptionsWindow( ) {
    delete ui;
}


void OptionsWindow::on_buttonJoinMenu_clicked()
{
    JoinServerWindow w;
    w.setModal(true);
    w.exec();
}

void OptionsWindow::on_buttonQuit_clicked()
{
    this->close();
}

void OptionsWindow::on_buttonCreateMenu_clicked()
{
    CreateServerWindow w;
    w.setModal(true);
    w.exec();
}
