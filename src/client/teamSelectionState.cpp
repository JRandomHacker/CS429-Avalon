#include "teamSelectionState.hpp"
#include "votingState.hpp"
#include "clientCustomActionsFromGUI.hpp"
#include "clientCustomActionsFromNet.hpp"
#include "clientCustomActionsForChat.hpp"
#include "clientInfo.hpp"
#include "voteHistory.hpp"
#include "chat_message.hpp"

#include "teamselection.pb.h"
#include "vote.pb.h"
#include "chatmessage.pb.h"
#include <string>

namespace avalon {
namespace client {

    // Constructor for the TeamSelectionState, simply sets the correct state name
    TeamSelectionState::TeamSelectionState( ClientInfo* dat ) : ClientControllerState( "TeamSelection", dat ) { }

    // Figures out what needs to be done with any given action, when we're in the voting state
    ControllerState* TeamSelectionState::handleAction( Action* action_to_be_handled ) {

        std::string action_type = action_to_be_handled->getMessage( );

        if( action_type == "SelectQuestGoer" ) {
            if( FROMMODEL( unsigned int, "leaderID" ) == FROMMODEL( unsigned int, "myID" ) ) {

                auto action = dynamic_cast< SelectQuestGoerAction* >( action_to_be_handled );
                avalon::network::TeamSelection buf;
                buf.set_selected( action->getSelected( ) );
                buf.set_id( action->getSelectedPlayer( ) );

                data->client->sendProtobuf( avalon::network::TEAM_SELECTION_BUF, buf.SerializeAsString( ) );
            } else {
                std::cerr << "[ ClientController ] You attempted to select a quest goer, but you're not the leader. Asshole." << std::endl;
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

            // Display stuff here
        } else if( action_type == "ModifyTeamSelection" ) {

            auto action = dynamic_cast< ModifyTeamSelectionAction* >( action_to_be_handled );

            unsigned int player = action->getSelection( );
            bool added = action->getSelected( );
            bool changed = addOrRemoveTeamMember( player, added );

            if ( changed ) {
                data->model->flagDataForUpdate( "questingTeam" );
            }

        } else if( action_type == "FinalizeTeam" ) {
            if( FROMMODEL( unsigned int, "leaderID" ) == FROMMODEL( unsigned int, "myID" ) ) {

                avalon::network::TeamSelection buf;
                buf.set_selected( true );
                buf.set_id( ( int ) 1189998819991197253 );
                buf.set_finished( true );

                data->client->sendProtobuf( avalon::network::TEAM_SELECTION_BUF, buf.SerializeAsString( ) );
            } else {
                std::cerr << "[ ClientController ] You attempted to finalize team selection, but you're not the leader. Asshole." << std::endl;
            }

        } else if( action_type == "EnterVoteState" ) {

            return new VotingState( data );
        } else {

            return ClientControllerState::handleAction( action_to_be_handled );
        }

        // By default, we don't change states
        return NULL;
    }

    // Adds or removes the team member, if it corresponds to a change and returns whether it changed anything
    bool TeamSelectionState::addOrRemoveTeamMember( unsigned int player, bool add ) {

        bool changed = false;
        unsigned int location;
        bool exists = false;

        auto team = data->model->getDataForUpdate< std::vector< unsigned int > >( "questingTeam" );

        // Check to see if the player is already proposed
        for( unsigned int i = 0; i < team->size( ); i++ ) {
            if( (*team)[ i ] == player ) {
                location = i;
                exists = true;
            }
        }

        // If the players status in the team has changed, make the change
        if( add && !exists ) {
            team->push_back( player );
            changed = true;
        } else if( !add && exists ) {
            team->erase( team->begin( ) + location );
            changed = true;
        }

        return changed;
    }

} // client
} // avalon
