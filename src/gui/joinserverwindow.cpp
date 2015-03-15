#include "joinserverwindow.hpp"
#include "optionswindow.hpp"
#include "gamewindow.hpp"
#include "ui_joinserverwindow.h"
#include "displayerrors.hpp"
#include "globals.hpp"
#include "clientCustomActionsFromGUI.hpp"

JoinServerWindow::JoinServerWindow( QWidget *parent ) :
    QDialog( parent ),
    ui( new Ui::JoinServerWindow ) {
    ui->setupUi( this );

    ui->fieldPortNum->insert( QString( std::to_string( DEFAULT_PORT ).c_str( ) ) );
    ui->fieldServerAddr->insert( QString( "localhost" ) );

    model = new Model( );
    controller = new ClientController( model );
}

JoinServerWindow::~JoinServerWindow( ) {
    delete controller;
    delete model;
    delete ui;
}

void JoinServerWindow::on_buttonCancel_clicked( ) {
    this->parentWidget()->show();
    
    this->hide();
}

void JoinServerWindow::on_buttonJoinServer_clicked( ) {
    std::string addr = ui->fieldServerAddr->text().toStdString( );
    int port = ui->fieldPortNum->text().toInt( );

    int status = controller->spawnNetwork( addr, port );
    if( status != EXIT_SUCCESS ) {
        displayError( status, this );
    } else {

        SetNameAction* setName = new SetNameAction( ui->fieldPlayerName->text( ).toStdString( ) );
        controller->addActionToQueue( ( Action* )setName );
        GameWindow* g = new GameWindow( NULL, controller, model );
        g->setModal( false );
        g->show( );

        this->hide( );
    }
}

