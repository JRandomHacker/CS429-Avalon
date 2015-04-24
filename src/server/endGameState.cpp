#include <string>

#include "endGameState.hpp"
#include "serverCustomActions.hpp"

#include "endgame.pb.h"

namespace avalon {
namespace server {

    EndGameState::EndGameState( ServInfo* mod, avalon::alignment_t winner ) : ServerControllerState( "EndGame", mod ) { 

        avalon::network::EndGame buf;
        buf.set_winner( winner );

        for( Player* p : model->players ) {
            avalon::network::Player data = p->getBuf( );
            avalon::network::Player* next = buf.add_players( );

            next->set_role( data.role( ) );
            next->set_alignment( data.alignment( ) );
            next->set_name( data.name( ) );
            next->set_id( data.id( ) );
        }

        sendProtobufToAll( avalon::network::END_GAME_INFO_BUF, buf.SerializeAsString( ) );
    }

    EndGameState::~EndGameState( ) { }

    ControllerState* EndGameState::handleAction( Action* action_to_be_handled ) {

        std::string action_type = action_to_be_handled->getMessage();

        if( action_type == "ConfirmEndGame" ) {

            std::cout << "Cool" << std::endl;
            model->server->broadcastStateChange( avalon::network::SHUTDOWN_BUF, 0 );
            exit( EXIT_SUCCESS );
        } else {
            return ServerControllerState::handleAction( action_to_be_handled );
        }
        return NULL;
    }

} // server
} // avalon

