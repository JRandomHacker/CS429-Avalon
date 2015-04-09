/*
 * Implementation of client class
 *
 * @file client.cpp
 * @author Ryan Kerr && Justin Koehler
 * @date 2015-03-03
 */

#include "client.hpp"
#include "player.hpp"
#include "globals.hpp"
#include <string>
#include <iostream>

#include "player.pb.h"
#include "settings.pb.h"
#include "teamselection.pb.h"
#include "vote.pb.h"
#include "teamvoteresults.pb.h"
#include "questvoteresults.pb.h"

#ifdef _WIN32
    #include <ws2tcpip.h>
#endif

// Constructor
Client::Client( ) {
    queue = NULL;
    network_initialized = false;
}

// Destructor
Client::~Client( ) {
    // Required socket cleanup in Windows
    #ifdef _WIN32
        if( network_initialized ) {
            WSACleanup( );
        }
    #endif
}

// Allows the initialization of a queue after the client has successfully connected to the server
void Client::initQueue( ActionHandler* new_queue ) {

    if( !queue ) {
        queue = new_queue;
    }
    else {
        std::cerr << "[ CLIENT ] Attempted to add a second ActionHandler" << std::endl;
    }
}

// Sets up the clients network connection
int Client::initClient( std::string host, int port ) {

    if( network_initialized ) {
        std::cerr << "[ CLIENT ] Attempting to connect to a second server" << std::endl;
        return EXIT_NETWORK_ERROR;
    }

    // Network initialization
    struct addrinfo hints, *servinfo;
    #ifdef _WIN32
        WSADATA wsaData;
        if( WSAStartup( WINSOCK_MAGIC, &wsaData ) != 0 ) {
            std::cerr << "[ CLIENT ] Unable to magic windows" << std::endl;
            return EXIT_NETWORK_ERROR;
        }
    #endif

    // Make sure there isn't any random data in the addrinfo before setting what we want
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = 0;

    // Populate servinfo with the proper addrinfo->ai_addr to be able to connect later
    if( getaddrinfo( host.c_str( ), std::to_string( port ).c_str(), &hints, &servinfo ) != 0 ) {
        std::cerr << "[ CLIENT ] Unable to get address info" << std::endl;
        return EXIT_SOCKET_ERROR;
    }

    // Open a new socket on the system
    if( ( sock = socket( AF_INET, SOCK_STREAM, 0 ) ) == INVALID_SOCKET ) {
        std::cerr << "[ CLIENT ] Socket bind failure" << std::endl;
        return EXIT_SOCKET_ERROR;
    }

    // Connect the socket to the server
    if( connect( sock, servinfo->ai_addr, servinfo->ai_addrlen ) != 0 ) {
        std::cerr << "[ CLIENT ] Unable to connect to server" << std::endl;
        return EXIT_SOCKET_ERROR;
    }

    network_initialized = true;

    return EXIT_SUCCESS;
}

// Function to wait for another packet from the server
// Receives the information about the next message,
// then delegates to helper functions
void Client::waitForData( ) {

    if( !network_initialized ) {
        std::cerr << "[ CLIENT ] Attempting to wait for data without connecting to a server" << std::endl;
        return;
    }

    // Receive the type of protobuf we're going to be working with
    int bufType;
    int size = recv( sock, (char*) &bufType, sizeof( int ), 0 );

    // Less than equal, since on Linux an error will return 0, and on Windows an error will return -1
    if(size <= 0) {
        std::cerr << "[ CLIENT ] Network recv error" << std::endl;
        exit( EXIT_NETWORK_ERROR );
    }

    // Receive the length of the protobuf we're looking for
    unsigned int bufLength;
    recv( sock, (char*) &bufLength, sizeof( int ), 0 );

    // Call the relevant helper method, depending on the type of the protobuf
    switch ( bufType ){

        case avalon::network::PLAYER_BUF:
            recvPlayer( bufLength );
            break;

        case avalon::network::SETTINGS_BUF:
            recvSettings( bufLength );
            break;

        case avalon::network::CHAT_MSG_BUF:
            recvMessage( bufLength );
            break;

        case avalon::network::TEAM_VOTE_BUF:
            recvTeamVote( bufLength );
            break;

        case avalon::network::QUEST_VOTE_BUF:
            recvQuestVote( bufLength );
            break;

        case avalon::network::QUEST_VOTE_RESULTS_BUF:
            recvQuestVoteResults( bufLength );
            break;

        case avalon::network::TEAM_VOTE_RESULTS_BUF:
            recvTeamVoteResults( bufLength );
            break;

        case avalon::network::TEAM_SELECTION_BUF:
            recvTeamSelection( bufLength );
            break;

        case avalon::network::ENTER_TEAM_SELECTION_BUF:
        case avalon::network::ENTER_TEAM_VOTE_BUF:
        case avalon::network::ENTER_QUEST_VOTE_BUF:
        case avalon::network::ENTER_END_GAME_BUF:
            recvStateChange( bufType, bufLength );
            break;

        default:
            std::cerr << "[ CLIENT ] Received an unknown type of protobuf" << std::endl;
            break;
    }
}

// Helper function to receive a player protobuf
void Client::recvPlayer( unsigned int bufLength ) {

    // Receive the protobuf
    char* playerBuf = new char[ bufLength ];
    recv( sock, playerBuf, bufLength * sizeof( char ), 0);

    // Create a new player object
    avalon::network::Player pBuf;
    pBuf.ParseFromArray( playerBuf, bufLength );
    Player* p = new Player( pBuf );

    // Add an action to the queue
    Action* action = new AddPlayerAction( pBuf.id(), p );
    queue->addAction( action );

    delete[] playerBuf;
}

// Helper function to receive a ChatMessage protobuf
void Client::recvMessage( unsigned int bufLength ) {

    // Receive the protobuf
    char* chatBuf = new char[ bufLength ];
    recv( sock, chatBuf, bufLength * sizeof( char ), 0);

    avalon::network::ChatMessage buf;
    buf.ParseFromArray( chatBuf, bufLength );

    // Add an action to the queue
    Action* action = new ChatMessageRecvAction( buf.message_text( ), buf.sender_id( ) );
    queue->addAction( action );

    delete[] chatBuf;
}

// Helper function to receive a TeamSelection protobuf
void Client::recvTeamSelection( unsigned int bufLength ) {

    // Receive the protobuf
    char* selectionBuf = new char[ bufLength ];
    recv( sock, selectionBuf, bufLength * sizeof( char ), 0);

    avalon::network::TeamSelection buf;
    buf.ParseFromArray( selectionBuf, bufLength );

    // Add an action to the queue
    Action* action = new ModifyTeamSelectionAction( buf.id(), buf.selected( ) );
    queue->addAction( action );

    delete[] selectionBuf;
}

// Helper function to receive a Vote protobuf
void Client::recvTeamVote( unsigned int bufLength ) {

    // Receive the protobuf
    char* voteBuf = new char[ bufLength ];
    recv( sock, voteBuf, bufLength * sizeof( char ), 0);

    avalon::network::Vote buf;
    buf.ParseFromArray( voteBuf, bufLength );

    // Add an action to the queue
    Action* action = new ReceiveTeamVoteAction( buf.id( ) );
    queue->addAction( action );

    delete[] voteBuf;
}

// Helper function to receive a Vote protobuf
void Client::recvQuestVote( unsigned int bufLength ) {

    // Receive the protobuf
    char* voteBuf = new char[ bufLength ];
    recv( sock, voteBuf, bufLength * sizeof( char ), 0);

    avalon::network::Vote buf;
    buf.ParseFromArray( voteBuf, bufLength );

    // Add an action to the queue
    Action* action = new ReceiveQuestVoteAction( buf.id( ) );
    queue->addAction( action );

    delete[] voteBuf;
}

// Helper function to receive a GameSettings protobuf
void Client::recvSettings( unsigned int bufLength ) {

    // Receive the game settings
    char* settingsBuf = new char[ bufLength ];
    recv( sock, settingsBuf, bufLength * sizeof( char ), 0);

    // Create a local copy of the GameSettings protobuf
    avalon::network::GameSettings* sBuf = new avalon::network::GameSettings( );
    sBuf->ParseFromArray( settingsBuf, bufLength );

    // Add an action to the queue
    Action* action = new GameSettingsAction( sBuf );
    queue->addAction( action );

    delete[] settingsBuf;
}

// Helper function to receive vote results
void Client::recvTeamVoteResults( unsigned int bufLength ) {

    // Receive the game settings
    char* voteBuf = new char[ bufLength ];
    recv( sock, voteBuf, bufLength * sizeof( char ), 0);

    // Create a local copy of the GameSettings protobuf
    avalon::network::VoteResults buf;
    buf.ParseFromArray( voteBuf, bufLength );
    bool votePassed = buf.passed( );
    unsigned int voteTrack = buf.vote_track( );

    std::vector< avalon::player_vote_t > votes;
    for( auto it = buf.votes( ).begin( ); it != buf.votes( ).end( ); it++ ) {
        votes.push_back( ( avalon::player_vote_t )( *it ) );
    }

    // Add an action to the queue
    Action* action = new TeamVoteResultsAction( votePassed, voteTrack, votes );
    queue->addAction( action );

    delete[] voteBuf;
}

// Helper function to receive vote results
void Client::recvQuestVoteResults( unsigned int bufLength ) {

    // Receive the game settings
    char* voteBuf = new char[ bufLength ];
    recv( sock, voteBuf, bufLength * sizeof( char ), 0);

    // Create a local copy of the GameSettings protobuf
    avalon::network::QuestVoteResults buf;
    buf.ParseFromArray( voteBuf, bufLength );
    bool questVotePassed = buf.passed( );
    unsigned int questVoteTrack = buf.quest_track( );

    std::vector< avalon::player_vote_t > votes;
    for( auto it = buf.votes( ).begin( ); it != buf.votes( ).end( ); it++ ) {
        votes.push_back( ( avalon::player_vote_t )( *it ) );
    }

    // Add an action to the queue
    Action* action = new QuestVoteResultsAction( questVotePassed, questVoteTrack, votes );
    queue->addAction( action );

    delete[] voteBuf;
}

// Helper function for switching states
void Client::recvStateChange( int bufType, unsigned int randomness ) {

    Action* action = NULL;
    switch( bufType ) {

        case avalon::network::ENTER_TEAM_SELECTION_BUF:
            action = new EnterTeamSelectionAction( randomness );
            break;

        case avalon::network::ENTER_TEAM_VOTE_BUF:
            action = new EnterTeamVoteStateAction( );
            break;

        case avalon::network::ENTER_QUEST_VOTE_BUF:
            action = new EnterQuestVoteStateAction( );
            break;

        default:
            std::cerr << "Attempted to parse a state change from a not state change" << std::endl;
            break;
    }

    if( action ) {
        queue->addAction( action );
    }
}

void Client::sendProtobuf( avalon::network::buffers_t bufType, std::string message ) {

    int messageSize = message.length( );

    // Windows REQUIRES that sockets send char* rather than void*... so trick it
    send( sock, (char*)(&bufType), sizeof( avalon::network::buffers_t ) / sizeof( char ), 0 );
    send( sock, (char*)(&messageSize), sizeof( int ) / sizeof( char ), 0 );
    send( sock, message.c_str( ), messageSize * sizeof( char ), 0 );
}
