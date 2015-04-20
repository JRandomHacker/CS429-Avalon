#include <string>

#include "endGameState.hpp"
#include "serverCustomActions.hpp"

namespace avalon {
namespace server {

    EndGameState::EndGameState( ServInfo* mod ) : ServerControllerState( "EndGame", mod ) { }

    EndGameState::~EndGameState( ) { }

    ControllerState* EndGameState::handleAction( Action* action_to_be_handled ) {

        std::string action_type = action_to_be_handled->getMessage();

        if( action_type == "" ) {

        }
        return NULL;
    }

} // server
} // avalon

