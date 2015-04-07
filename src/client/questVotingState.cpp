#include "questVotingState.hpp"
#include "teamSelectionState.hpp"
#include "clientCustomActionsFromGUI.hpp"
#include "clientCustomActionsFromNet.hpp"
#include "clientCustomActionsForChat.hpp"
#include "clientInfo.hpp"
#include "voteHistory.hpp"
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

        } else if( action_type == "QuestVoteResults" ) {

            auto action = dynamic_cast< QuestVoteResultsAction* >( action_to_be_handled );

            // Pull the vote history vector from the model, append our new
            // history, and flag it as updated
            auto questHistory = data->model->getDataForUpdate< std::vector< bool > >( "questHistory" );
            questHistory->push_back( action->getVoteResult( ) );
            data->model->flagDataForUpdate( "questHistory" );
            
            data->model->updateData( "currentQuestTrack", action->getQuestVoteTrack( ) );

            // TODO Put everything in the model instead of printing
            if( action->getVoteResult( ) ) {
                std::cout << "[ ClientController ] Vote passed." << std::endl;
            } else {
                std::cout << "[ ClientController ] Vote failed." << std::endl;
            }

            std::cout << "[ ClientController ]";
            auto votes = action->getVotes( );
            for( unsigned int i = 0; i < votes->size( ); i++ ) {
                std::string pvote;
                switch( ( *votes )[ i ] ) {
                    case avalon::YES:
                        pvote = "pass";
                        break;
                    case avalon::NO:
                        pvote = "fail";
                        break;
                    case avalon::HIDDEN:
                        pvote = "hidden";
                        break;
                    case avalon::NO_VOTE:
                        pvote = "nothing";
                        break;
                }
                std::cout << " Vote " << i << " is " << pvote;
            }
            std::cout << std::endl;

        } else if( action_type == "ReceiveQuestVote" ) {

            auto action = dynamic_cast< ReceiveQuestVoteAction* >( action_to_be_handled );
            unsigned int votingPlayer = action->getVoter( );

            // TODO Update model with voting player instead of printing
            std::cout << "[ ClientController ] Received a quest vote from player " << votingPlayer << std::endl;

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

