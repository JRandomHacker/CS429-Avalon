#include "gamewindow.h"
#include "ui_gamewindow.h"
#include "subscriber.hpp"
#include "player.hpp"
#include <QStandardItem>
#include <thread>
#include <chrono>

GameWindow::GameWindow(QWidget *parent, ClientController* controller, Model * model) :
    QDialog(parent),
    ui(new Ui::GameWindow)
{
    this->control = controller;
    this->model = model;
    ui->setupUi(this);
    startWatchOnHasGameSettings();
    
    // Start up thread for controller
	pthread_t controlThread;    
    if( pthread_create( &controlThread, NULL, &controlThreadFn, this->control ) != 0 )
    {
        std::cerr << "Unable to start controller thread" << std::endl;
        exit( EXIT_THREAD_ERROR );
    }	
    
    // Create thread to watch for new players and update their info
    pthread_t playerInfoThread;    
    if( pthread_create( &playerInfoThread, NULL, &waitForPlayers, this ) != 0 )
    {
        std::cerr << "Unable to start player subscription thread" << std::endl;
        exit( EXIT_THREAD_ERROR );
    }	
    
}

GameWindow::~GameWindow()
{
    delete ui;
}

void* GameWindow::waitForPlayers(void* data)
{
    GameWindow* gw = (GameWindow*) data;

    // Wait for game data to arrive
    ClosureSubscriber nPlayersSub( NULL, NULL);
    while(!gw->model->subscribe("numberOfPlayers", &nPlayersSub))
        Sleep(10);
    
    unsigned int nPlayers = *(nPlayersSub.getData<unsigned int>());
    
    ClosureSubscriber myIDSub( NULL, NULL );
    while(!gw->model->subscribe("myID", &myIDSub))
        std::this_thread::sleep_for(std::chrono::seconds(1));
    int myID = *(myIDSub.getData<int>());
    
    // Add the appropriate number of players to the player list
    std::string playerInfoStrings[nPlayers];
    QStandardItemModel* listModel = new QStandardItemModel();
    gw->ui->playerList->setModel(listModel);
    for(unsigned int i = 0; i < nPlayers; i++)
    {
        playerInfoStrings[i] = "Player " + std::to_string(i);
        if((int) i == myID)
            playerInfoStrings[i] += " (me)";
        playerInfoStrings[i] += " (not connected)";
        
        QStandardItem* pli = new QStandardItem();
        pli->setText(QString(playerInfoStrings[i].c_str()));
        listModel->appendRow(pli);
    }
    
    // Wait for the other players to connect
    ClosureSubscriber* pSubs[nPlayers];
    for(unsigned int i = 0; i < nPlayers; i++)
    {
        std::string newString = "Player " + std::to_string(i);
        if((int) i == myID)
        {
            newString += " (me)";
        }
        newString += " (connected)";
        
        // This subscriber will change the player to appear "connected" if the data becomes non-null
        pSubs[i] = new ClosureSubscriber( [i, newString, listModel](Subscriber* s){
            if(s->getData<Player*>() != NULL)
                listModel->item(i)->setText(QString(newString.c_str()));
        }, NULL);
        
        while(!gw->model->subscribe(std::string("player") + std::to_string(i), pSubs[i]))
            std::this_thread::sleep_for(std::chrono::seconds(1));
        
        // If this player is already connected, no need to stay subscribed.
        if(pSubs[i]->getData<Player*>() != NULL)
        {
            listModel->item(i)->setText(QString(newString.c_str()));
            gw->model->unsubscribe(std::string("player") + std::to_string(i), pSubs[i]);
        }
    }
    
    return NULL;
}
    
void GameWindow::startWatchOnHasGameSettings( ) {
    model->subscribe("hasGameSettings", new ClosureSubscriber(
        [&](Subscriber* s){
            if (s->getData<bool>()) {
                createPlayerSubscribers();
            }
        },
        [&](Subscriber*){}));
}

void GameWindow::createPlayerSubscribers( ) {
    num_players_subscriber = new ClosureSubscriber(
        [&](Subscriber*){},
        [&](Subscriber*){});
    model->subscribe("numberOfPlayers", num_players_subscriber);
    unsigned int num_players = *num_players_subscriber->getData<unsigned int>();
    // Where the GUI for all the players should be added

    for (unsigned int i = 0; i < num_players; i++) {
        player_subscribers.push_back(new ClosureSubscriber(
            [&](Subscriber* s){
                Player* p = *(s->getData<Player*>());
                std::cout << "Player being added: " << p->getName() << std::endl;
                // Where the GUI for the specific player should be updated
            },
            [&](Subscriber*){}));
        model->subscribe(std::string("player")+std::to_string(i), player_subscribers[i]);
    }
}

void* GameWindow::controlThreadFn(void* data)
{   
    ((ClientController*) data)->processActions();
    return NULL;
}