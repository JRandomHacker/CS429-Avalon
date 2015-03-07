#include "createserverwindow.h"
#include "gamewindow.h"
#include "ui_createserverwindow.h"
#include "globals.hpp"
#include "clientController.hpp"
#include <unistd.h>
#include <iostream>
#include <string>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <sys/wait.h>
#endif

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

int CreateServerWindow::createServer() {
    #ifdef _WIN32
        std::string execStr = "server.exe";

        // Get the number of players
        execStr += " --players=";
        execStr += std::to_string( ui->sbNumPlayers->value( ) );
        // Get the port number
        execStr += " --port=";
        execStr += ui->editPortNum->text().toStdString();

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
            return EXIT_SERVER_NOT_FOUND;
        }
        Sleep( 2000 );

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
            portStr += ui->editPortNum->text().toStdString();

            QList< QListWidgetItem* > roles = ui->listWidget->selectedItems();

            // First argument must be the executable
            args[ i++ ] = (char*)"./server.exe";
            args[ i++ ] = (char*)playerStr.c_str( );
            args[ i++ ] = (char*)portStr.c_str( );

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
            exit( EXIT_SERVER_NOT_FOUND );
        } else if( pid > 0 ) {
            sleep( 2 );
            int status;
            if( waitpid( pid, &status, WNOHANG ) == pid ) {
                return WEXITSTATUS( status );
            }
        }
    #endif

    return 0;
}

void CreateServerWindow::on_buttonCreateServer_clicked( )
{
    int serverCreation = createServer( );
    switch( serverCreation ) {
        case 0:
            std::cout << "Connecting to server" << std::endl;
            connectToServer( );
            break;
        case EXIT_INVALID_PLAYERS_ERROR:
            std::cout << "Invalid players number" << std::endl;
            // Not enough players
            break;
        case EXIT_SOCKET_ERROR:
            std::cout << "socket already bound" << std::endl;
            // Unable to bind port (probably... technically just a generic socketing error)
            break;
        case EXIT_EVIL_ERROR:
            std::cout << "More special evil than evil" << std::endl;
            // More evil specials than evil players
            break;
        case EXIT_SERVER_NOT_FOUND:
            std::cout << "server not found" << std::endl;
            // Unable to find server executable
            break;
    }
}


void CreateServerWindow::connectToServer( )
{
    int port = ui->editPortNum->text().toInt();
    
    Model m;
    ClientController controller( &m, "localhost", port );
    
    GameWindow g(this, &controller, &m);
    g.exec();
    
    close();
}
