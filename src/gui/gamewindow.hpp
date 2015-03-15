#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QDialog>
#include <QKeyEvent>
#include "clientController.hpp"
#include "model.hpp"
#include "subscriber.hpp"
#ifdef _WIN32
    #include <windows.h>
#endif

namespace Ui {
class GameWindow;
}

/**
 *  Game window class. This is the window where the game is played.
 *  Displays the progress of the game.
 */
class GameWindow : public QDialog {
    Q_OBJECT

public:
    #ifdef _WIN32
    /**
     *  Creates a game window, starts the controller thread.
     */
    explicit GameWindow( QWidget *parent = 0, ClientController* controller = NULL, Model * model = NULL, HANDLE serverHandle = 0 );
    #else
    /**
     *  Creates a game window, starts the controller thread.
     */
    explicit GameWindow( QWidget *parent = 0, ClientController* controller = NULL, Model * model = NULL, int serverHandle = 0 );
    #endif
    
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
    void closeEvent(QCloseEvent* event);

    /**
     * Overrides the escape key to clean up the server if on host
     *
     * @param event Information about the key that was pressed
     * @return None
     */
    void keyPressEvent( QKeyEvent* event );

signals:
    /**
     * A function that is called whenever the GameSettings protobuf is received
     *
     * @return None
     */
    void gameSettingsReceived( );

    /**
     * A function that is called whenever a player gets updated information
     *
     * @param playerNum The ID of the player that is being updated
     * @return None
     */
    void playerInfoUpdated( unsigned int playerNum );

    /**
     * A function that is called when the GameSettings have finished setting things up
     *
     * @return None
     */
    void gameInfoUpdated( );

private slots:
    /**
     *  Makes subscriber to get num of players.
     *  Makes subscribers to current player ID.
     *  Creates and displays the list of players.
     *  Makes a subscriber list and registers them for each of the players.
     */
    void createPlayerSubscribers( );

    /**
     * Updates a player's entry in the player list
     * @param id The playerNum of the updated player
     */
    void updatePlayer(unsigned int id );

    /**
     *  Updates the numEvil and roleList UI elements
     */
    void updateGameInfo();
    
    /**
     *  Callback for positive vote button
     */
    void on_buttonVotePass_clicked();
    
    /**
     *  Callback for negative vote button
     */
    void on_buttonVoteFail_clicked();

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

    /**
     *  Subscriber that gets num of players from the model.
     */
    Subscriber* num_players_subscriber;

    /**
     *  Subscriber that gets current player ID from the model.
     */
    Subscriber* myID_subscriber;
    
    Subscriber* numEvil_subscriber;
    
    Subscriber* roleList_subscriber;
    
    /**
     *  Subscriber vector that watches player objects.
     */
    std::vector<Subscriber*> player_subscribers;
    

    #ifdef _WIN32
        /**
         *  Process handle of server, if hosting
         */
        HANDLE serverH;
    #else
        /**
         *  pid of the server, if hosting
         */
        int serverH;
    #endif

    /**
     *  Create a subscriber that checks if the model has game settings, 
     *  and if it does, create player subscribers
     */
    void startWatchOnHasGameSettings( );

    /**
     *  Process the action queue for given client controller. 
     * 
     *  @param   data    pointer to client controller
     */
    static void* controlThreadFn( void* clientController );
};

#endif // GAMEWINDOW_H
