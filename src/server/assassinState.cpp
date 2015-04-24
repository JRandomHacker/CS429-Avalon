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

                avalon::network::Player playerBuf = model->players[ selection ]->getBuf( );
                playerBuf.set_id( selection );
                sendProtobufToAll( avalon::network::ASSASSIN_TARGET_BUF, playerBuf.SerializeAsString( ) );

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
            return ServerControllerState::handleAction( action_to_be_handled );
        }
        return NULL;
    }

} // server
} // avalon

