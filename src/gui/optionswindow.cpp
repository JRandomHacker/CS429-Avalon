#include "globals.hpp"
#include "clientController.hpp"
#include "optionswindow.hpp"
#include "joinserverwindow.hpp"
#include "createserverwindow.hpp"
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

    ui->setupUi( this );
    
    joinWindow = NULL;
    createWindow = NULL;
}

OptionsWindow::~OptionsWindow( ) {
    delete ui;
    delete joinWindow;
    delete createWindow;
}


void OptionsWindow::on_buttonJoinMenu_clicked( ) {
    if( joinWindow == NULL ) {
        joinWindow = new JoinServerWindow( this );
        joinWindow->setModal( false );
    }
    joinWindow->show( );
    this->hide( );
}

void OptionsWindow::on_buttonQuit_clicked( ) {
    this->close( );
}

void OptionsWindow::on_buttonCreateMenu_clicked( ) {
    if( createWindow == NULL ) {
        createWindow = new CreateServerWindow( this );
        createWindow->setModal( false );
    }
    createWindow->show( );
    this->hide( );
}
