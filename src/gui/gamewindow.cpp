#include "gamewindow.hpp"
#include "ui_gamewindow.h"
#include "subscriber.hpp"
#include "player.hpp"
#include "globals.hpp"
#include "guihelpers.hpp"
#include "clientCustomActionsFromGUI.hpp"
#include "voteHistory.hpp"
#include "resultsdialog.hpp"
#include <climits>
#include <signal.h>
#include <QStandardItem>
#include <QCloseEvent>
#include <QStringList>
#include <QKeyEvent>
#include <QHeaderView>
#include <QModelIndex>
#include <QList>
#include <vector>


GameWindow::GameWindow( QWidget *parent, ClientController* controller, Model * model, handle_t serverHandle ) :
    QWidget( parent ),
    ui( new Ui::GameWindow ) {
    this->control = controller;
    this->model = model;
    ui->setupUi( this );
    this->serverH = serverHandle;
    sync_sem = new sem_t;
    sem_init( sync_sem, 0, 0 );

    startWatchOnHasGameSettings( );

    // Connect slots and signals for updating UI elements
    connect( this, SIGNAL( gameSettingsReceived( ) ), this, SLOT( createPlayerSubscribersSlot( ) ) );
    connect( this, SIGNAL( playerInfoUpdated( unsigned int ) ), this, SLOT( updatePlayerSlot( unsigned int ) ) );
    connect( this, SIGNAL( gameInfoUpdated( ) ), this, SLOT( updateGameInfoSlot( ) ) );
    connect( this, SIGNAL( leaderIDUpdated( ) ), this, SLOT( updateLeaderSlot( ) ) );
    connect( this, SIGNAL( questingTeamUpdated( ) ), this, SLOT( updateQuestingTeamSlot( ) ) );
    connect( this, SIGNAL( trackUpdated( ) ), this, SLOT( updateTrackSlot( ) ) );
    connect( this, SIGNAL( voteStateUpdated( ) ), this, SLOT( updateVoteStateSlot( ) ) );
    connect( this, SIGNAL( voteHistoryUpdated( ) ), this, SLOT( updateVoteHistorySlot( ) ) );
    connect( this, SIGNAL( currentVotesUpdated( ) ), this, SLOT( updateCurrentVotesSlot( ) ) );

    // Start up thread for controller
    pthread_t controlThread;
    if( pthread_create( &controlThread, NULL, &controlThreadFn, this->control ) != 0 ) {
        std::cerr << "Unable to start controller thread" << std::endl;
        exit( EXIT_THREAD_ERROR );
    }
}

GameWindow::~GameWindow( ) {
    delete control;
    delete model;
    delete num_players_subscriber;
    delete myID_subscriber;
    delete numEvil_subscriber;
    delete roleList_subscriber;
    delete leaderID_subscriber;
    delete questingTeam_subscriber;
    delete voteState_subscriber;
    for( std::vector<Subscriber*>::iterator i = player_subscribers.begin( ); i != player_subscribers.end( ); i++ )
        delete *i;

    delete ui;
}

void GameWindow::startWatchOnHasGameSettings( ) {
    model->subscribe( "hasGameSettings", new ClosureSubscriber(
        [&]( Subscriber* s ) {
            if ( s->getData<bool>( ) ) {
                // Signal the UI to run createPlayerSubscribers( )
                emit gameSettingsReceived( );
                sem_wait( sync_sem );
            }
        },
        NULL ) );
}

void GameWindow::createPlayerSubscribersSlot( ) {
    createPlayerSubscribers( );
    sem_post( sync_sem );
}

void GameWindow::createPlayerSubscribers( ) {

    ui->votingSection->hide( );

    // Add subscriber for number of players
    num_players_subscriber = new ClosureSubscriber( NULL, NULL );
    model->subscribe( "numberOfPlayers", num_players_subscriber );
    unsigned int num_players = *num_players_subscriber->getData<unsigned int>( );

    // Add subscriber for numEvil
    numEvil_subscriber = new ClosureSubscriber(
                [&]( Subscriber* ) {
                emit gameInfoUpdated( );
                sem_wait( sync_sem );
            },
            NULL );
    model->subscribe( "numEvilChars", numEvil_subscriber );
    
    // Add subscriber for role list
    roleList_subscriber = new ClosureSubscriber(
                [&]( Subscriber* ) {
                emit gameInfoUpdated( );
                sem_wait( sync_sem );
            }
            , NULL );
    model->subscribe( "roleList", roleList_subscriber );

    // Update numEvil and roleList
    updateGameInfo( );
    
    // Subscribe to myID
    myID_subscriber = new ClosureSubscriber( NULL, NULL );
    model->subscribe( "myID", myID_subscriber );
    
    // Subscribe to questingTeam
    questingTeam_subscriber = new ClosureSubscriber(
                [&]( Subscriber* ) {
                    emit questingTeamUpdated( );
                    sem_wait( sync_sem );
            },
            NULL );
    model->subscribe( "questingTeam", questingTeam_subscriber );

    // Subscribe to quest and vote tracks
    voteTrackLength_subscriber = new ClosureSubscriber( NULL, NULL );
    currentVoteTrack_subscriber = new ClosureSubscriber(
                [&]( Subscriber* ) {
                    emit trackUpdated( );
                    sem_wait( sync_sem );
            },
            NULL );
    questTrackLength_subscriber = new ClosureSubscriber( NULL, NULL );
    model->subscribe( "voteTrackLength", voteTrackLength_subscriber );
    model->subscribe( "currentVoteTrack", currentVoteTrack_subscriber );
    model->subscribe( "questTrackLength", questTrackLength_subscriber );
    QStandardItemModel* trackModel = new QStandardItemModel( 2, 1 );
    ui->voteTrackList->setModel( trackModel );
    updateTrack( );

    // Add list items and subscribers for each player
    QStandardItemModel* listModel = new QStandardItemModel( num_players, 3 );
    ui->playerList->setModel( listModel );

    QStringList headers;
    headers.append( QString( "Name" ) );
    headers.append( QString( "Alignment" ) );
    headers.append( QString( "Role" ) );
    listModel->setHorizontalHeaderLabels( headers );

    for ( unsigned int i = 0; i < num_players; i++ ) {
        // Add subscriber for this player
        player_subscribers.push_back( new ClosureSubscriber(
            [&,i]( Subscriber* ){
                emit playerInfoUpdated( i );
                sem_wait( sync_sem );
            },
            NULL ) );
        model->subscribe( std::string( "player" ) + std::to_string( i ), player_subscribers[i] );

        updatePlayer( i );
    }

    // Subscribe to leaderID
    leaderID_subscriber = new ClosureSubscriber(
                [&]( Subscriber* ) {
                    emit leaderIDUpdated( );
                    sem_wait( sync_sem );
            },
            NULL );
    model->subscribe( "leaderID", leaderID_subscriber );
    updateLeader( );

    // Subscribe to voteState
    voteState_subscriber = new ClosureSubscriber(
                [&]( Subscriber* ) {
                    emit voteStateUpdated( );
                    sem_wait( sync_sem );
            },
            NULL );
    model->subscribe( "teamVoteState", voteState_subscriber );

    voteHistory_subscriber = new ClosureSubscriber(
                [&]( Subscriber* ) {
                    emit voteHistoryUpdated( );
                    sem_wait( sync_sem );
                },
                NULL );
    model->subscribe( "voteHistory", voteHistory_subscriber );
}

void GameWindow::updatePlayerSlot( unsigned int id ) {
    updatePlayer( id );
    sem_post( sync_sem );
}

void GameWindow::updatePlayer( unsigned int id ) {

    QStandardItemModel* listModel = ( QStandardItemModel* ) ui->playerList->model( );

    Player* p = player_subscribers[id]->getData<Player>( );

    // Update the player's info in GUI
    std::string newString = "<waiting for player>";
    if( p != NULL ) {
        newString = p->getName( );
        if( id == *myID_subscriber->getData<unsigned int>( ) )
            newString += " (me)";

        if( id == 0 )
            newString += " (host)";

        listModel->setItem( id, 1, new QStandardItem(
                                QString( avalon::gui::alignmentToString( p->getAlignment( ) ).c_str( ) ) ) );
        listModel->setItem( id, 2, new QStandardItem(
                                QString( avalon::gui::roleToString( p->getRole( ) ).c_str( ) ) ) );
    }
    listModel->setItem( id, 0, new QStandardItem( QString( newString.c_str( ) ) ) );
    
    ui->playerList->resizeColumnsToContents( );
}

void GameWindow::updateLeaderSlot( ) {
    updateLeader( );
    sem_post( sync_sem );
}

void GameWindow::updateLeader( ) {

    unsigned int lid = *leaderID_subscriber->getData<unsigned int>( );
    unsigned int myID = *myID_subscriber->getData<unsigned int>( );

    if( lid == myID ) {

        ui->leaderLabel->setText( QString( "You are the leader!" ) );
        ui->proposeTeamButton->show( );
    } else if( lid != UINT_MAX ) {

        Player* leader = player_subscribers[lid]->getData<Player>( );
        if( leader != NULL ) {
            ui->leaderLabel->setText( QString( ( "Current Leader: "+leader->getName( ) ).c_str( ) ) );
        }
        ui->proposeTeamButton->hide( );
    } else {

        ui->leaderLabel->setText( QString( "" ) );
        ui->proposeTeamButton->hide( );
    }
}

void GameWindow::updateQuestingTeamSlot( ) {
    updateQuestingTeam( );
    sem_post( sync_sem );
}

void GameWindow::updateQuestingTeam( ) {
    std::vector<unsigned int> team = *questingTeam_subscriber->getData<std::vector<unsigned int>>( );

    QStandardItemModel* qModel = new QStandardItemModel( );
    for( unsigned int i = 0; i < team.size( ); i++ ) {

        unsigned int player_num = team[ i ];
        Player* p = player_subscribers[ player_num ]->getData< Player >( );
        qModel->appendRow( new QStandardItem( QString( p->getName( ).c_str( ) ) ) );
    }
    ui->proposeTeamList->setModel( qModel );
}

void GameWindow::updateGameInfoSlot( ) {
    updateGameInfo( );
    sem_post( sync_sem );
}

void GameWindow::updateGameInfo( ) {
    unsigned int num_evil = *numEvil_subscriber->getData<unsigned int>( );

    std::string num_evil_string = "Evil Players: " + std::to_string( num_evil );

    ui->numOfEvilPlayers->setText( QString( num_evil_string.c_str( ) ) );

    std::vector< avalon::special_roles_t >* roleList = roleList_subscriber->getData< std::vector< avalon::special_roles_t > >( );
    if( roleList != NULL ) {
        QString roles = QString( "" );
        for( unsigned int i = 0; i < roleList->size( ); i++ ) {
            avalon::special_roles_t role = ( *roleList )[ i ];
            if( i != 0 ) {
                roles.append( ", " );
            }
            roles.append( avalon::gui::roleToString( role ).c_str( ) );
        }
        ui->currRolesList->setText( roles );
    }
}

void GameWindow::updateTrackSlot( ) {
    updateTrack( );
    sem_post( sync_sem );
}

void GameWindow::updateTrack( ) {

    unsigned int qLength = *questTrackLength_subscriber->getData<unsigned int>( );
    unsigned int vLength = *voteTrackLength_subscriber->getData<unsigned int>( );
    unsigned int currVote = *currentVoteTrack_subscriber->getData<unsigned int>( );

    std::string questStr = "Quest Track Length: " + std::to_string( qLength );
    std::string voteStr = "Vote " + std::to_string( currVote + 1 ) + "/" + std::to_string( vLength );

    QStandardItemModel* trackModel = ( QStandardItemModel* ) ui->voteTrackList->model( );
    QStandardItem* questItem = new QStandardItem( QString( questStr.c_str( ) ) );
    QStandardItem* voteItem = new QStandardItem( QString( voteStr.c_str( ) ) );
    trackModel->setItem( 0, questItem );
    trackModel->setItem( 1, voteItem );
}


void GameWindow::updateVoteStateSlot( ) {
    updateVoteState( );
    sem_post( sync_sem );
}

void GameWindow::updateVoteState( ) {

    bool inVoteState = *voteState_subscriber->getData<bool>( );
    QStandardItemModel* listModel = (QStandardItemModel*) ui->playerList->model( );
    if( inVoteState ) {

        // Hide the propose team button
        ui->proposeTeamButton->hide( );

        // Create the "Has Voted" column
        for( int i = 0; i < listModel->rowCount( ); i++ ) {
            listModel->setItem( i, 3, new QStandardItem( "No" ) );
        }
        listModel->setHorizontalHeaderItem( 3, new QStandardItem( QString( "Has Voted" ) ) );
        ui->votingSection->show( );

        // Subscribe to the data
        currentVotes_subscriber = new ClosureSubscriber(
                    [&]( Subscriber* ) {
                        emit currentVotesUpdated( );
                        sem_wait( sync_sem );
                    },
                    NULL);
        model->subscribe( "currentVotes", currentVotes_subscriber );
    } else {
        listModel->removeColumn( 3 );
        ui->votingSection->hide( );
    }
}

void GameWindow::updateCurrentVotesSlot( ) {
    updateCurrentVotes( );
    sem_post( sync_sem );
}

void GameWindow::updateCurrentVotes( ) {

    std::vector<avalon::player_vote_t> votes = *currentVotes_subscriber->getData<std::vector<avalon::player_vote_t>>( );
    for( unsigned int i = 0; i < votes.size( ); i++ ) {
        if( votes[i] != avalon::NO_VOTE ) {
            QStandardItemModel* listModel = ( QStandardItemModel* ) ui->playerList->model( );
            listModel->item( i, 3 )->setText( QString( "Yes" ) );
        }
    }
}

void GameWindow::updateVoteHistorySlot( ) {
    updateVoteHistory( );
    sem_post( sync_sem );
}

void GameWindow::updateVoteHistory( ) {
    std::vector<VoteHistory> hist = *voteHistory_subscriber->getData<std::vector<VoteHistory>>( );

    VoteHistory thisVote = hist.back( );

    ResultsDialog results( NULL, thisVote, player_subscribers );
    results.exec( );
}


void GameWindow::on_playerList_clicked( const QModelIndex& index ) {
    if(leaderID_subscriber == NULL || myID_subscriber == NULL )
        return;

    unsigned int row = (unsigned int) index.row( );

    std::vector<unsigned int> qTeam = *questingTeam_subscriber->getData<std::vector<unsigned int>>( );
    for( unsigned int i = 0; i < qTeam.size( ); i++ ) {
        if( qTeam[i] == row ) {
            SelectQuestGoerAction* act = new SelectQuestGoerAction( false, row );
            control->addActionToQueue( act );
            return;
        }
    }

    SelectQuestGoerAction* act = new SelectQuestGoerAction( true, row );
    control->addActionToQueue( act );
}

void GameWindow::on_buttonVotePass_clicked( ) {
    TeamVoteAction* vote = new TeamVoteAction( avalon::YES );
    control->addActionToQueue( vote );
}

void GameWindow::on_buttonVoteFail_clicked( ) {
    TeamVoteAction* vote = new TeamVoteAction( avalon::NO );
    control->addActionToQueue( vote );
}

void GameWindow::on_proposeTeamButton_clicked( ) {
    FinalizeTeamAction* act = new FinalizeTeamAction( );
    control->addActionToQueue( act );
}

void GameWindow::closeEvent( QCloseEvent* e ) {
    if( serverH != 0 ) {
        #ifdef _WIN32
            TerminateProcess( serverH, 0 );
        #else
            kill( serverH, SIGKILL );
        #endif
    }
    
    e->accept( );
}

// Overrides the keyPressEvent of QWidget to make the escape key
// also clean up the server by emitting a close event
// By default, the ESC key simply accept( )'s, which doesn't call closeEvent
void GameWindow::keyPressEvent( QKeyEvent* event ) {

    if( Qt::Key_Escape == event->key( ) ) {
        QWidget::close( );
    }
}

void* GameWindow::controlThreadFn( void* clientController ) {
    ( ( ClientController* ) clientController )->processActions( );
    return NULL;
}
