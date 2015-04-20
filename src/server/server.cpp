/*
 * Implementation of server class
 *
 * @file server.cpp
 * @author Ryan Kerr && Justin Koehler
 * @date 2015-02-16
 */

#include "server.hpp"
#include "player.hpp"
#include "globals.hpp"
#include "actionHandler.hpp"
#include "serverCustomActions.hpp"

#include "player.pb.h"
#include "settings.pb.h"
#include "teamselection.pb.h"
#include "vote.pb.h"

#include <iostream>

#ifdef _WIN32
    #include <winsock2.h>
    //! A constant that needs to be passed into Windows socket initialization
    #define WINSOCK_MAGIC 0x202
#else
    #include <netdb.h>
    #include <sys/select.h>
    //! Allows us to use the SOCKET type in both Windows and Linux
    #define SOCKET int
#endif

// Initializes the server to start listening for connections
int Server::initServer( ) {

    if( port > 65536 || port < 0 )
    {
        std::cerr << "[ SERVER ] Invalid port number" << std::endl;
        return EXIT_NETWORK_ERROR;
    }
    // Windows requires some initial socket setup
    #ifdef _WIN32
        WSADATA wsaData;
        if( WSAStartup( WINSOCK_MAGIC, &wsaData ) != 0 ) {
            std::cerr << "[ SERVER ] Unable to magic windows" << std::endl;
            return EXIT_NETWORK_ERROR;
        }
    #endif

    // Create a system socket
    servsock = socket( AF_INET, SOCK_STREAM, 0 );

    memset( &servparm, 0, sizeof( struct sockaddr_in ) );
    servparm.sin_family = AF_INET;
    servparm.sin_port = htons( port ); // Port to listen on
    servparm.sin_addr.s_addr = htonl( INADDR_ANY ); // INET addresses to listen to

    // Bind the socket with our servparm settings
    if( bind( servsock, ( struct sockaddr* )( &servparm ), sizeof( struct sockaddr_in ) ) < 0 ) {
        std::cerr << "[ SERVER ] Unable to bind network socket" << std::endl;
        return EXIT_SOCKET_ERROR;
    }

    // Start listening for connections
    if( listen( servsock, SOMAXCONN ) < 0 ) {
        std::cerr << "[ SERVER ] Unable to listen on specified port" << std::endl;
        return EXIT_SOCKET_ERROR;
    }

    return EXIT_SUCCESS;
}

void Server::initQueue( ActionHandler* queue ) {
    if( !(this->queue) ) {
        this->queue = queue;
    } else {
        std::cerr << "[ SERVER ] Attempted to add a second ActionHandler" << std::endl;
    }
}

// Constructor
Server::Server( int port ) {

    this->port = port;
    this->queue = NULL;
    this->clients_connected = false;

}

// Destructor
Server::~Server( ) {

    // Windows requires additional socket teardown
    #ifdef _WIN32
        WSACleanup( );
    #endif
}

// Selects across the sockets to find one ready to read
// Should then add an action to the Queue
void Server::waitForData( ) {
    if( !clients_connected ) {
        std::cerr << "[ SERVER ] Attempted to wait for data before connecting to clients" << std::endl;
        return;
    }

    // Set up the fd_set to select across
    fd_set clients;
    FD_ZERO( &clients );
    int fdnum = 0;
    for( std::vector< SOCKET >::iterator it = sockets.begin(); it != sockets.end(); it++ ) {
        FD_SET( *it, &clients );

        #ifndef _WIN32
            if( *it > fdnum ) {
                fdnum = *it;
            }
        #endif
    }
    fdnum++;

    select( fdnum, &clients, NULL, NULL, NULL );

    // Find the sockets that had data
    for( std::vector< SOCKET >::iterator it = sockets.begin(); it != sockets.end(); it++ ) {
        if( FD_ISSET( *it, &clients ) ) {
            recvData( *it );
        }

    }
}

void Server::recvData( SOCKET recvSock ) {

    // Receive the type of protobuf we're waiting for
    int bufType;
    int size = recv( recvSock, ( char* )&bufType, sizeof( int ), 0 );

    // Make sure there wasn't a receive error
    if( size <= 0 ) {
        std::cerr << "[ SERVER ] Network recv error" << std::endl;
        exit( EXIT_NETWORK_ERROR );
    }

    // Receive the length of the protobuf we're waiting for
    int bufLength;
    recv( recvSock, ( char* )&bufLength, sizeof( int ), 0 );

    switch( bufType ) {

        case avalon::network::TEAM_SELECTION_BUF:
            recvTeamSelection( recvSock, bufLength );
            break;

        case avalon::network::CHAT_MSG_BUF:
            recvMessage( recvSock, bufLength );
            break;

        case avalon::network::TEAM_VOTE_BUF:
            recvTeamVote( recvSock, bufLength );
            break;

        case avalon::network::QUEST_VOTE_BUF:
            recvQuestVote( recvSock, bufLength );
            break;

        case avalon::network::ENTER_FINAL_GAME_BUF:
            recvConfirmEnd( recvSock, bufLength );
            break;

        default:
            std::cerr << "[ SERVER ] Received an unknown type of protobuf" << std::endl;
            break;
    }
}

void Server::recvTeamSelection( SOCKET recvSock, int bufLength ) {

    // Receive the buffer
    char* buffer = new char[ bufLength ];
    recv( recvSock, ( char* )buffer, bufLength * sizeof( char ), 0 );

    // Parse the player
    avalon::network::TeamSelection buf;
    buf.ParseFromArray( buffer, bufLength );
    delete[] buffer;

    unsigned int selector = getIdFromSocket( recvSock );
    Action* action = NULL;

    // Check to see if they're done or not
    if( buf.has_finished( ) && buf.finished( ) ) {
        action = new ConfirmTeamSelectionAction( selector );
    } else {

        unsigned int selection = buf.id( );

        action = new ToggleTeamMemberAction( selector, selection );
    }

    queue->addAction( action );
}

void Server::recvMessage( SOCKET recvSock, int bufLength ) {

    // Receive the buffer
    char* buffer = new char[ bufLength ];
    recv( recvSock, ( char* )buffer, bufLength * sizeof( char ), 0 );

    // Parse the ChatMessage
    avalon::network::ChatMessage buf;
    buf.ParseFromArray( buffer, bufLength );
    delete[] buffer;

    avalon::common::ChatMessage recMessage( buf.sender_id( ), buf.message_text( ), buf.timestamp( ) );

    Action* action = new ChatMessageRecvAction( recMessage );
    queue->addAction( action );
}

void Server::recvConfirmEnd( SOCKET recvSock, int bufLength ) {

    unsigned int sender = getIdFromSocket( recvSock );
    Action* action = new ConfirmEndGameAction( sender );

    queue->addAction( action );
}

void Server::recvTeamVote( SOCKET recvSock, int bufLength ) {

    // Receive the buffer
    char* buffer = new char[ bufLength ];
    recv( recvSock, ( char* )buffer, bufLength * sizeof( char ), 0 );

    // Parse the vote
    avalon::network::Vote buf;
    buf.ParseFromArray( buffer, bufLength );
    delete[] buffer;

    avalon::player_vote_t vote = ( avalon::player_vote_t )buf.vote( );
    unsigned int id = getIdFromSocket( recvSock );

    Action* action = new TeamVoteAction( id, vote );
    queue->addAction( action );
}

void Server::recvQuestVote( SOCKET recvSock, int bufLength ) {

    // Receive the buffer
    char* buffer = new char[ bufLength ];
    recv( recvSock, ( char* )buffer, bufLength * sizeof( char ), 0 );

    // Parse the vote
    avalon::network::Vote buf;
    buf.ParseFromArray( buffer, bufLength );
    delete[] buffer;

    avalon::player_vote_t vote = ( avalon::player_vote_t )buf.vote( );
    unsigned int id = getIdFromSocket( recvSock );

    Action* action = new QuestVoteAction( id, vote );
    queue->addAction( action );
}

std::string Server::recvCustomName( SOCKET recvSock ) {

    // Receive the type of protobuf we're waiting for
    int bufType;
    int size = recv( recvSock, ( char* )&bufType, sizeof( int ), 0 );

    // Make sure there wasn't a receive error
    if( size <= 0 ) {
        std::cerr << "[ SERVER ] Network recv error" << std::endl;
        exit( EXIT_NETWORK_ERROR );
    }

    // Receive the length of the protobuf we're waiting for
    int bufLength;
    recv( recvSock, ( char* )&bufLength, sizeof( int ), 0 );

    // Receive the buffer
    char* buffer = new char[ bufLength ];
    recv( recvSock, ( char* )buffer, bufLength * sizeof( char ), 0 );

    // If it was the wrong type, print an error, free the buffer and return an empty string
    if( avalon::network::PLAYER_BUF != bufType ) {
        std::cerr << "[ SERVER ] Attempted to receive a custom player name, but got bufType " << bufType << std::endl;
        delete[] buffer;
        return "";
    }

    // Parse the player
    avalon::network::Player pbuf;
    pbuf.ParseFromArray( buffer, bufLength );
    Player p( pbuf );

    delete[] buffer;

    // Return the name
    return p.getName( );
}

// Wait for all the players to connect
// Adds an action to the queue for the Controller when they do
bool Server::waitForClients( unsigned int num_clients ) {

    for( unsigned int i = 0; i < num_clients; i++ ) {

        SOCKET new_sock = accept( servsock, NULL, NULL );
        sockets.push_back( new_sock );
        std::string playerName = recvCustomName( new_sock );
        Action* action = new NewPlayerAction( i, playerName );
        queue->addAction( action );
    }

    this->num_clients = num_clients;
    clients_connected = true;

    Action* action = new EnterTeamSelectionAction( );
    queue->addAction( action );
    return true;
}

// Sends an arbitrary protobuf to an arbitrary player
void Server::sendProtobuf( avalon::network::buffers_t bufType, unsigned int destinationID, std::string message ) {

    if( destinationID > sockets.size( ) ) {
        std::cerr << "[ SERVER ] Attempted to send a buffer to an invalid client ID" << std::endl;
        return;
    }
    int messageSize = message.length( );

    // Windows REQUIRES that model->sockets send char* rather than void*... so we have to do some bullshit to trick it
    send( sockets[ destinationID ], ( char* )( &bufType ), sizeof( avalon::network::buffers_t ) / sizeof( char ), 0 );
    send( sockets[ destinationID ], ( char* )( &messageSize ), sizeof( int ) / sizeof( char ), 0 );
    send( sockets[ destinationID ], message.c_str( ), messageSize * sizeof( char ), 0 );
}

void Server::broadcastStateChange( avalon::network::buffers_t bufType, unsigned int randomness ) {

    if( !clients_connected ) {
        std::cerr << "[ SERVER ] Attempted to broadcast state change before everyone was connected." << std::endl;
        return;
    }

    for( unsigned int i = 0; i < num_clients; i++ ) {
        send( sockets[ i ], ( char* )( &bufType ), sizeof( avalon::network::buffers_t ) / sizeof( char ), 0 );
        send( sockets[ i ], ( char* )( &randomness ), sizeof( int ) / sizeof( char ), 0 );
    }
}

unsigned int Server::getIdFromSocket( SOCKET sock ) {

    for( unsigned int i = 0; i < sockets.size( ); i++ ) {
        if( sock == sockets[ i ] ) {
            return i;
        }
    }

    std::cerr << "[ SERVER ] Failed to parse socket to ID" << std::endl;
    return -1;
}
