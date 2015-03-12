#include "joinserverwindow.hpp"
#include "optionswindow.hpp"
#include "gamewindow.hpp"
#include "ui_joinserverwindow.h"
#include "displayerrors.hpp"
#include "globals.hpp"

JoinServerWindow::JoinServerWindow( QWidget *parent ) :
    QDialog( parent ),
    ui( new Ui::JoinServerWindow ) {
    ui->setupUi( this );

    ui->fieldPortNum->insert( QString( std::to_string( DEFAULT_PORT ).c_str( ) ) );
    ui->fieldServerAddr->insert( QString( "localhost" ) );
}

JoinServerWindow::~JoinServerWindow( ) {
    delete ui;
}

void JoinServerWindow::on_buttonCancel_clicked( ) {
    OptionsWindow* o = new OptionsWindow();
    o->show();
    
    close();
}

void JoinServerWindow::on_buttonJoinServer_clicked( ) {
    std::string addr = ui->fieldServerAddr->text().toStdString( );
    int port = ui->fieldPortNum->text().toInt( );

    Model* m = new Model( );
    ClientController* controller = new ClientController( m );
    int status = controller->spawnNetwork( addr, port );
    if( status != EXIT_SUCCESS ) {
        displayError( status, this );
    } else {

        GameWindow* g = new GameWindow( NULL, controller, m );
        g->setModal( false );
        g->show( );

        close( );
    }
}

