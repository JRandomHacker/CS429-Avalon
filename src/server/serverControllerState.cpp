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
        : ControllerState(state_type_desc), model( mod ) { }

    // Sends one player another player's information
    void ServerControllerState::sendPlayer( int playerID, int destinationID, bool allInfo ) {

        avalon::network::Player playerBuf;

        if ( allInfo ) {
            playerBuf = model->players[ playerID ]->getBuf( );
        } else {
            playerBuf = model->players[ playerID ]->getHiddenBuf( );
        }

        playerBuf.set_id( playerID );
        model->server->sendProtobuf( avalon::network::PLAYER_BUF, destinationID, playerBuf.SerializeAsString( ) );
    }
// }

// WaitingForClientsState {
    WaitingForClientsState::WaitingForClientsState( ServInfo* mod ) : ServerControllerState( "Voting", mod ) { }

    WaitingForClientsState::~WaitingForClientsState( ) { }

    ServerControllerState* WaitingForClientsState::handleAction( Action* action_to_be_handled ) {

        std::string action_type = action_to_be_handled->getMessage();

        if( action_type == "NewPlayer" ) {
            auto action = dynamic_cast< NewPlayerAction* >( action_to_be_handled );
            unsigned int playerID = action->getPlayerID( );
            std::string requestedName = action->getPlayerName( );
            if( !requestedName.empty( ) ) {
                model->players[ playerID ]->setName( requestedName );
            }

            sendStartingInfo( playerID );
        } else if( action_type == "EnterTeamSelection" ) {
            // TODO Send a message to the clients

            return new TeamSelectionState( model );
        } else {
            reportUnhandledAction( action_type );
        }

        return NULL;
    }


    // Sends the beginning information when a player connects
    void WaitingForClientsState::sendStartingInfo( int playerID ) {

        model->settingsBuf.set_client( playerID );
        model->server->sendProtobuf( avalon::network::SETTINGS_BUF, playerID, model->settingsBuf.SerializeAsString( ) );

        for( int i = 0; i < playerID; i++ ) {
            sendPlayer( i, playerID, false ); // Send each currently connected player to the new player
            sendPlayer( playerID, i, false ); // Send the new player to each player already connected
        }

        sendPlayer( playerID, playerID, true ); // Send the new player their own info
    }
// }

// TeamSelectionState {
    TeamSelectionState::TeamSelectionState( ServInfo* mod ) : ServerControllerState( "Voting", mod ) { }

    TeamSelectionState::~TeamSelectionState( ) { }

    ServerControllerState* TeamSelectionState::handleAction( Action* action_to_be_handled ) {

        std::string action_type = action_to_be_handled->getMessage();

        if( action_type == "SelectTeamMember" ) {
            //auto action = dynamic_cast< SelectTeamMemberAction* >( action_to_be_handled );

        } else if( action_type == "CancelTeamSelection" ) {
            //auto action = dynamic_cast< CancelTeamSelectionAction* >( action_to_be_handled );

        } else if( action_type == "ConfirmTeamSelection" ) {
            //auto action = dynamic_cast< ConfirmTeamSelectionAction* >( action_to_be_handled );

        } else {
            reportUnhandledAction( action_type );
        }

        return NULL;
    }
// }
