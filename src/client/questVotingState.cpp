#include <string>

#include "questVotingState.hpp"
#include "teamSelectionState.hpp"
#include "clientCustomActionsFromGUI.hpp"
#include "clientCustomActionsFromNet.hpp"
#include "clientCustomActionsForChat.hpp"
#include "clientInfo.hpp"
#include "questVoteHistory.hpp"
#include "chat_message.hpp"

#include "teamselection.pb.h"
#include "vote.pb.h"
#include "chatmessage.pb.h"

namespace avalon {
namespace client {

    // Constructor for the QuestVotingState, simply sets the correct state name
    QuestVotingState::QuestVotingState( ClientInfo* dat ) : ClientControllerState( "QuestVoting", dat ) { }

    // Figures out what needs to be done with any given action, when we're in the voting state
    ControllerState* QuestVotingState::handleAction( Action* action_to_be_handled ) {

        std::string action_type = action_to_be_handled->getMessage( );

        if( action_type == "QuestVote" ) {

            auto action = dynamic_cast< QuestVoteAction* >( action_to_be_handled );

            avalon::network::Vote buf;
            buf.set_id( FROMMODEL( unsigned int, "myID" ) );
            buf.set_vote( action->getPlayerVote( ) );
            data->client->sendProtobuf( avalon::network::QUEST_VOTE_BUF, buf.SerializeAsString( ) );

        } else if( action_type == "ChatMessageRecv" ) {

            auto action = dynamic_cast< ChatMessageRecvAction* >( action_to_be_handled );
            unsigned int sender = action->getMessage( ).getSenderId( );
            std::string message_text = action->getMessage( ).getMessageText( );
            unsigned int time = action->getMessage( ).getTimestamp( );

            avalon::network::ChatMessage buf;
            buf.set_sender_id( sender );
            buf.set_message_text( message_text );
            buf.set_timestamp( time );
        } else if( action_type == "QuestVoteResults" ) {

            auto action = dynamic_cast< QuestVoteResultsAction* >( action_to_be_handled );

            QuestVoteHistory record( action->getVoteResult(), *action->getVotes(), FROMMODEL( unsigned int, "questTrackLength" ) );

            // Pull the vote history vector from the model, append our new
            // history, and flag it as updated
            auto questHistory = data->model->getDataForUpdate< std::vector< QuestVoteHistory > >( "questHistory" );
            questHistory->push_back( record );
            data->model->flagDataForUpdate( "questHistory" );

            data->model->updateData( "currentQuestTrack", action->getQuestVoteTrack( ) );

        } else if( action_type == "ReceiveQuestVote" ) {

            auto action = dynamic_cast< ReceiveQuestVoteAction* >( action_to_be_handled );
            unsigned int votingPlayer = action->getVoter( );

            auto currentVotes = data->model->getDataForUpdate< std::vector< avalon::player_vote_t > >( "currentVotes" );
            (*currentVotes)[votingPlayer] = avalon::HIDDEN;
            data->model->flagDataForUpdate( "currentVotes" );

        } else if( action_type == "EnterTeamSelection" ) {

            auto action = dynamic_cast< EnterTeamSelectionAction* >( action_to_be_handled );
            data->model->updateData( "leaderID", action->getLeader( ) );
            data->model->updateData( "questingTeam", std::vector< unsigned int >( ) );

            return new TeamSelectionState( data );
        } else {
            return ClientControllerState::handleAction( action_to_be_handled );
        }

        // By default, we don't change states
        return NULL;
    }

    void QuestVotingState::setupState( ) {
        std::vector< avalon::player_vote_t > votes;
        votes.resize( FROMMODEL( unsigned int, "numberOfPlayers" ), avalon::NO_VOTE );
        data->model->addData( "currentVotes", votes );
        data->model->updateData( "questVoteState", true );
    }

    void QuestVotingState::teardownState( ) {
        data->model->updateData( "questVoteState", false );
        data->model->removeData( "currentVotes" );

    }

} // client
} // avalon

