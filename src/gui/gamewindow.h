#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QDialog>
#include "clientController.hpp"
#include "model.hpp"
#include "subscriber.hpp"

namespace Ui {
class GameWindow;
}

class GameWindow : public QDialog
{
    Q_OBJECT

public:
    explicit GameWindow(QWidget *parent = 0, ClientController* controller = NULL, Model * model = NULL);
    ~GameWindow();

private:
    Ui::GameWindow *ui;
    ClientController * control;
    Model * model;

    Subscriber* num_players_subscriber;
    Subscriber* myID_subscriber;
    std::vector<Subscriber*> player_subscribers;

    void startWatchOnHasGameSettings();
    void createPlayerSubscribers();
    
    static void* waitForPlayers(void* data);
    
    static void* controlThreadFn(void* data);
};

#endif // GAMEWINDOW_H
