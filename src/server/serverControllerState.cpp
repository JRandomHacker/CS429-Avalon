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

// ServerControllerState {
    ServerControllerState::ServerControllerState( std::string state_type_desc, ServInfo* mod )
        : ControllerState(state_type_desc), model( mod ) {
            std::cerr << "[ ServerController ] Entered " << state_type_desc << " state" << std::endl;
    }

    ControllerState* ServerControllerState::handleAction( Action* action_to_be_handled ) {
        return ControllerState::handleAction( action_to_be_handled );
    }

    // Sends one player another player's information.
    // If allInfo is false, it hides the players affiliation
    void ServerControllerState::sendPlayer( unsigned int playerID, unsigned int destinationID, info_level_t infoLevel ) {

        avalon::network::Player playerBuf;

        switch( infoLevel ) {

            case ALLINFO:
                playerBuf = model->players[ playerID ]->getBuf( );
                break;

            case ALIGNMENTINFO:
                playerBuf = model->players[ playerID ]->getHiddenBuf( );
                playerBuf.set_alignment( model->players[ playerID ]->getAlignment( ) );
                break;

            case LOOKSLIKEMERLININFO:
                playerBuf = model->players[ playerID ]->getHiddenBuf( );
                playerBuf.set_role( avalon::MERLIN );
                break;

            case NOINFO:
            default:
                playerBuf = model->players[ playerID ]->getHiddenBuf( );
                break;
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

} // server
} // avalon
