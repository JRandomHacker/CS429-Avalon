#include "globals.hpp"
#include "clientController.hpp"
#include "optionswindow.h"
#include "connectwindow.h"
#include "gamewindow.h"
#include "joinserverwindow.h"
#include "createserverwindow.h"
#include "ui_optionswindow.h"
#include <unistd.h>
#include <iostream>
#include <string>

#ifdef _WIN32
	#include <windows.h>
#endif

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
    
    //connect( ui->serverSectionCreateButton, SIGNAL( released( ) ), this, SLOT( createServer( ) ) );
    //connect( ui->clientSectionJoinButton, SIGNAL( released( ) ), this, SLOT( joinServerSlot( ) ) );
}

OptionsWindow::~OptionsWindow( ) {
    delete ui;
}
/*
void OptionsWindow::createServer() {
	#ifdef _WIN32
		std::string execStr = "server.exe"; 

        // Get the number of players
        execStr += " --players=";
        execStr += std::to_string( ui->serverSectionPlayerAmount->value( ) );

        QList<QListWidgetItem* > roles = ui->listWidget->selectedItems();

        for ( int j = 0; j < roles.length(); j++ ) {
            std::string flagStr = " --";
            flagStr += roles[j]->text().toStdString();
            execStr += flagStr;
        }

		// Setup required structs for windows process
		STARTUPINFOA StartupInfo;
		PROCESS_INFORMATION ProcessInfo;
		
		// Don't show the console in the new process
		memset(&StartupInfo, 0, sizeof(StartupInfo));
		StartupInfo.cb = sizeof(STARTUPINFO);
		StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
		StartupInfo.wShowWindow = SW_HIDE;
		
		char* execChar = new char[ execStr.size( ) ];
		strcpy( execChar, execStr.c_str( ) );

		if (!CreateProcessA( NULL, execChar, NULL, NULL, FALSE,
			DETACHED_PROCESS, 
			NULL, 
			NULL,
			&StartupInfo,
			&ProcessInfo))
		{
			std::cerr << "Unable to find server executable, or something terrible happened!" << std::endl;	
		}
	#else 
		if( fork( ) == 0 ) {
	        int i = 0;
	        char* args[ MAX_ARGS + 1 ]; // +1 for the NULL pointer at the end of the array
	
	        // Get the number of players
	        std::string playerStr = "--players=";
	        playerStr += std::to_string( ui->serverSectionPlayerAmount->value( ) );

	        QList<QListWidgetItem* > roles = ui->listWidget->selectedItems();
	
	        // First argument must be the executable
	        args[ i++ ] = (char*)"./server.exe";
	        args[ i++ ] = (char*)playerStr.c_str();
	
	        for ( int j = 0; j < roles.length(); j++ ) {
                std::string flagStr = "--";
                flagStr += roles[j]->text().toStdString();
                char* tempFlagStr = new char[ flagStr.size( ) ];
                strcpy( tempFlagStr, flagStr.c_str() );
                args[ i++ ] = tempFlagStr;
	        }
	
	        args[ i ] = NULL; // Must be null terminated
	        // execv takes an array for the arguments
	        execv( args[ 0 ], args );
	
	        // If we get here, the executable crashed, so exit this process (The main process is still running)
	        // If the server exits on its own, we won't get here
	        std::cerr << "Unable to find server executable, or something terrible happened!" << std::endl;
	        exit( 1 );
	    }
    #endif
    sleep(1);
    joinServer( "localhost", DEFAULT_PORT );
}*

void OptionsWindow::joinServerSlot( ) {
    joinServer( ui->clientSectionServerAddressField->text().toStdString(), DEFAULT_PORT );
*/

void OptionsWindow::on_buttonJoinMenu_clicked()
{
    JoinServerWindow w;
    w.setModal(true);
    w.exec();
}

void OptionsWindow::on_buttonQuit_clicked()
{
    this->close();
}

void OptionsWindow::on_buttonCreateMenu_clicked()
{
    CreateServerWindow w;
    w.setModal(true);
    w.exec();
}
/*
void OptionsWindow::joinServer( std::string host, int port ) {
    ClientController controller( NULL, host, port );
    controller.processActions( );
}
*/