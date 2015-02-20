#include "optionswindow.h"
#include "ui_optionswindow.h"
#include <unistd.h>
#include <iostream>
#include <string>

/*
 *  Possible args:
 *  --players=x (This one is REQUIRED)
 *  --port=x
 *  --merlin
 *  --percival
 *  --morgana
 *  --mordred
 *  --oberon
 *  --assassin
 */
#define MAX_ARGS 8

OptionsWindow::OptionsWindow( QWidget *parent ) :
    QMainWindow( parent ),
    ui( new Ui::OptionsWindow ) {

    ui->setupUi(this);
    
    connect( ui->serverSectionCreateButton, SIGNAL( released( ) ), this, SLOT( createServer( ) ) );
}

OptionsWindow::~OptionsWindow( ) {
    delete ui;
}

void OptionsWindow::createServer() {
    //fprintf(stdout, "Fork!  If I could figure out how...");
    if( fork( ) == 0 ) {
        int i = 0;
        char* args[ MAX_ARGS + 1 ]; // +1 for the NULL pointer at the end of the array

        // Get the number of players
        std::string playerStr = "--players=";
        playerStr += std::to_string( ui->serverSectionPlayerAmount->value( ) );
        std::cout << playerStr << std::endl;

        // First argument must be the executable
        args[ i++ ] = (char*)"../server.exe";
        args[ i++ ] = (char*)playerStr.c_str();
        args[ i ] = NULL; // Must be null terminated
        
        // execv takes an array for the arguments
        execv( args[ 0 ], args );

        // If we get here, the executable crashed, so exit this process (The main process is still running)
        // If the server exits on its own, we won't get here
        std::cerr << "Unable to find server executable, or something terrible happened!" << std::endl;
        exit( 1 );
    }
    else {
        std::cout << "Server Created." << std::endl;
    }
}
