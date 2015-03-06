#include "createserverwindow.h"
#include "connectwindow.h"
#include "ui_createserverwindow.h"
#include "globals.hpp"
#include "clientController.hpp"
#include <unistd.h>
#include <iostream>
#include <string>

CreateServerWindow::CreateServerWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateServerWindow)
{
    ui->setupUi(this);
    
    ui->editPortNum->insert(QString(std::to_string(DEFAULT_PORT).c_str()));
    
}

CreateServerWindow::~CreateServerWindow()
{
    delete ui;
}

void CreateServerWindow::createServer() {
	#ifdef _WIN32
		std::string execStr = "server.exe"; 

        // Get the number of players
        execStr += " --players=";
        execStr += std::to_string( ui->sbNumPlayers->value( ) );
        execStr += " --port=" + ui->editPortNum->text().toStdString();

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
		Sleep(1500);
	#else 
		if( fork( ) == 0 ) {
	        int i = 0;
	        char* args[ MAX_ARGS + 1 ]; // +1 for the NULL pointer at the end of the array
	
	        // Get the number of players
	        std::string playerStr = "--players=";
	        playerStr += std::to_string( ui->sbNumPlayers->value( ) );

	        QList<QListWidgetItem* > roles = ui->listWidget->selectedItems();
	
	        // First argument must be the executable
	        args[ i++ ] = (char*)"./server.exe";
	        args[ i++ ] = (char*)playerStr.c_str();
            args[ i++ ] = (char*)ui->editPortNum->text().toStdString().c_str( );
          
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
	sleep(1);
    #endif
    
    
    
}

void CreateServerWindow::on_buttonCreateServer_clicked()
{
    createServer();

    int port = ui->editPortNum->text().toInt();
    ConnectWindow w(this, "localhost", port);
    w.exec();
    
    close();
}


