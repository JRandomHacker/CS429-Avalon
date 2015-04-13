#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QDialog>
#include <QKeyEvent>
#include "clientController.hpp"
#include "model.hpp"
#include "subscriber.hpp"
#include <semaphore.h>
#ifdef _WIN32
#include <windows.h>
#endif

#ifdef _WIN32
    typedef HANDLE handle_t;
#else
    typedef int handle_t;
#endif

namespace Ui {
class GameWindow;
}

/**
 *  Game window class. This is the window where the game is played.
 *  Displays the progress of the game.
 */
class GameWindow : public QWidget {
    Q_OBJECT

public:
    /**
     *  Creates a game window, starts the controller thread.
     */
    explicit GameWindow( QWidget *parent = 0, ClientController* controller = NULL, Model * model = NULL, handle_t serverHandle = 0 );

    
    /**
     *  Destroys the game window.
     */
    ~GameWindow( );
    
    /**
     *  On close, kills the server if this client is the host
     *
     *  @param event The CloseEvent that happened, causing this function to be called
     *  @return None
     */
    void closeEvent( QCloseEvent* event );

    /**
     * Overrides the escape key to clean up the server if on host
     *
     * @param event Information about the key that was pressed
     * @return None
     */
    void keyPressEvent( QKeyEvent* event );

signals:
    /**
     * A signal that is emitted whenever the GameSettings protobuf is received
     *
     * @return None
     */
    void gameSettingsReceived( );

    /**
     * A signal that is emitted whenever a player gets updated information
     *
     * @param playerNum The ID of the player that is being updated
     * @return None
     */
    void playerInfoUpdated( unsigned int playerNum );

    /**
     * A signal that is emitted when the GameSettings have finished setting things up
     *
     * @return None
     */
    void gameInfoUpdated( );

    /**
     * A signal that is emitted when the leader ID is updated
     * @return None
     */
    void leaderIDUpdated( );

    /**
     * A signal that is emitted when the questing team is updated
     *
     * @return None
     */
    void questingTeamUpdated( );

    /**
     * Signal emitted when vote track or quest track is updated
     *
     * @return None
     */
    void trackUpdated( );

    /**
     * Signal emitted when team vote state is entered or exited
     *
     * @return None
     */
    void teamVoteStateUpdated( );

    /**
     * Signal emitted when quest vote state is entered or exited
     *
     * @return None
     */
    void questVoteStateUpdated( );

    /**
     * Signal emitted when vote history is updated
     *
     * @return  None
     */
    void voteHistoryUpdated( );

    /**
     * Signal emitted when quest history is updated
     *
     * @return None
     */
    void questHistoryUpdated( );

    /**
     * Signal emitted when current votes is updated
     *
     * @return None
     */
    void currentVotesUpdated( );

    /**
     * Signal emitted when chatmessages are updated
     */
    void chatMessagesUpdated( );

private slots:
    /**
     *  Makes subscriber to get num of players.
     *  Makes subscribers to current player ID.
     *  Creates and displays the list of players.
     *  Makes a subscriber list and registers them for each of the players.
     *
     * @return None
     */
    void createPlayerSubscribersSlot( );

    /**
     * Updates a player's entry in the player list
     * @param id The playerNum of the updated player
     * @return None
     */
    void updatePlayerSlot( unsigned int id );

    /**
     *  Updates the numEvil and roleList UI elements
     *
     * @return None
     */
    void updateGameInfoSlot( );

    /**
     *  Changes gui for leader and non-leaders
     *
     * @return None
     */
    void updateLeaderSlot( );

    /**
     * Display the current questing team.
     *
     * @return None
     */
    void updateQuestingTeamSlot( );

    /**
     * Displays info about vote/quest tracks
     *
     * @return None
     */
    void updateTrackSlot( );

    /**
     * Changes gui to enable/disable team vote state
     * @return None
     */
    void updateTeamVoteStateSlot( );

    /**
     * Changes gui to enable/disable quest vote state
     */
    void updateQuestVoteStateSlot( );

    /**
     * Pops up vote result
     */
    void updateVoteHistorySlot( );

    /**
     * Pops up quest result
     */
    void updateQuestHistorySlot( );

    /**
     * Updates who has voted
     */
    void updateCurrentVotesSlot( );

    /**
     * Callback for chats
     */
    void updateChatMessagesSlot( );
    
    /**
     * When the leader clicks a player, they are toggled on/off the team
     *
     * @return None
     */
    void on_playerList_clicked( const QModelIndex &index );

    /**
     *  Callback for positive vote button
     *
     * @return None
     */
    void on_buttonVotePass_clicked( );
    
    /**
     *  Callback for negative vote button
     *
     * @return None
     */
    void on_buttonVoteFail_clicked( );

    /**
     * Callback to propose a team
     *
     * @return None
     */
    void on_proposeTeamButton_clicked( );

    /**
     * Callback to send chat messages
     */
    void on_sendMsgButton_clicked( );

private:
    /**
     *  UI field for gamewindow
     */
    Ui::GameWindow *ui;

    /**
     *  
     */
    ClientController * control;

    /**
     *  Pointer to model for client
     */
    Model * model;

    sem_t * sync_sem;

    /**
     *  Subscriber that gets num of players from the model.
     */
    Subscriber* num_players_subscriber;

    /**
     *  Subscriber that gets current player ID from the model.
     */
    Subscriber* myID_subscriber;

    /**
     *  Subscriber that gets the number of evil players from the model.
     */
    Subscriber* numEvil_subscriber;
    
    /**
     *  Subscriber that gets list of roles in the game from the model.
     */
    Subscriber* roleList_subscriber;

    /**
     *  Subscriber that gets current leader's ID from the model.
     */
    Subscriber* leaderID_subscriber;

    /**
     *  Subscriber that gets current questing team from the model.
     */
    Subscriber* questingTeam_subscriber;

    /**
     *  Subscriber for vote track length
     */
    Subscriber* voteTrackLength_subscriber;

    /**
     *  Subscriber for current vote number
     */
    Subscriber* currentVoteTrack_subscriber;

    /**
     *  Subscriber for current quest number
     */
    Subscriber* currentQuestTrack_subscriber;
    
    /**
     *  Subscriber for quest track length
     */
    Subscriber* questTrackLength_subscriber;

    /**
     *  Subscriber for players per quest
     */
    Subscriber* playersPerQuest_subscriber;

    /**
     *  Subscriber for team vote state flag
     */
    Subscriber* teamVoteState_subscriber;

    /**
     *  Subscriber for quest voting flag
     */
    Subscriber* questVoteState_subscriber;

    /**
     * Subscriber for vote history
     */
    Subscriber* voteHistory_subscriber;

    /**
     *  Subscriber for quest history
     */
    Subscriber* questHistory_subscriber;

    /**
     * Subscriber for current votes
     */
    Subscriber* currentVotes_subscriber;

    /**
     * Subscriber for chat
     */
    Subscriber* chatMessages_subscriber;


    /**
     *  Subscriber vector that watches player objects.
     */
    std::vector<Subscriber*> player_subscribers;
    
    handle_t serverH;

    /**
     *  Create a subscriber that checks if the model has game settings, 
     *  and if it does, create player subscribers
     *
     * @return None
     */
    void startWatchOnHasGameSettings( );

    /**
     *  Process the action queue for given client controller. 
     * 
     *  @param   data    pointer to client controller
     * @return None
     */
    static void* controlThreadFn( void* clientController );

    /**
     *  Makes subscriber to get num of players.
     *  Makes subscribers to current player ID.
     *  Creates and displays the list of players.
     *  Makes a subscriber list and registers them for each of the players.
     *
     * @return None
     */
    void createPlayerSubscribers( );

    /**
     * Updates a player's entry in the player list
     * @param id The playerNum of the updated player
     * @return None
     */
    void updatePlayer( unsigned int id );

    /**
     *  Updates the numEvil and roleList UI elements
     *
     * @return None
     */
    void updateGameInfo( );

    /**
     *  Changes gui for leader and non-leaders
     *
     * @return None
     */
    void updateLeader( );

    /**
     * Display the current questing team.
     *
     * @return None
     */
    void updateQuestingTeam( );

    /**
      *
      * Builds the array of quest icons.
      *
      * @return None
      */
    void buildQuestIcons( );

    /**
     * Displays info about vote/quest tracks
     *
     * @return None
     */
    void updateTrack( );

    /**
     * Changes gui to enable/disable team voting
     * @return None
     */
    void updateTeamVoteState( );

    /**
     * Changes gui to enable/disable quest voting
     */
    void updateQuestVoteState( );

    /**
     * Pops up vote result
     */
    void updateVoteHistory( );

    /**
     * Pops up quest result
     */
    void updateQuestHistory( );

    /**
     * Updates who has voted
     */
    void updateCurrentVotes( );

    /**
     * Updates chat log
     */
    void updateChatMessages( );
};

#endif // GAMEWINDOW_H
