#include "gamewindow.hpp"
#include "ui_gamewindow.h"
#include "subscriber.hpp"
#include "player.hpp"
#include "globals.hpp"
#include "clientCustomActionsFromGUI.hpp"
#include <signal.h>
#include <QStandardItem>
#include <QCloseEvent>
#include <QKeyEvent>
#include <vector>

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
    this->serverH = serverHandle;

    startWatchOnHasGameSettings( );

    // Connect slots and signals for updating UI elements
    connect(this, SIGNAL(gameSettingsReceived()), this, SLOT(createPlayerSubscribers()));
    connect(this, SIGNAL(playerInfoUpdated(unsigned int)), this, SLOT(updatePlayer(unsigned int)));
    connect(this, SIGNAL(gameInfoUpdated()), this, SLOT(updateGameInfo()));

    // Start up thread for controller
    pthread_t controlThread;
    if( pthread_create( &controlThread, NULL, &controlThreadFn, this->control ) != 0 ) {
        std::cerr << "Unable to start controller thread" << std::endl;
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
                // Signal the UI to run createPlayerSubscribers()
                emit gameSettingsReceived();
            }
        },
        NULL ) );
}

void GameWindow::createPlayerSubscribers( ) {

    // Add subscriber for number of players
    num_players_subscriber = new ClosureSubscriber(NULL, NULL);
    model->subscribe( "numberOfPlayers", num_players_subscriber );
    unsigned int num_players = *num_players_subscriber->getData<unsigned int>( );

    // Add subscriber for numEvil
    numEvil_subscriber = new ClosureSubscriber(
                [&]( Subscriber* ) {
                emit gameInfoUpdated();
            },
            NULL);
    model->subscribe( "numEvilChars", numEvil_subscriber );
    
    // Add subscriber for role list
    roleList_subscriber = new ClosureSubscriber(
                [&]( Subscriber* ) {
                emit gameInfoUpdated();
            }
            , NULL);
    model->subscribe( "isRoleInGame", roleList_subscriber );

    // Update numEvil and roleList
    updateGameInfo();
    
    // Subscribe to myID
    myID_subscriber = new ClosureSubscriber( NULL, NULL );
    model->subscribe( "myID", myID_subscriber );

    // Add list items and subscribers for each player
    QStandardItemModel* listModel = new QStandardItemModel( );
    ui->playerList->setModel( listModel );

    for ( unsigned int i = 0; i < num_players; i++ ) {
        // Add subscriber for this player
        player_subscribers.push_back( new ClosureSubscriber(
            [&,i]( Subscriber* ){
                emit playerInfoUpdated(i);
            },
            NULL) );
        model->subscribe( std::string("player") + std::to_string( i ), player_subscribers[i] );

        QStandardItem* pli = new QStandardItem( );
        listModel->appendRow( pli );

        updatePlayer(i);
    }
}

void GameWindow::updatePlayer(unsigned int id) {

    QStandardItemModel* listModel = (QStandardItemModel*) ui->playerList->model();

    Player** p = player_subscribers[id]->getData<Player*>( );

    // Update the player's info in GUI
    std::string newString = "<waiting for player>";
    if(p != NULL)
    {
        newString = (*p)->getName( );
        if( (int) id == *myID_subscriber->getData<int>( ) )
            newString += " (me)";

        if(id == 0)
            newString += " (host)";
        else
            newString += " (connected)";
    }
    listModel->item( id )->setText( QString( newString.c_str( ) ) );
}

void GameWindow::updateGameInfo() {
    unsigned int* num_evil = numEvil_subscriber->getData<unsigned int>();

    std::string num_evil_string = "Evil Players: ";
    if(num_evil != NULL)
        num_evil_string += std::to_string(*num_evil);
    else
        num_evil_string += "unknown number";

    ui->numOfEvilPlayers->setText(QString(num_evil_string.c_str()));

    std::vector<bool>* roleList = roleList_subscriber->getData<std::vector<bool>>();
    if(roleList != NULL)
    { /* TODO: translate list of bool to list of roles in game */ }
}

void GameWindow::on_buttonVotePass_clicked() {
    TeamVoteAction* vote = new TeamVoteAction(avalon::YES);
    control->addActionToQueue(vote);
}

void GameWindow::on_buttonVoteFail_clicked() {
    TeamVoteAction* vote = new TeamVoteAction(avalon::NO);
    control->addActionToQueue(vote);
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

// Overrides the keyPressEvent of QWidget to make the escape key
// also clean up the server by emitting a close event
// By default, the ESC key simply accept()'s, which doesn't call closeEvent
void GameWindow::keyPressEvent( QKeyEvent* event ) {

    if( Qt::Key_Escape == event->key( ) ) {
        QWidget::close( );
    }
}

void* GameWindow::controlThreadFn( void* clientController ) {
    ( ( ClientController* ) clientController )->processActions( );
    return NULL;
}
