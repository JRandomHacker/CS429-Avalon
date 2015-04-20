#include <string>

#include "serverControllerState.hpp"
#include "waitingForClientsState.hpp"
#include "teamSelectionState.hpp"
#include "serverInfo.hpp"
#include "serverCustomActions.hpp"

#include "teamselection.pb.h"
#include "vote.pb.h"

namespace avalon {
namespace server {

// ServerControllerState {
    ServerControllerState::ServerControllerState( std::string state_type_desc, ServInfo* mod )
        : ControllerState(state_type_desc), model( mod ) {
            std::cerr << "[ ServerController ] Entered " << state_type_desc << " state" << std::endl;
    }

    ControllerState* ServerControllerState::handleAction( Action* action_to_be_handled ) {

        std::string action_type = action_to_be_handled->getMessage( );

        if( action_type == "ChatMessageRecv" ) {

            auto action = dynamic_cast< ChatMessageRecvAction* >( action_to_be_handled );
            unsigned int sender = action->getMessage( ).getSenderId( );
            std::string message_text = action->getMessage( ).getMessageText( );
            unsigned int time = action->getMessage( ).getTimestamp( );

            avalon::network::ChatMessage buf;
            buf.set_sender_id( sender );
            buf.set_message_text( message_text );
            buf.set_timestamp( time );
            sendProtobufToAll( avalon::network::CHAT_MSG_BUF, buf.SerializeAsString( ) );

            return NULL;
        }

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

    // Checks the game state to decide if the evil players have currently won
    bool ServerControllerState::badGuysWon( ) {

        return ServerControllerState::badGuysWon( model );
    }

    // Checks the game state to decide if the evil players have currently won
    bool ServerControllerState::goodGuysWon( ) {

        return ServerControllerState::goodGuysWon( model );
    }

    // Checks the game state to decide if the evil players have currently won
    bool ServerControllerState::badGuysWon( ServInfo* model ) {

        bool badWins = false;

        if( model->quests_failed >= ( ( model->quest_track_length / 2 ) + 1 ) ) {
            badWins = true;
        } else if ( model->vote_track >= model->vote_track_length ) {
            badWins = true;
        }

        return badWins;
    }

    // Checks the game state to decide if the good players have currently won (sans assassin)
    bool ServerControllerState::goodGuysWon( ServInfo* model ) {

        bool goodWins = false;

        unsigned int quests_passed = model->quest_track - model->quests_failed;

        if( quests_passed >= ( ( model->quest_track_length / 2 ) + 1 ) ) {
            goodWins = true;
        } else if ( model->quest_track >= model->quest_track_length ) {
            goodWins = true;
        }

        return goodWins;
    }
// }

} // server
} // avalon
