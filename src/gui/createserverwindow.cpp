#include "createserverwindow.hpp"
#include "gamewindow.hpp"
#include "optionswindow.hpp"
#include "ui_createserverwindow.h"
#include "globals.hpp"
#include "displayerrors.hpp"
#include "clientController.hpp"
#include "clientCustomActionsFromGUI.hpp"

#include <unistd.h>
#include <iostream>
#include <string>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <sys/wait.h>
#endif

CreateServerWindow::CreateServerWindow( QWidget *parent ) :
    QDialog( parent ),
    ui( new Ui::CreateServerWindow ) {
    ui->setupUi( this );
    
    serverH = 0;

    ui->editPortNum->insert( QString( std::to_string( DEFAULT_PORT ).c_str( ) ) );

    model = new Model( );
    controller = new ClientController( model );
}

CreateServerWindow::~CreateServerWindow( ) {
    delete controller;
    delete model;
    delete ui;
}

int CreateServerWindow::createServer( ) {
    #ifdef _WIN32
        std::string execStr = "server.exe";

        // Get the number of players
        execStr += " --players=";
        execStr += std::to_string( ui->sbNumPlayers->value( ) );
        // Get the port number
        execStr += " --port=";
        execStr += ui->editPortNum->text( ).toStdString( );

        if( ui->cbOpenVoting->isChecked( ) )
            execStr += " --hidden";

        QList<QListWidgetItem* > roles = ui->listWidget->selectedItems( );

        for ( int j = 0; j < roles.length( ); j++ ) {
            std::string flagStr = " --";
            flagStr += roles[j]->text( ).toStdString( );
            execStr += flagStr;
        }

        // Setup required structs for windows process
        STARTUPINFOA StartupInfo;
        PROCESS_INFORMATION ProcessInfo;

        // Don't show the console in the new process
        memset( &StartupInfo, 0, sizeof( StartupInfo ) );
        StartupInfo.cb = sizeof( STARTUPINFO );
        StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
        StartupInfo.wShowWindow = SW_HIDE;

        char* execChar = new char[ execStr.size( ) ];
        strcpy( execChar, execStr.c_str( ) );

        if ( !CreateProcessA( NULL, execChar, NULL, NULL, FALSE,
            DETACHED_PROCESS,
            NULL,
            NULL,
            &StartupInfo,
            &ProcessInfo ) ) {
            std::cerr << "Unable to find server executable, or something terrible happened!" << std::endl;
            return EXIT_SERVER_NOT_FOUND;
        }
        Sleep( 2000 );
        
        serverH = ProcessInfo.hProcess;

        DWORD exitCode;
        GetExitCodeProcess( ProcessInfo.hProcess, &exitCode );
        if( exitCode != STILL_ACTIVE ) {
            if( exitCode > INT_MAX ) {
                std::cerr << "Windows exit code error" << std::endl;
                return UNKNOWN_ERROR;
            }

            return ( int )exitCode;
        }
    #else
        int pid = fork( );
        if( pid == 0 ) {
            int i = 0;
            char* args[ MAX_ARGS + 1 ]; // +1 for the NULL pointer at the end of the array

            // Get the number of players
            std::string playerStr = "--players=";
            playerStr += std::to_string( ui->sbNumPlayers->value( ) );

            // Get the port
            std::string portStr = "--port=";
            portStr += ui->editPortNum->text( ).toStdString( );

            if( ui->cbOpenVoting->isChecked( ) )
                playerStr += " --hidden";

            QList< QListWidgetItem* > roles = ui->listWidget->selectedItems( );

            // First argument must be the executable
            args[ i++ ] = ( char* )"./server.exe";
            args[ i++ ] = ( char* )playerStr.c_str( );
            args[ i++ ] = ( char* )portStr.c_str( );

            for ( int j = 0; j < roles.length( ); j++ ) {
                std::string flagStr = "--";
                flagStr += roles[j]->text( ).toStdString( );
                char* tempFlagStr = new char[ flagStr.size( ) ];
                strcpy( tempFlagStr, flagStr.c_str( ) );
                args[ i++ ] = tempFlagStr;
            }

            args[ i ] = NULL; // Must be null terminated
            // execv takes an array for the arguments
            execv( args[ 0 ], args );

            // If we get here, the executable crashed, so exit this process (The main process is still running)
            // If the server exits on its own, we won't get here
            std::cerr << "Unable to find server executable, or something terrible happened!" << std::endl;
            exit( EXIT_SERVER_NOT_FOUND );
        } else if( pid > 0 ) {
            serverH = pid;
            sleep( 2 );
            int status;
            if( waitpid( pid, &status, WNOHANG ) == pid ) {
                return WEXITSTATUS( status );
            }
        }
    #endif
    return 0;
}

void CreateServerWindow::on_buttonCreateServer_clicked( ) {
    int serverCreation = createServer( );
    if( serverCreation != EXIT_SUCCESS ) {
        displayError( serverCreation, this );
    } else {
        connectToServer( );
    }
}

void CreateServerWindow::on_buttonCancel_clicked( ) {
    this->parentWidget( )->show( );
    
    this->hide( );
}


void CreateServerWindow::connectToServer( ) {
    int port = ui->editPortNum->text( ).toInt( );

    int status = controller->spawnNetwork( "localhost", port );
    if( status != EXIT_SUCCESS ) {
        displayError( status, this );
    } else {

        std::string name = ui->editPlayerName->text( ).toStdString( );
        SetNameAction* setName = new SetNameAction( name );
        controller->addActionToQueue( ( Action* )setName );
        GameWindow* g = new GameWindow( NULL, controller, model, serverH );
        //g->setModal( false );
        g->show( );

        this->hide( );
    }
}
