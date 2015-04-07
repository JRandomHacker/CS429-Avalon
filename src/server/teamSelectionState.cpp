#include <string>

#include "serverControllerState.hpp"
#include "teamSelectionState.hpp"
#include "teamVotingState.hpp"
#include "serverInfo.hpp"
#include "serverCustomActions.hpp"

#include "teamselection.pb.h"
#include "vote.pb.h"

#ifdef _WIN32
    #include <winsock2.h>
#else
    #include <netdb.h>
#endif

namespace avalon {
namespace server {

    TeamSelectionState::TeamSelectionState( ServInfo* mod ) : ServerControllerState( "TeamSelection", mod ) { }

    TeamSelectionState::~TeamSelectionState( ) {
        model->team.clear( );
    }

    ServerControllerState* TeamSelectionState::handleAction( Action* action_to_be_handled ) {

        std::string action_type = action_to_be_handled->getMessage();

        // We received a new team member, or are removing one
        if( action_type == "ToggleTeamMember" ) {

            auto action = dynamic_cast< ToggleTeamMemberAction* >( action_to_be_handled );
            unsigned int selector = action->getSelectorID( );

            // Make sure the player is allowed to take this action
            if( selector == model->leader ) {

                unsigned int player = action->getPlayerID( );
                bool selected = toggleSelection( player );

                // Set up the protobuf to send to all the clients
                avalon::network::TeamSelection buf;
                buf.set_id( player );
                buf.set_selected( selected );
                std::string message = buf.SerializeAsString( );

                // Send the protobuf to all the clients
                sendProtobufToAll( avalon::network::TEAM_SELECTION_BUF, buf.SerializeAsString( ) );
            } else {
                std::cerr << "[ ServerController ] Received a team selection from someone who isn't the leader" << std::endl;
            }

        // The leader is happy with the proposed team, and wants to put it on a vote
        // Makes sure that there aren't too many selections here, rather than during proposal
        } else if( action_type == "ConfirmTeamSelection" ) {

            auto action = dynamic_cast< ConfirmTeamSelectionAction* >( action_to_be_handled );
            unsigned int selector = action->getSelectorID( );

            // Make sure the player is allowed to take this action
            if( selector == model->leader ) {

                // TODO Add logic to make sure there aren't too many people
                model->server->broadcastStateChange( avalon::network::ENTER_TEAM_VOTE_BUF, 0 );
                return new TeamVotingState( model );
            } else {
                std::cerr << "[ ServerController ] Received a team confirmation from someone who isn't the leader" << std::endl;
            }

        } else if( action_type == "ChatMessageRecv" ) {

            auto action = dynamic_cast< ChatMessageRecvAction* >( action_to_be_handled );
            unsigned int sender = action->getMessage( ).getSenderId( );
            std::string message_text = action->getMessage( ).getMessageText( );
            unsigned int time = action->getMessage( ).getTimestamp( );

            avalon::network::ChatMessage buf;
            buf.set_sender_id( sender );
            buf.set_message_text( message_text );
            buf.set_timestamp( time );
            sendProtobufToAll( avalon::network::CHAT_MSG_BUF, buf.SerializeAsString( ) );
        } else {
            reportUnhandledAction( action_type );
        }

        // We haven't changed states
        return NULL;
    }

    bool TeamSelectionState::toggleSelection( unsigned int player_id ) {
        bool wasSelected = false;

        // Look through the vector to see if the player is currently selected
        for( unsigned int i = 0; i < model->team.size( ); i++ ) {
            if( model->team[ i ] == player_id ) {
                model->team.erase( model->team.begin( ) + i );
                wasSelected = true;
            }
        }

        // If they weren't selected, select them
        if( !wasSelected ) {
            model->team.push_back( player_id );
        }

        // If they hadn't been selected, they are now
        return !wasSelected;
    }

} // server
} // avalon
