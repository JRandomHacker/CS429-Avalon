#include "serverControllerState.hpp"
#include "teamSelectionState.hpp"
#include "votingState.hpp"
#include "questVotingState.hpp"
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
    VotingState::VotingState( ServInfo* mod ) : ServerControllerState( "Voting", mod ) { }

    VotingState::~VotingState( ) { }

    ServerControllerState* VotingState::handleAction( Action* action_to_be_handled ) {

        std::string action_type = action_to_be_handled->getMessage();

        // We received a vote from a player
        if( action_type == "TeamVote" ) {

            auto action = dynamic_cast< TeamVoteAction* >( action_to_be_handled );
            unsigned int voter = action->getVoter( );
            avalon::player_vote_t vote = action->getVote( );

            // Checks to see if the voter modified their vote
            // (It is modified if it is new, or changed)
            // If they haven't changed their vote, we don't need to do anything
            if( modifyVote( voter, vote ) ) {

                // During the voting phase, you shouldn't know other players votes
                avalon::network::Vote buf;
                buf.set_id( voter );
                buf.set_vote( avalon::HIDDEN );

                sendProtobufToAll( avalon::network::VOTE_BUF, buf.SerializeAsString( ) );

                // If we've received a vote from every player, we're ready to tally the results,
                // send the data to every player, and perform a state switch
                if( model->votes.size( ) == model->num_clients ) {

                    bool passed = sendVoteResults( );
                    return decideNewState( passed );
                }
            }
        } else {
            reportUnhandledAction( action_type );
        }

        return NULL;
    }

    // Goes through the votes array to see if you've already voted
    // changes your vote (if you did) and returns whether anything was changed
    bool VotingState::modifyVote( unsigned int voter, avalon::player_vote_t vote ) {

        bool changed = false;
        bool exists = false;

        // Look through the votes vector to see if the player voted previously
        for( unsigned int i = 0; i < model->votes.size( ); i++ ) {

            if( model->votes[ i ].first == voter ) {

                exists = true;
                avalon::player_vote_t currentVote = model->votes[ i ].second;
                if( currentVote != vote ) {

                    model->votes[ i ].second = vote;
                    changed = true;
                }
            }
        }

        // If they hadn't voted previously, just add their vote
        if( !exists ) {

            std::pair < unsigned int, avalon::player_vote_t > newVote( voter, vote );
            model->votes.push_back( newVote );
            changed = true;
        }

        return changed;
    }

    // Calculates the vote results, sends them to the players, and returns it
    bool VotingState::sendVoteResults( ) {

        // This is functionally the end of the voting phase,
        // so increase the vote track, switch the leader,
        // and tally the votes
        model->vote_track++;
        model->leader++;
        model->leader %= model->num_clients;
        bool passed = figureOutResultsOfVote( );

        // If it passed, we need to reset the vote track
        if( passed ) {
            model->vote_track = 0;
        }

        // Sort the votes into player order
        avalon::player_vote_t sorted_votes[ model->num_clients ];
        for( std::vector< std::pair< unsigned int, avalon::player_vote_t > >::iterator it = model->votes.begin( ); it != model->votes.end( ); it++ ) {

            sorted_votes[ (*it).first ] = (*it).second;
        }

        // Create the protobuf
        avalon::network::VoteResults buf;
        buf.set_passed( passed );
        buf.set_vote_track( model->vote_track );
        for( unsigned int i = 0; i < model->num_clients; i++ ) {
            buf.add_votes( sorted_votes[ i ] );
        }

        // Send the protobuf to all the players
        // We can't use broadcast, because we want to support hidden voting later
        for( unsigned int i = 0; i < model->num_clients; i++ ) {

            if( model->hidden_voting ) {

                buf.set_votes( i, sorted_votes[ i ] );
                model->server->sendProtobuf( avalon::network::VOTE_RESULTS_BUF, i, buf.SerializeAsString( ) );
                buf.set_votes( i, avalon::HIDDEN );
            } else {

                model->server->sendProtobuf( avalon::network::VOTE_RESULTS_BUF, i, buf.SerializeAsString( ) );
            }

        }
        model->votes.clear( );

        return passed;
    }

    // Figure out what state we should be entering, based off the vote
    // If it passed, go to quest vote
    // If it failed, and there is more space on vote track, go to team selection
    // If it failed, and there isn't more space on the vote track, go to end game
    ServerControllerState* VotingState::decideNewState( bool vote_passed ) {

        if( vote_passed ) {

            // TODO change our state to quest vote
            model->server->broadcastStateChange( avalon::network::ENTER_QUEST_VOTE_BUF, model->leader );
            return new QuestVotingState( model );

        // If these fuckers can't make up their mind, they lose.
        } else if( model->vote_track >= model->vote_track_length ) {
            // TODO change our state to end game
            model->server->broadcastStateChange( avalon::network::ENTER_END_GAME_BUF, 0 );
            return NULL;

        // Go back to the selection state, moving one step closer to annihilation
        } else {
            model->server->broadcastStateChange( avalon::network::ENTER_TEAM_SELECTION_BUF, model->leader );
            return new TeamSelectionState( model );
        }

    }

    // Iterates through the votes vector, counts the votes, and returns the results
    bool VotingState::figureOutResultsOfVote( ) {

        unsigned int yes = 0;
        unsigned int no = 0;
        for( std::vector< std::pair< unsigned int, avalon::player_vote_t > >::iterator it = model->votes.begin( ); it != model->votes.end( ); it++ ) {

            if( (*it).second == avalon::YES ) {
                yes++;
            } else {
                no++;
            }
        }

        return yes > no;
    }
} // server
} // avalon
