#include "displayerrors.hpp"
#include "globals.hpp"
#include <string>
#include <QWidget>
#include <QErrorMessage>

void displayError( int errorCode, QWidget *parent ) {

    std::string message;
    switch ( errorCode ) {
        case EXIT_INVALID_PLAYERS_ERROR:
            message = "Invalid player number";
            break;

        case EXIT_THREAD_ERROR:
            message = "Unable to spawn a thread";
            break;

        case EXIT_SOCKET_ERROR:
            message = "Creation: Socket already bound\n";
            message += "Join: Running server not found";
            break;

        case EXIT_EVIL_ERROR:
            message = "More special evil than total evil";
            break;

        case EXIT_GOOD_ERROR:
            message = "More special good characters than total good";
            break;

        case EXIT_SPECIAL_ERROR:
            message = "More special characters than players";
            break;

        case EXIT_SERVER_NOT_FOUND:
            message = "Unable to find server executable";
            break;

        case EXIT_NETWORK_ERROR:
            message = "Generic network error";
            break;

        default:
            message = "Unknown error";
            break;
    }

    QErrorMessage error( parent );
    error.showMessage( message.c_str( ) );
    error.exec( );
}
