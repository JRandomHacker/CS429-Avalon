#include "gamewindow.h"
#include "ui_gamewindow.h"

GameWindow::GameWindow(QWidget *parent, ClientController* controller, Model * model) :
    QDialog(parent),
    ui(new Ui::GameWindow)
{
    this->control = controller;
    this->model = model;
    ui->setupUi(this);
    startWatchOnHasGameSettings();
    
	pthread_t controlThread;    
    if( pthread_create( &controlThread, NULL, &controlThreadFn, this->control ) != 0 )
    {
        std::cerr << "Unable to start controller thread" << std::endl;
        exit( EXIT_THREAD_ERROR );
    }	
}

GameWindow::~GameWindow()
{
    delete ui;
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