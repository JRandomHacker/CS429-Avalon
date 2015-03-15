#include "serverControllerState.hpp"
#include "serverInfo.hpp"
#include "serverCustomActions.hpp"

#include "teamselection.pb.h"

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

    // Sends a protobuf to all the clients
    void ServerControllerState::sendProtobufToAll( avalon::network::buffers_t bufType, std::string message ) {

        for( unsigned int i = 0; i < model->num_clients; i++ ) {
            model->server->sendProtobuf( bufType, i, message );
        }
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

        if( action_type == "ToggleTeamMember" ) {
            auto action = dynamic_cast< ToggleTeamMemberAction* >( action_to_be_handled );
            unsigned int selector = action->getSelectorID( );
            if( selector == model->leader ) {

                unsigned int player = action->getPlayerID( );
                bool selected = toggleSelection( player );

                // Set up the protobuf to send to all the clients
                avalon::network::TeamSelection buf;
                buf.set_id( player );
                buf.set_selected( selected );
                std::string message = buf.SerializeAsString( );

                // Send the protobuf to all the clients
                sendProtobufToAll( avalon::network::TEAM_SELECTION_BUF, buf.SerializeAsString( ) );
            } else {
                std::cerr << "[ ServerController ] Received a team selection from someone who isn't the leader" << std::endl;
            }

        } else if( action_type == "ConfirmTeamSelection" ) {

            auto action = dynamic_cast< ConfirmTeamSelectionAction* >( action_to_be_handled );
            unsigned int selector = action->getSelectorID( );
            if( selector == model->leader ) {
                // TODO Switch states and send a message to the clients
            } else {
                std::cerr << "[ ServerController ] Received a team confirmation from someone who isn't the leader" << std::endl;
            }

        } else {
            reportUnhandledAction( action_type );
        }

        return NULL;
    }

    bool TeamSelectionState::toggleSelection( unsigned int player_id ) {
        bool wasSelected = false;

        // Look through the vector to see if the player is currently selected
        for( unsigned int i = 0; i < model->team.size( ); i++ ) {
            if( model->team[ i ] == player_id ) {
                model->team.erase( model->team.begin( ) + i );
                wasSelected = true;
            }
        }

        // If they weren't selected, select them
        if( !wasSelected ) {
            model->team.push_back( player_id );
        }

        // If they hadn't been selected, they are now
        return !wasSelected;
    }
// }
