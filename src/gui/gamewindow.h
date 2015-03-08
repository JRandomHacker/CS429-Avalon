#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QDialog>
#include "clientController.hpp"
#include "model.hpp"
#include "subscriber.hpp"

namespace Ui {
class GameWindow;
}

/**
 *  Game window class. This is the window where the game is played.
 *  Displays the progress of the game.
 */
class GameWindow : public QDialog
{
    Q_OBJECT

public:
    /**
     *  Creates a game window, starts the controller thread.
     */
    explicit GameWindow(QWidget *parent = 0, ClientController* controller = NULL, Model * model = NULL);

    /**
     *  Destroys the game window.
     */
    ~GameWindow();

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

    /**
     *  Subscriber vector that watches player objects.
     */
    std::vector<Subscriber*> player_subscribers;

    /**
     *  Create a subscriber that checks if the model has game settings, 
     *  and if it does, create player subscribers
     */
    void startWatchOnHasGameSettings();

    /**
     *  Makes subscriber to get num of players.
     *  Makes subscribers to current player ID.
     *  Creates and displays the list of players.
     *  Makes a subscriber list and registers them for each of the players.
     */
    void createPlayerSubscribers();
    
    /**
     *  Process the action queue for given client controller. 
     * 
     *  @param   data    pointer to client controller
     */
    static void* controlThreadFn(void* clientController);
};

#endif // GAMEWINDOW_H
