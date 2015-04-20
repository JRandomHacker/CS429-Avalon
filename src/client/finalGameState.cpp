/**
 * Implementation of the FinalGameState class
 *
 * @file endGameState.cpp
 * @author Ryan Kerr && Justin Koehler
 * @date 2015-04-20
 */

#include <string>

#include "finalGameState.hpp"
#include "clientCustomActionsFromGUI.hpp"
#include "clientCustomActionsFromNet.hpp"

#include "chatmessage.pb.h"

namespace avalon {
namespace client {

    // Constructor for the FinalGameState, simply sets the correct state name
    FinalGameState::FinalGameState( ClientInfo* dat ) : ClientControllerState( "FinalGame", dat ) { }

    // Figures out what needs to be done with any given action, when we're in the voting state
    ControllerState* FinalGameState::handleAction( Action* action_to_be_handled ) {

        std::string action_type = action_to_be_handled->getMessage( );

        if( action_type == "" ) {

        } else {
            return ClientControllerState::handleAction( action_to_be_handled );
        }

        // By default, we don't change states
        return NULL;
    }

} // client
} // avalon

