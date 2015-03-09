#include <iostream>
#include <cstring>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #define WINSOCK_MAGIC 0x202
#else
    #include <netdb.h>
#endif

#include "globals.hpp"
#include "player.hpp"

#include "model.hpp"
#include "subscriber.hpp"

#include "../protos/player.pb.h"


int main( int argc, char** argv ) {

    // Construct model and subscribe (as view) with a print method
    Model* model = new Model( );
    model->addData( "player1" );

    ClosureSubscriber* sub = new ClosureSubscriber(
        [&]( Subscriber* s ){
            Player* p = s->getData< Player >();
            if ( p != NULL ) {
                std::cout << "Player name: " << p->getName( ) << std::endl;
                std::cout << "Role ID#: " << p->getRole( ) << std::endl;
                std::cout << "You are ";
                if ( p->getAlignment( ) == avalon::GOOD ) {
                    std::cout << "good." << std::endl;
                } else {
                    std::cout << "evil." << std::endl;
                }

            }
        }, [&]( Subscriber* ) { }
    );
    model->subscribe("player1", sub);

    // Network initialization
    int rv;
    struct addrinfo hints, *servinfo;
    #ifdef _WIN32
        SOCKET sock;

        WSADATA wsaData;
        if( WSAStartup( WINSOCK_MAGIC, &wsaData ) != 0 ) {
            std::cerr << "Unable to magic windows" << std::endl;
        }
    #else
        int sock;
    #endif

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    if( ( rv = getaddrinfo( "localhost", std::to_string( DEFAULT_PORT ).c_str(), &hints, &servinfo ) ) != 0 ) {
        std::cerr << "getaddrinfo: " << gai_strerror( rv ) << std::endl;
        exit( EXIT_FAILURE );
    }

    if( ( sock = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 ) {
        std::cerr << "Socket bind failure" << std::endl;
        exit( EXIT_FAILURE );
    }

    if( connect( sock, servinfo->ai_addr, servinfo->ai_addrlen ) != 0 ) {
        std::cerr << "Unable to connect to server" << std::endl;
        exit( EXIT_FAILURE );
    }

    // recv transmission size, followed by transmission
    int playerBufSize;
    avalon::network::Player currNetPlayer;

    recv( sock, (char*) &playerBufSize, sizeof( int ), 0 );

    std::cout << "Receiving: " << playerBufSize << std::endl;

    char* playerBuf = new char[ playerBufSize ];
    recv( sock, playerBuf, playerBufSize * sizeof( char ), 0);

    currNetPlayer.ParseFromArray( playerBuf, playerBufSize );
    Player currPlayer( currNetPlayer );

    model->updateData< Player >( "player1", currPlayer );

    #ifdef _WIN32
        WSACleanup( );
    #endif

    exit( EXIT_SUCCESS );
}
