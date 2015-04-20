#include <string>

#include "finalGameState.hpp"
#include "serverCustomActions.hpp"

namespace avalon {
namespace server {

    FinalGameState::FinalGameState( ServInfo* mod ) : ServerControllerState( "FinalGame", mod ) { }

    FinalGameState::~FinalGameState( ) { }

    ControllerState* FinalGameState::handleAction( Action* action_to_be_handled ) {

        std::string action_type = action_to_be_handled->getMessage();

        if( action_type == "" ) {

        }
        return NULL;
    }

} // server
} // avalon

