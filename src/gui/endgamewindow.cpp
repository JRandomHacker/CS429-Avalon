#include "endgamewindow.hpp"
#include "ui_endgamewindow.h"

EndGameWindow::EndGameWindow(QWidget *parent, std::vector<Player*> allPlayers, std::string resultString) :
    QDialog(parent),
    ui(new Ui::EndGameWindow)
{
    ui->setupUi(this);

    ui->resultsText->setText( QString( resultString.c_str( ) ) );

    //Display all players in the list
}

EndGameWindow::~EndGameWindow()
{
    delete ui;
}
