#include "serverControllerState.hpp"
#include "serverInfo.hpp"
#include "serverCustomActions.hpp"

#ifdef _WIN32
    #include <winsock2.h>
#else
    #include <netdb.h>
#endif

// ServerControllerState {
    ServerControllerState::ServerControllerState( std::string state_type_desc, ServInfo* mod )
        : ControllerState(state_type_desc), model( mod ) {
    }

    // Sends one player another player's information
    void ServerControllerState::sendPlayer( int playerID, int destinationID, bool allInfo ) {

        avalon::network::Player playerBuf;

        if ( allInfo ) {
            playerBuf = model->players[ playerID ]->getBuf( );
        } else {
            playerBuf = model->players[ playerID ]->getHiddenBuf( );
        }

        playerBuf.set_id( playerID );
        sendProtobuf( avalon::network::PLAYER_BUF, destinationID, playerBuf.SerializeAsString( ) );
    }

    // Sends a protobuf to a player
    void ServerControllerState::sendProtobuf( avalon::network::buffers_t bufType, int destinationID, std::string message ) {

        int messageSize = message.length( );

        // Windows REQUIRES that model->sockets send char* rather than void*... so we have to do some bullshit to trick it
        send( model->sockets[ destinationID ], (char*)(&bufType), sizeof( avalon::network::buffers_t ) / sizeof( char ), 0 );
        send( model->sockets[ destinationID ], (char*)(&messageSize), sizeof( int ) / sizeof( char ), 0 );
        send( model->sockets[ destinationID ], message.c_str( ), messageSize * sizeof( char ), 0 );
    }
// }

// WaitingForClientsState {
    WaitingForClientsState::WaitingForClientsState( ServInfo* mod ) : ServerControllerState( "Voting", mod ) {

    }

    WaitingForClientsState::~WaitingForClientsState( ) { }

    ServerControllerState* WaitingForClientsState::handleAction( Action* action_to_be_handled ) {

        std::string action_type = action_to_be_handled->getMessage();

        if( action_type == "NewPlayer" ) {
            auto action = dynamic_cast< NewPlayerAction* >( action_to_be_handled );
            unsigned int playerID = action->getPlayerID( );
            sendStartingInfo( playerID );
        } else {
            reportUnhandledAction( action_type );
        }

        return NULL;
    }


    // Sends the beginning information when a player connects
    void WaitingForClientsState::sendStartingInfo( int playerID ) {

        model->settingsBuf.set_client( playerID );
        sendProtobuf( avalon::network::SETTINGS_BUF, playerID, model->settingsBuf.SerializeAsString( ) );

        for( int i = 0; i < playerID; i++ ) {
            sendPlayer( i, playerID, false ); // Send each currently connected player to the new player
            sendPlayer( playerID, i, false ); // Send the new player to each player already connected
        }

        sendPlayer( playerID, playerID, true ); // Send the new player their own info
    }
// }
