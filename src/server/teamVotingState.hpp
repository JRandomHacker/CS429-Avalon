/**
 * Third state
 * Waits for everyone to send a vote
 *
 * @class TeamVotingState
 * @author Ryan Kerr
 * @date 2015-03-16
 */

#ifndef _TEAM_VOTING_STATE_HPP
#define _TEAM_VOTING_STATE_HPP

#include <string>
#include <iostream>

#include "action.hpp"
#include "serverInfo.hpp"
#include "serverControllerState.hpp"

namespace avalon {
namespace server {

class TeamVotingState : public ServerControllerState {

    public:

        /**
         * Constructor
         *
         * @param mod A pointer to the ServInfo struct that the clientController is using
         */
        TeamVotingState( ServInfo* mod );

        /**
         * Destructor
         */
        ~TeamVotingState( );

        /**
         * The workhorse that actually takes care of an action
         *
         * @param action_to_be_handled The action that this controllerState should parse
         */
        ServerControllerState* handleAction( Action* action_to_be_handled );

    private:

        /*
         * A helper to change the player's vote or add it if they hadn't voted
         *
         * @param player_id The id of the player who sent a vote
         * @param player_vote_t The vote that the player sent
         * @return Whether the vote changed
         */
        bool modifyVote( unsigned int player_id, avalon::player_vote_t );

        /*
         * A helper to send the vote results at the end of the voting phase
         * Also sends the state change, and returns our new state
         *
         * @return Whether the vote passed or failed
         */
        bool sendVoteResults( );

        /*
         * A helper to decide the new server state based off the vote
         *
         * @return The state we should enter
         */
        ServerControllerState* decideNewState( bool vote_passed );

        /*
         * A helper to figure out the vote results
         *
         * @return None
         */
        bool figureOutResultsOfVote( );
};

} // server
} // avalon

#endif // VOTING_STATE_HPP
