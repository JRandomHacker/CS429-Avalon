#include <string>

#include "finalGameState.hpp"
#include "serverCustomActions.hpp"

#include "endgame.pb.h"

namespace avalon {
namespace server {

    FinalGameState::FinalGameState( ServInfo* mod, avalon::alignment_t winner ) : ServerControllerState( "FinalGame", mod ) { 

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

    FinalGameState::~FinalGameState( ) { }

    ControllerState* FinalGameState::handleAction( Action* action_to_be_handled ) {

        std::string action_type = action_to_be_handled->getMessage();

        if( action_type == "ConfirmEndGame" ) {

            std::cout << "Cool" << std::endl;
        }
        return NULL;
    }

} // server
} // avalon

