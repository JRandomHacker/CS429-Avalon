#include <string>

#include "assassinState.hpp"
#include "endGameState.hpp"
#include "serverCustomActions.hpp"

namespace avalon {
namespace server {

    AssassinState::AssassinState( ServInfo* mod ) : ServerControllerState( "Assassin", mod ) { }

    AssassinState::~AssassinState( ) { }

    ControllerState* AssassinState::handleAction( Action* action_to_be_handled ) {

        std::string action_type = action_to_be_handled->getMessage();

        if( action_type == "AssassinTargetSelection" ) {

            auto action = dynamic_cast< AssassinTargetSelectionAction* >( action_to_be_handled );
            unsigned int selector = action->getSelectorID( );
            unsigned int selection = action->getPlayerID( );
            bool correct = false;

            if( model->players[ selector ]->getRole( ) == avalon::ASSASSIN ) {
                if( model->players[ selection ]->getRole( ) == avalon::MERLIN ) {
                    correct = true;
                }

                sendProtobufToAll( avalon::network::ASSASSIN_TARGET_BUF, model->players[ selection ]->getBuf( ).SerializeAsString( ) );

                avalon::alignment_t winner;
                if( correct ) {
                    winner = avalon::EVIL;
                } else {
                    winner = avalon::GOOD;
                }

                model->server->broadcastStateChange( avalon::network::ENTER_END_GAME_BUF, 0 );
                return new EndGameState( model, winner );
            }
        } else {
            return ControllerState::handleAction( action_to_be_handled );
        }
        return NULL;
    }

} // server
} // avalon

