#include "gamewindow.hpp"
#include "ui_gamewindow.h"
#include "subscriber.hpp"
#include "player.hpp"
#include "globals.hpp"
#include "guihelpers.hpp"
#include "clientCustomActionsFromGUI.hpp"
#include "voteHistory.hpp"
#include "questVoteHistory.hpp"
#include "resultsdialog.hpp"
#include "chat_message.hpp"
#include "enterdetector.hpp"
#include "clientCustomActionsForChat.hpp"
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
    connect( this, SIGNAL( playerInfoUpdated( ) ), this, SLOT( updatePlayerSlot( ) ) );
    connect( this, SIGNAL( gameInfoUpdated( ) ), this, SLOT( updateGameInfoSlot( ) ) );
    connect( this, SIGNAL( leaderIDUpdated( ) ), this, SLOT( updateLeaderSlot( ) ) );
    connect( this, SIGNAL( questingTeamUpdated( ) ), this, SLOT( updateQuestingTeamSlot( ) ) );
    connect( this, SIGNAL( trackUpdated( ) ), this, SLOT( updateTrackSlot( ) ) );
    connect( this, SIGNAL( teamVoteStateUpdated( ) ), this, SLOT( updateTeamVoteStateSlot( ) ) );
    connect( this, SIGNAL( questVoteStateUpdated( ) ), this, SLOT( updateQuestVoteStateSlot( ) ) );
    connect( this, SIGNAL( voteHistoryUpdated( ) ), this, SLOT( updateVoteHistorySlot( ) ) );
    connect( this, SIGNAL( questHistoryUpdated( ) ), this, SLOT( updateQuestHistorySlot( ) ) );
    connect( this, SIGNAL( currentVotesUpdated( ) ), this, SLOT( updateCurrentVotesSlot( ) ) );
    connect( this, SIGNAL( chatMessagesUpdated( ) ), this, SLOT( updateChatMessagesSlot( ) ) );
    connect( this, SIGNAL( assassinStateUpdated( ) ), this, SLOT( updateAssassinStateSlot( ) ) );
    connect( this, SIGNAL( endGameStateUpdated( ) ), this, SLOT( updateEndGameStateSlot( ) ) );

    ui->chatEdit->installEventFilter( new EnterDetector( this, ui->sendMsgButton ) );

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
    delete teamVoteState_subscriber;
    delete voteTrackLength_subscriber;
    delete currentVoteTrack_subscriber;
    delete questTrackLength_subscriber;
    delete currentQuestTrack_subscriber;
    delete playersPerQuest_subscriber;
    delete questVoteState_subscriber;
    delete assassinState_subscriber;
    delete endGameState_subscriber;
    delete voteHistory_subscriber;
    delete questHistory_subscriber;
    delete currentVotes_subscriber;
    delete chatMessages_subscriber;
    delete player_subscriber;
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
    ui->stateLabel->setText( QString( "Team Select" ) );

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
    currentQuestTrack_subscriber = currentVoteTrack_subscriber;
    playersPerQuest_subscriber = new ClosureSubscriber( NULL, NULL );
    questTrackLength_subscriber = new ClosureSubscriber( NULL, NULL );
    model->subscribe( "voteTrackLength", voteTrackLength_subscriber );
    model->subscribe( "currentVoteTrack", currentVoteTrack_subscriber );
    model->subscribe( "questTrackLength", questTrackLength_subscriber );
    model->subscribe( "currentQuestTrack", currentQuestTrack_subscriber );
    model->subscribe( "playersPerQuest", playersPerQuest_subscriber );

    unsigned int qLength = *questTrackLength_subscriber->getData<unsigned int>( );

    voteTrackLabels = new QLabel*[qLength]();

    //Instantiate quest tracker GUI
    for(unsigned int i = 0; i < qLength; i++) {

        voteTrackLabels[i] = new QLabel("");
        voteTrackLabels[i]->setPixmap( QPixmap( ":/images/QUEST_UNKNOWN.png" ) );
        voteTrackLabels[i]->setToolTip( QString("No quest history to display.") );

        ui->votingTrackLayout->addWidget(voteTrackLabels[i]);
    }

    updateTrack( );

    // Add list items and subscribers for each player
    QStandardItemModel* listModel = new QStandardItemModel( num_players, 3 );
    ui->playerList->setModel( listModel );

    QStringList headers;
    headers.append( QString( "Name" ) );
    headers.append( QString( "Alignment" ) );
    headers.append( QString( "Role" ) );

    listModel->setHorizontalHeaderLabels( headers );

    player_subscriber = new ClosureSubscriber(
            [&]( Subscriber* ){
                emit playerInfoUpdated( );
                sem_wait( sync_sem );
            },
            NULL );
    model->subscribe( "players", player_subscriber );
    updatePlayer( );
    
    ui->playerList->hide();
    ui->playerList->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->playerList->show();

    // Subscribe to leaderID
    leaderID_subscriber = new ClosureSubscriber(
                [&]( Subscriber* ) {
                    emit leaderIDUpdated( );
                    sem_wait( sync_sem );
            },
            NULL );
    model->subscribe( "leaderID", leaderID_subscriber );
    updateLeader( );

    // Subscribe to state flags
    teamVoteState_subscriber = new ClosureSubscriber(
                [&]( Subscriber* ) {
                    emit teamVoteStateUpdated( );
                    sem_wait( sync_sem );
            },
            NULL );
    model->subscribe( "teamVoteState", teamVoteState_subscriber );

    questVoteState_subscriber = new ClosureSubscriber(
                [&]( Subscriber* ) {
                    emit questVoteStateUpdated( );
                    sem_wait( sync_sem );
            },
            NULL );
    model->subscribe( "questVoteState", questVoteState_subscriber );

    assassinState_subscriber = new ClosureSubscriber(
                [&]( Subscriber* ) {
                    emit assassinStateUpdated( );
                    sem_wait( sync_sem );
                },
                NULL );
    model->subscribe( "assassinState", assassinState_subscriber );

    endGameState_subscriber = new ClosureSubscriber(
                [&]( Subscriber* ) {
                    emit endGameStateUpdated( );
                    sem_wait( sync_sem );
                },
                NULL );
    model->subscribe( "endGameState", endGameState_subscriber );

    // Subscribe to histories
    voteHistory_subscriber = new ClosureSubscriber(
                [&]( Subscriber* ) {
                    emit voteHistoryUpdated( );
                    sem_wait( sync_sem );
                },
                NULL );
    model->subscribe( "voteHistory", voteHistory_subscriber );

    questHistory_subscriber = new ClosureSubscriber(
                [&]( Subscriber* ) {
                    emit questHistoryUpdated( );
                    sem_wait( sync_sem );
                },
                NULL );
    model->subscribe( "questHistory", questHistory_subscriber );

    chatMessages_subscriber = new ClosureSubscriber(
                [&]( Subscriber* ) {
                    emit chatMessagesUpdated( );
                    sem_wait( sync_sem );
                },
                NULL );
    model->subscribe( "chatMessages", chatMessages_subscriber );
    QStandardItemModel* chatModel = new QStandardItemModel( );
    ui->chatList->setModel( chatModel );
}

void GameWindow::updatePlayerSlot( ) {
    updatePlayer( );
    sem_post( sync_sem );
}

void GameWindow::updatePlayer( ) {

    QStandardItemModel* listModel = ( QStandardItemModel* ) ui->playerList->model( );

    
    std::vector <Player> player_vector = (*player_subscriber->getData<std::vector<Player> >( ) );

    for (unsigned int i = 0; i < player_vector.size(); i++ ) {
        // Update the player's info in GUI
        std::string newString = "<waiting for player>";
        Player p = player_vector[i];
        newString = p.getName( );
        if( i == *myID_subscriber->getData<unsigned int>( ) ) {
            newString += " (me)";

            //Load client player's role avatar
            ui->playerAvatarLabel->setVisible(false);
            ui->playerAvatarLabel->setPixmap( QPixmap( avalon::gui::roleToImage( p.getRole() ).c_str( ) ) );
            ui->playerAvatarLabel->setVisible(true);
        }

        if( i == 0 )
            newString += " (host)";

        listModel->setItem( i, 1, new QStandardItem(
                                QString( avalon::gui::alignmentToString( p.getAlignment( ) ).c_str( ) ) ) );
        listModel->setItem( i, 2, new QStandardItem(
                                QString( avalon::gui::roleToString( p.getRole( ) ).c_str( ) ) ) );   
    
        listModel->setItem( i, 0, new QStandardItem( QString( newString.c_str( ) ) ) );
    }
    
    
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
        ui->proposeTeamButton->setEnabled( false );
    } else if( lid != UINT_MAX ) {

        Player leader = (*player_subscriber->getData<std::vector<Player> >( ) )[lid];
        //if( leader != NULL ) {
            ui->leaderLabel->setText( QString( ( "Current Leader: "+leader.getName( ) ).c_str( ) ) );
        //}
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
        Player p = (*player_subscriber->getData<std::vector<Player> >( ))[player_num];
        qModel->appendRow( new QStandardItem( QString( p.getName( ).c_str( ) ) ) );

        QStringList qHeaders;
        qHeaders.append( QString( "Name" ) );
        qModel->setHorizontalHeaderLabels( qHeaders );
    }

    ui->proposeTeamList->setModel( qModel );

    ui->proposeTeamList->hide( );
    ui->proposeTeamList->horizontalHeader( )->setSectionResizeMode( QHeaderView::Stretch );
    ui->proposeTeamList->show( );

    // Enable or disable the propose button depending on how many questers are selected
    std::vector<unsigned int> playersPerQuest = *playersPerQuest_subscriber->getData<std::vector<unsigned int>>( );
    unsigned int currQuest = *currentQuestTrack_subscriber->getData<unsigned int>( );
    unsigned int playersCurrQuest = playersPerQuest[currQuest];
    ui->teamSizeLabel->setText( QString( (std::to_string( team.size( ) ) + "/" + std::to_string( playersCurrQuest )
                                         + " players selected").c_str( ) ) );

    ui->proposeTeamButton->setEnabled( playersCurrQuest == team.size( ) );
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
        QString roles = QString( "Roles: " );
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

std::string GameWindow::buildQuestHistoryString(VoteHistory vote_results) {

    std::string header1 = "---Questing Team---\n";
    std::string header2 = "---Player Votes---\n";

    for ( unsigned int i = 0; i < vote_results.getPlayerVotes( ).size( ); i++ ) {
        if ( vote_results.getPlayerVotes( )[i] == avalon::YES) {

            header2 += " voted to accept the team\n";

        } else if ( vote_results.getPlayerVotes( )[i] == avalon::NO) {

            header2 += " voted to reject the team\n";

        } else if ( vote_results.getPlayerVotes( )[i] == avalon::HIDDEN) {

            // Handle hidden votes here if need be

        } else {

        }
    }

    return header1 + header2;
}

void GameWindow::updateTrack( ) {
    unsigned int currQuest = *currentQuestTrack_subscriber->getData<unsigned int>( );
    std::vector<unsigned int> playersPerQuest = *playersPerQuest_subscriber->getData<std::vector<unsigned int>>( );

    if(currQuest > 0) {
        std::vector<QuestVoteHistory> hist = *questHistory_subscriber->getData<std::vector<QuestVoteHistory>>( );

        QuestVoteHistory temp = hist[currQuest-1];

        if( temp.getVotePassed() )
            voteTrackLabels[currQuest-1]->setPixmap( QPixmap( ":/images/QUEST_PASS.png" ) );
        else
            voteTrackLabels[currQuest-1]->setPixmap( QPixmap( ":/images/QUEST_FAIL.png" ) );

        std::vector<VoteHistory> hist2 = *voteHistory_subscriber->getData<std::vector<VoteHistory>>( );

        VoteHistory prevVote = hist2.back( );

        voteTrackLabels[currQuest-1]->setToolTip( QString(buildQuestHistoryString(prevVote).c_str()) );

    }
}

void GameWindow::updateTeamVoteStateSlot( ) {
    updateTeamVoteState( );
    sem_post( sync_sem );
}

void GameWindow::updateTeamVoteState( ) {

    bool inVoteState = *teamVoteState_subscriber->getData<bool>( );
    QStandardItemModel* listModel = (QStandardItemModel*) ui->playerList->model( );
    if( inVoteState ) {

        ui->stateLabel->setText( QString( "Team Vote" ) );

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

void GameWindow::updateQuestVoteStateSlot( ) {
    updateQuestVoteState( );
    sem_post( sync_sem );
}

void GameWindow::updateQuestVoteState( ) {
    // update UI for quest vote
    bool inQuestVote = *questVoteState_subscriber->getData<bool>( );
    QStandardItemModel* listModel = (QStandardItemModel*) ui->proposeTeamList->model( );
    if( inQuestVote ) {
        ui->stateLabel->setText( QString( "Quest Vote" ) );

        // Create the "Has Voted" column
        for( int i = 0; i < listModel->rowCount( ); i++ ) {
            listModel->setItem( i, 1, new QStandardItem( "No" ) );
        }
        listModel->setHorizontalHeaderItem( 1, new QStandardItem( QString( "Has Voted" ) ) );

        // Display vote buttons if on team
        std::vector<unsigned int> team = *questingTeam_subscriber->getData<std::vector<unsigned int>>( );
        unsigned int myID = *myID_subscriber->getData<unsigned int>( );
        bool onTeam = false;
        for( unsigned int i = 0; i < team.size( ); i++ ) {
            if( team[i] == myID ) {
                onTeam = true;
                break;
            }
        }

        if( onTeam ) {
            ui->votingSection->show( );

            unsigned int myID = *myID_subscriber->getData<unsigned int>( );
            Player me = (*player_subscriber->getData<std::vector<Player> >( ))[myID];
            if( me.getAlignment( ) == avalon::GOOD ) {
                ui->buttonVoteFail->setText( "PASS!!!" );
            }

        } else {
            ui->votingSection->hide( );
        }

        // Subscribe to the data
        currentVotes_subscriber = new ClosureSubscriber(
                    [&]( Subscriber* ) {
                        emit currentVotesUpdated( );
                        sem_wait( sync_sem );
                    },
                    NULL);
        model->subscribe( "currentVotes", currentVotes_subscriber );
    } else {
        listModel->removeColumn( 1 );
        ui->stateLabel->setText( QString( "Team Selection" ) );
        ui->votingSection->hide( );
        ui->buttonVoteFail->setText( "FAIL!!!" );
    }

}

void GameWindow::updateAssassinStateSlot( ) {
    updateAssassinState( );
    sem_post( sync_sem );
}

void GameWindow::updateAssassinState( ) {
    // Do stuff for assassin

    bool inAssassinState = *assassinState_subscriber->getData<bool>( );
    if( inAssassinState ) {
        ui->stateLabel->setText( QString( "Assassin Round" ) );

    }
}

void GameWindow::updateEndGameStateSlot( ) {
    updateEndGameState( );
    sem_post( sync_sem );
}

void GameWindow::updateEndGameState( ) {
    // End game
    bool inEndGameState = *endGameState_subscriber->getData<bool>( );
    if( inEndGameState ) {
        Subscriber* winningTeam_subscriber = new ClosureSubscriber( NULL, NULL );
        model->subscribe( "winningTeam", winningTeam_subscriber );
        avalon::alignment_t winner = *winningTeam_subscriber->getData<avalon::alignment_t>( );
        unsigned int myID = *myID_subscriber->getData<unsigned int>( );
        Player myPlayer = (*player_subscriber->getData<std::vector<Player> >( ))[myID];

        std::string gameResultString = avalon::gui::getGameResultString( myPlayer.getAlignment( ), winner );

    }
}

void GameWindow::updateCurrentVotesSlot( ) {
    updateCurrentVotes( );
    sem_post( sync_sem );
}

void GameWindow::updateCurrentVotes( ) {

    std::vector<avalon::player_vote_t> votes = *currentVotes_subscriber->getData<std::vector<avalon::player_vote_t>>( );
    if( *teamVoteState_subscriber->getData<bool>( ) ) {
        for( unsigned int i = 0; i < votes.size( ); i++ ) {
            if( votes[i] != avalon::NO_VOTE ) {
                QStandardItemModel* listModel = ( QStandardItemModel* ) ui->playerList->model( );
                listModel->item( i, 3 )->setText( QString( "Yes" ) );
            }
        }
    } else {
        std::vector<unsigned int> team = *questingTeam_subscriber->getData<std::vector<unsigned int>>( );

        for( unsigned int i = 0; i < votes.size( ); i++ ) {
            if( votes[i] != avalon::NO_VOTE ) {
                unsigned int listPos = 0;
                for( ; listPos < team.size( ); listPos++ ) {
                    if( team[listPos] == i ) {
                        break;
                    }
                }
                QStandardItemModel* listModel = ( QStandardItemModel* ) ui->proposeTeamList->model( );
                listModel->item( listPos, 1 )->setText( QString( "Yes" ) );
            }
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

    // Show pop-up with results
    ResultsDialog results( NULL, &thisVote, NULL, player_subscriber );
    results.exec( );
}

void GameWindow::updateQuestHistorySlot( ) {
    updateQuestHistory( );
    sem_post( sync_sem );
}

void GameWindow::updateQuestHistory( ) {
    std::vector<QuestVoteHistory> hist = *questHistory_subscriber->getData<std::vector<QuestVoteHistory>>( );

    QuestVoteHistory thisQuest = hist.back( );

    // Show pop-up with results
    ResultsDialog results( NULL, NULL, &thisQuest, player_subscriber );
    results.exec( );
}

void GameWindow::updateChatMessagesSlot( ) {
    updateChatMessages( );
    sem_post( sync_sem );
}

void GameWindow::updateChatMessages( ) {
    std::vector<avalon::common::ChatMessage> messages =
            *chatMessages_subscriber->getData<std::vector<avalon::common::ChatMessage>>( );
    avalon::common::ChatMessage message = messages.back( );

    unsigned int senderID = message.getSenderId( );
    Player p = (*player_subscriber->getData<std::vector<Player> >( ) )[senderID];
    std::string senderName = p.getName( );

    std::string messageStr = senderName + ": " + message.getMessageText( );
    messageStr.erase(std::find_if(messageStr.rbegin(), messageStr.rend(),
                                  std::not1(std::ptr_fun<int, int>(std::isspace))).base(), messageStr.end());

    QStandardItemModel* listModel = (QStandardItemModel*) ui->chatList->model( );
    QStandardItem* messageItem = new QStandardItem( QString( messageStr.c_str( ) ) );
    listModel->setItem( listModel->rowCount( ), messageItem );
    ui->chatList->scrollToBottom( );
}


void GameWindow::on_playerList_clicked( const QModelIndex& index ) {
    if( leaderID_subscriber == NULL || myID_subscriber == NULL )
        return;

    unsigned int row = (unsigned int) index.row( );

    std::vector<unsigned int> qTeam = *questingTeam_subscriber->getData<std::vector<unsigned int>>( );

    // Remove this player and return if they are already on the team
    for( unsigned int i = 0; i < qTeam.size( ); i++ ) {
        if( qTeam[i] == row ) {
            SelectQuestGoerAction* act = new SelectQuestGoerAction( false, row );
            control->addActionToQueue( act );
            return;
        }
    }

    // Add the player if possible
    std::vector<unsigned int> playersPerQuest = *playersPerQuest_subscriber->getData<std::vector<unsigned int>>( );
    unsigned int currQuest = *currentQuestTrack_subscriber->getData<unsigned int>( );
    unsigned int playersCurrQuest = playersPerQuest[currQuest];
    if( playersCurrQuest > qTeam.size( ) ) {
        SelectQuestGoerAction* act = new SelectQuestGoerAction( true, row );
        control->addActionToQueue( act );
    }
}

void GameWindow::on_proposeTeamList_clicked( const QModelIndex& index ) {
    if( leaderID_subscriber == NULL || myID_subscriber == NULL )
        return;

    unsigned int row = ( unsigned int ) index.row( );

    // Remove the player in that row from the questing team
    std::vector<unsigned int> qTeam = *questingTeam_subscriber->getData<std::vector<unsigned int>>( );
    if( row < qTeam.size( ) ) {
        SelectQuestGoerAction* act = new SelectQuestGoerAction( false, qTeam[row] );
        control->addActionToQueue( act );
    }
}

void GameWindow::on_buttonVotePass_clicked( ) {
    if( *teamVoteState_subscriber->getData<bool>( ) ) {
        TeamVoteAction* vote = new TeamVoteAction( avalon::YES );
        control->addActionToQueue( vote );
    } else {
        QuestVoteAction* vote = new QuestVoteAction( avalon::YES );
        control->addActionToQueue( vote );
    }
}

void GameWindow::on_buttonVoteFail_clicked( ) {
    if( *teamVoteState_subscriber->getData<bool>( ) ) {
        TeamVoteAction* vote = new TeamVoteAction( avalon::NO );
        control->addActionToQueue( vote );
    } else {
        unsigned int myID = *myID_subscriber->getData<unsigned int>( );
        Player me = (*player_subscriber->getData<std::vector<Player> >( ))[myID];
        if( me.getAlignment( ) == avalon::GOOD ) {
            QuestVoteAction* vote = new QuestVoteAction( avalon::YES );
            control->addActionToQueue( vote );
        } else {
            QuestVoteAction* vote = new QuestVoteAction( avalon::NO );
            control->addActionToQueue( vote );
        }
    }
}

void GameWindow::on_proposeTeamButton_clicked( ) {
    FinalizeTeamAction* act = new FinalizeTeamAction( );
    control->addActionToQueue( act );
}

void GameWindow::on_sendMsgButton_clicked( ) {
    std::string message = ui->chatEdit->toPlainText( ).toStdString( );
    message = avalon::gui::trimString( message );
    if( message.empty( ) ) {
        ui->chatEdit->clear( );
        return;
    }
    unsigned int myID = *myID_subscriber->getData<unsigned int>( );
    avalon::common::ChatMessage* chatMess = new avalon::common::ChatMessage( myID, message, 0 );
    ChatMessageSentAction* act = new ChatMessageSentAction( *chatMess );

    control->addActionToQueue( act );

    ui->chatEdit->clear( );
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
