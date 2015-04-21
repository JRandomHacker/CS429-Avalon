/**
 * Implementation of the AssassinState class
 *
 * @file assassinState.cpp
 * @author Ryan Kerr && Justin Koehler
 * @date 2015-04-20
 */

#include <string>

#include "assassinState.hpp"
#include "endGameState.hpp"
#include "clientCustomActionsFromGUI.hpp"
#include "clientCustomActionsFromNet.hpp"

#include "chatmessage.pb.h"

namespace avalon {
namespace client {

    // Constructor for the AssassinState, simply sets the correct state name
    AssassinState::AssassinState( ClientInfo* dat ) : ClientControllerState( "Assassin", dat ) { }

    // Figures out what needs to be done with any given action, when we're in the voting state
    ControllerState* AssassinState::handleAction( Action* action_to_be_handled ) {

        std::string action_type = action_to_be_handled->getMessage( );

        if( action_type == "AssassinTargetSelection" ) {

            auto action = dynamic_cast< AssassinTargetSelectionAction* >( action_to_be_handled );

            avalon::network::Player buf;
            buf.set_id( action->getSelectionId( ) );
            buf.set_role( avalon::MERLIN );
            buf.set_name( "Myrddin Wyllt" );
            buf.set_alignment( avalon::GOOD );

            data->client->sendProtobuf( avalon::network::ASSASSIN_TARGET_BUF, buf.SerializeAsString( ) );

        } else if( action_type == "AssassinSelected" ) {
            
            auto action = dynamic_cast< AssassinSelectedAction* >( action_to_be_handled );

            data->model->updateData( "assassinTargeted", action->getAssassinationTarget( ) );

        } else if( action_type == "EnterEndGameState" ) {
          return new EndGameState( data );  
        } else {
            return ClientControllerState::handleAction( action_to_be_handled );
        }

        // By default, we don't change states
        return NULL;
    }

    void AssassinState::setupState( ) {
        data->model->updateData( "assassinState", true );
    }

} // client
} // avalon

