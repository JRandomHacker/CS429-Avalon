#include "serverControllerState.hpp"
#include "waitingForClientsState.hpp"
#include "teamSelectionState.hpp"
#include "serverInfo.hpp"
#include "serverCustomActions.hpp"

#include "teamselection.pb.h"
#include "vote.pb.h"
#include "voteresults.pb.h"

#ifdef _WIN32
    #include <winsock2.h>
#else
    #include <netdb.h>
#endif

namespace avalon {
namespace server {

WaitingForClientsState::WaitingForClientsState( ServInfo* mod ) : ServerControllerState( "WaitingForClients", mod ) { }

WaitingForClientsState::~WaitingForClientsState( ) { }

ServerControllerState* WaitingForClientsState::handleAction( Action* action_to_be_handled ) {

    std::string action_type = action_to_be_handled->getMessage();

    // Someone is connecting
    if( action_type == "NewPlayer" ) {

        auto action = dynamic_cast< NewPlayerAction* >( action_to_be_handled );
        unsigned int playerID = action->getPlayerID( );

        // See if they requested a custom name
        std::string requestedName = action->getPlayerName( );
        if( !requestedName.empty( ) ) {
            model->players[ playerID ]->setName( requestedName );
        }

        sendStartingInfo( playerID );

    // Everyone is connected
    } else if( action_type == "EnterTeamSelection" ) {

        model->server->broadcastStateChange( avalon::network::ENTER_TEAM_SELECTION_BUF, model->leader );

        return new TeamSelectionState( model );
    } else {
        reportUnhandledAction( action_type );
    }

    // We haven't changed states
    return NULL;
}


// Sends the beginning information when a player connects
void WaitingForClientsState::sendStartingInfo( unsigned int playerID ) {

    model->settingsBuf.set_client( playerID );
    model->server->sendProtobuf( avalon::network::SETTINGS_BUF, playerID, model->settingsBuf.SerializeAsString( ) );

    // Trade the information the player should know with each already connected player
    for( unsigned int i = 0; i < playerID; i++ ) {

        sendRelevantInfo( i, playerID );

        sendRelevantInfo( playerID, i );
    }

    sendPlayer( playerID, playerID, ALLINFO ); // Send the new player their own info
}

void WaitingForClientsState::sendRelevantInfo( unsigned int player, unsigned int recipient ) {

    bool sent = false;

    // Merlin should know of all the evil players who aren't Mordred
    if( model->players[ recipient ]->getRole( ) == avalon::MERLIN ) {
        if( model->players[ player ]->getAlignment( ) == avalon::EVIL && model->players[ player ]->getRole( ) != avalon::MORDRED ) {
            sent = true;
            sendPlayer( player, recipient, ALIGNMENTINFO );
        }
    }

    // Evil players, who aren't Oberon, should know of each other
    if( model->players[ recipient ]->getAlignment( ) == avalon::EVIL && model->players[ player ]->getAlignment( ) == avalon::EVIL ) {
        if( model->players[ recipient ]->getRole( ) != avalon::OBERON && model->players[ player ]->getRole( ) != avalon::OBERON ) {
            sent = true;
            sendPlayer( player, recipient, ALIGNMENTINFO );
        }
    }

    // Percival should know of Merlin and Morgana
    if( model->players[ recipient ]->getRole( ) == avalon::PERCIVAL ) {
        if( model->players[ player ]->getRole( ) == avalon::MERLIN || model->players[ player ]->getRole( ) == avalon::MORGANA ) {
            sent = true;
            sendPlayer( player, recipient, LOOKSLIKEMERLININFO );
        }
    }

    // No special rules, so they shouldn't know anything
    if( !sent ) {
        sendPlayer( player, recipient, NOINFO );
    }
}

} // server
} // avalon
