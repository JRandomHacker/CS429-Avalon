#include <string>

#include "serverControllerState.hpp"
#include "teamSelectionState.hpp"
#include "questVotingState.hpp"
#include "serverInfo.hpp"
#include "serverCustomActions.hpp"

#include "teamselection.pb.h"
#include "vote.pb.h"
#include "questvoteresults.pb.h"

namespace avalon {
namespace server {
    QuestVotingState::QuestVotingState( ServInfo* mod ) : ServerControllerState( "QuestVoting", mod ) { }

    QuestVotingState::~QuestVotingState( ) {
        model->team.clear( );
    }

    ControllerState* QuestVotingState::handleAction( Action* action_to_be_handled ) {

        std::string action_type = action_to_be_handled->getMessage();

        // We received a vote from a player
        if( action_type == "QuestVote" ) {

            auto action = dynamic_cast< QuestVoteAction* >( action_to_be_handled );
            unsigned int voter = action->getVoter( );
            avalon::player_vote_t vote = action->getVote( );

            bool isOnQuest = false;
            for( unsigned int i = 0; i < model->team.size( ); i++ ) {
                if( voter == model->team[ i ] ) {
                    isOnQuest = true;
                }
            }

            if ( !isOnQuest ) {
                std::cerr << "[ ServerController ] Received a quest vote from someone who wasn't on the team" << std::endl;
                return NULL;
            }

            // Checks to see if the voter modified their vote
            // (It is modified if it is new, or changed)
            // If the vote isn't new, we don't need to do anything
            if( modifyVote( voter, vote ) ) {

                // During the voting phase, you shouldn't know other players votes
                avalon::network::Vote buf;
                buf.set_id( voter );
                buf.set_vote( avalon::HIDDEN );

                sendProtobufToAll( avalon::network::QUEST_VOTE_BUF, buf.SerializeAsString( ) );

                // If we've received a vote from every player, we're ready to tally the results,
                // send the data to every player, and perform a state switch
                if( model->votes.size( ) == model->team.size( ) ) {

                    sendVoteResults( );
                    return decideNewState( );
                }
            }
        } else {
            return ServerControllerState::handleAction( action_to_be_handled );
        }

        return NULL;
    }

    // Goes through the votes array to see if you've already voted
    // changes your vote (if you did) and returns whether the vote was the first vote from the player
    bool QuestVotingState::modifyVote( unsigned int voter, avalon::player_vote_t vote ) {

        bool exists = false;

        // Look through the votes vector to see if the player voted previously
        for( unsigned int i = 0; i < model->votes.size( ); i++ ) {

            if( model->votes[ i ].first == voter ) {

                exists = true;
                avalon::player_vote_t currentVote = model->votes[ i ].second;
                if( currentVote != vote ) {

                    model->votes[ i ].second = vote;
                }
            }
        }

        // If they hadn't voted previously, just add their vote
        if( !exists ) {

            std::pair < unsigned int, avalon::player_vote_t > newVote( voter, vote );
            model->votes.push_back( newVote );
        }

        return !exists;
    }

    // Calculates the vote results, sends them to the players, and returns it
    void QuestVotingState::sendVoteResults( ) {

        // This is functionally the end of the voting phase,
        // so increase the vote track, switch the leader,
        // and tally the votes
        bool passed = figureOutResultsOfVote( );
        model->quest_track++;
        if ( !passed ) {
            model->quests_failed++;
        }

        randomizeVector( model->votes, model->rng );

        // Create the protobuf
        avalon::network::QuestVoteResults buf;
        buf.set_passed( passed );
        buf.set_quest_track( model->quest_track );
        for( unsigned int i = 0; i < model->votes.size( ); i++ ) {
            buf.add_votes( model->votes[ i ].second );
        }

        // Send the protobuf to all the players
        // We can't use broadcast, because we want to support hidden voting later
        for( unsigned int i = 0; i < model->num_clients; i++ ) {
            buf.set_my_vote( getVote( i ) );
            model->server->sendProtobuf( avalon::network::QUEST_VOTE_RESULTS_BUF, i, buf.SerializeAsString( ) );
        }
        model->votes.clear( );
    }

    // Figure out what state we should be entering, based off the vote
    // If it passed, go to quest vote
    // If it failed, and there is more space on vote track, go to team selection
    // If it failed, and there isn't more space on the vote track, go to end game
    ControllerState* QuestVotingState::decideNewState( ) {

        if( badGuysWon( ) ) {
            // TODO change our state to endgame
            model->server->broadcastStateChange( avalon::network::ENTER_END_GAME_BUF, 0 );
            return NULL;

        // If these fuckers can't make up their mind, they lose.
        } else if( model->quest_track >= model->quest_track_length ) {
            // TODO change our state to end game
            model->server->broadcastStateChange( avalon::network::ENTER_END_GAME_BUF, 0 );
            return NULL;

        // Go back to the selection state, moving one step closer to annihilation
        } else {
            model->server->broadcastStateChange( avalon::network::ENTER_TEAM_SELECTION_BUF, model->leader );
            return new TeamSelectionState( model );
        }
        return NULL;
    }

    // Iterates through the votes vector, counts the votes, and returns the results
    bool QuestVotingState::figureOutResultsOfVote( ) {

        unsigned int no = 0;
        for( std::vector< std::pair< unsigned int, avalon::player_vote_t > >::iterator it = model->votes.begin( ); it != model->votes.end( ); it++ ) {
            if( (*it).second == avalon::NO ) {
                no++;
            }
        }

        return no < model->fails_per_quest[ model->quest_track ];
    }

    avalon::player_vote_t QuestVotingState::getVote( unsigned int player_id ) {
        avalon::player_vote_t vote = avalon::NO_VOTE;

        for( unsigned int i = 0; i < model->votes.size( ); i++ ) {
            if ( model->votes[ i ].first == player_id ) {
                vote = model->votes[ i ].second;
            }
        }

        return vote;
    }

} // server
} // avalon

