#include "gamewindow.hpp"
#include "ui_gamewindow.h"
#include "subscriber.hpp"
#include "player.hpp"
#include <signal.h>
#include <QStandardItem>
#include <QCloseEvent>
#include <thread>
#include <chrono>
#include <vector>

using namespace std;

#ifdef _WIN32
	GameWindow::GameWindow( QWidget *parent, ClientController* controller, Model * model, HANDLE serverHandle ) :
#else     
	GameWindow::GameWindow( QWidget *parent, ClientController* controller, Model * model, int serverHandle ) :
#endif
    QDialog( parent ),
    ui( new Ui::GameWindow ) {
    this->control = controller;
    this->model = model;
    ui->setupUi( this );
    serverH = serverHandle;

    startWatchOnHasGameSettings( );

    connect(this, SIGNAL(gameSettingsReceived()), this, SLOT(createPlayerSubscribers()));
    connect(this, SIGNAL(playerInfoUpdated(int)), this, SLOT(updatePlayer(int)));

    // Start up thread for controller
    pthread_t controlThread;
    if( pthread_create( &controlThread, NULL, &controlThreadFn, this->control ) != 0 ) {
        cerr << "Unable to start controller thread" << endl;
        exit( EXIT_THREAD_ERROR );
    }
}

GameWindow::~GameWindow( ) {
    delete ui;
}

void GameWindow::startWatchOnHasGameSettings( ) {
    model->subscribe("hasGameSettings", new ClosureSubscriber(
        [&]( Subscriber* s ) {
            if ( s->getData<bool>( ) ) {
                emit gameSettingsReceived();
            }
        },
        [&]( Subscriber* ){ } ) );
}

void GameWindow::createPlayerSubscribers( ) {

    // Get number of players
    num_players_subscriber = new ClosureSubscriber(
        [&](Subscriber*){ },
        [&](Subscriber*){ } );
    model->subscribe( "numberOfPlayers", num_players_subscriber );
    unsigned int num_players = *num_players_subscriber->getData<unsigned int>( );

    // Get number of evil
    numEvil_subscriber = new ClosureSubscriber(NULL, NULL);
    model->subscribe( "numEvilChars", numEvil_subscriber );
    unsigned int* num_evil = numEvil_subscriber->getData<unsigned int>();
    string num_evil_string = "Evil Players: ";
    if(num_evil != NULL)
        num_evil_string += to_string(*num_evil);
    else
        num_evil_string += "unknown number";
    ui->numOfEvilPlayers->setText(QString(num_evil_string.c_str()));
    
    // Get list of roles
    roleList_subscriber = new ClosureSubscriber(NULL, NULL);
    model->subscribe( "isRoleInGame", roleList_subscriber );
    vector<bool>* roleList = roleList_subscriber->getData<vector<bool>>();
    if(roleList != NULL) 
    {/* TODO: Add roles to list */}
    
    // Get my ID
    myID_subscriber = new ClosureSubscriber( NULL, NULL );
    model->subscribe( "myID", myID_subscriber );
    int myID = *myID_subscriber->getData<int>( );

    //Add list items for each player
    QStandardItemModel* listModel = new QStandardItemModel( );
    ui->playerList->setModel( listModel );

    for ( unsigned int i = 0; i < num_players; i++ ) {
        // Add subscriber for this player
        player_subscribers.push_back( new ClosureSubscriber(
            [&,i,listModel]( Subscriber* ){
                emit playerInfoUpdated(i);
            },
            [&]( Subscriber* ){ } ) );
        model->subscribe( string( "player" )+to_string( i ), player_subscribers[i] );

        // Check whether this player is already connected
        Player** p = player_subscribers[i]->getData<Player*>( );

        string player_string = "";
        if( p != NULL )
            player_string += (*p)->getName( );
        else
            player_string += "p" + to_string( i );

        if( ( int ) i == myID )
            player_string += " (me)";

        if(i == 0)
            player_string += " (host)";
        else if(p == NULL)
            player_string += " (not connected)";
        else
            player_string += " (connected)";

        QStandardItem* pli = new QStandardItem( );
        pli->setText( QString( player_string.c_str( ) ) );
        listModel->appendRow( pli );
    }
}

void GameWindow::updatePlayer(int id)
{
    QStandardItemModel* listModel = (QStandardItemModel*) ui->playerList->model();

    Player* p = *(player_subscribers[id]->getData<Player*>( ) );

    // Update the player's info in GUI
    string newString = p->getName( );
    if( ( int ) id == *myID_subscriber->getData<int>( ) ) {
        newString += " (me)";
    }
    if(id == 0)
        newString += " (host)";
    else
        newString += " (connected)";
    listModel->item( id )->setText( QString( newString.c_str( ) ) );

}

void* GameWindow::controlThreadFn( void* clientController ) {
    ( ( ClientController* ) clientController )->processActions( );
    return NULL;
}

void GameWindow::closeEvent(QCloseEvent* e) {
    if(serverH != 0)
    {
        #ifdef _WIN32
            TerminateProcess(serverH, 0);
        #else
            kill(serverH, SIGKILL);
        #endif
    }
    
    e->accept();
}
