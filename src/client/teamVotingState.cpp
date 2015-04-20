#include "teamVotingState.hpp"
#include "questVotingState.hpp"
#include "teamSelectionState.hpp"
#include "endGameState.hpp"
#include "finalGameState.hpp"
#include "clientCustomActionsFromGUI.hpp"
#include "clientCustomActionsFromNet.hpp"
#include "clientInfo.hpp"
#include "voteHistory.hpp"
#include "chat_message.hpp"

#include "teamselection.pb.h"
#include "vote.pb.h"
#include "chatmessage.pb.h"
#include <string>

namespace avalon {
namespace client {

    // Constructor for the VotingState, simply sets the correct state name
    TeamVotingState::TeamVotingState( ClientInfo* dat ) : ClientControllerState( "TeamVoting", dat ) { }

    // Figures out what needs to be done with any given action, when we're in the voting state
    ControllerState* TeamVotingState::handleAction( Action* action_to_be_handled ) {

        std::string action_type = action_to_be_handled->getMessage( );

        if( action_type == "TeamVote" ) {

            auto action = dynamic_cast< TeamVoteAction* >( action_to_be_handled );

            avalon::network::Vote buf;
            buf.set_id( FROMMODEL(unsigned int, "myID") );
            buf.set_vote( action->getPlayerVote( ) );
            data->client->sendProtobuf( avalon::network::TEAM_VOTE_BUF, buf.SerializeAsString( ) );

        } else if( action_type == "TeamVoteResults" ) {

            auto action = dynamic_cast< TeamVoteResultsAction* >( action_to_be_handled );

            // Construct a new VoteHistory from the current data
            VoteHistory record( action->getVoteResult(), action->getVotes(),
                FROMMODEL( std::vector< unsigned int >, "questingTeam" ),
                FROMMODEL( unsigned int, "voteTrackLength" ),
                FROMMODEL( unsigned int, "questTrackLength" ) );

            // Pull the vote history vector from the model, append our new
            // history, and flag it as updated
            auto voteHistories = data->model->getDataForUpdate< std::vector< VoteHistory > >( "voteHistory" );
            voteHistories->push_back( record );
            data->model->flagDataForUpdate( "voteHistory" );

            data->model->updateData( "currentVoteTrack", action->getVoteTrack( ) );

        } else if( action_type == "ReceiveTeamVote" ) {

            auto action = dynamic_cast< ReceiveTeamVoteAction* >( action_to_be_handled );
            unsigned int votingPlayer = action->getVoter( );

            auto currentVotes = data->model->getDataForUpdate< std::vector< avalon::player_vote_t > >( "currentVotes" );
            (*currentVotes)[votingPlayer] = avalon::HIDDEN;
            data->model->flagDataForUpdate( "currentVotes" );

        } else if( action_type == "EnterTeamSelection" ) {

            auto action = dynamic_cast< EnterTeamSelectionAction* >( action_to_be_handled );
            data->model->updateData( "leaderID", action->getLeader( ) );
            data->model->updateData( "questingTeam", std::vector< unsigned int >( ) );

            return new TeamSelectionState( data );
        } else if( action_type == "EnterQuestVoteState" ) {
            return new QuestVotingState( data );
        } else if( action_type == "EnterEndGameState" ) {
            return new EndGameState( data );
        } else if( action_type == "EnterFinalGameState" ) {
            return new FinalGameState( data );
        } else {
            return ClientControllerState::handleAction( action_to_be_handled );
        }

        // By default, we don't change states
        return NULL;
    }

    void TeamVotingState::setupState( ) {
        std::vector< avalon::player_vote_t > votes;
        votes.resize( FROMMODEL( unsigned int, "numberOfPlayers" ), avalon::NO_VOTE );
        data->model->addData( "currentVotes", votes );
        data->model->updateData( "teamVoteState", true );
    }

    void TeamVotingState::teardownState( ) {
        data->model->updateData( "teamVoteState", false );
        data->model->removeData( "currentVotes" );

    }

} // client
} // avalon
