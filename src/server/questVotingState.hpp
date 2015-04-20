/**
 * Fourth state
 * Waits for everyone to send a quest vote
 *
 * @class QuestVotingState
 * @author Ryan Kerr && Justin Koehler
 * @date 2015-04-06
 */

#ifndef _QUEST_VOTING_STATE_HPP
#define _QUEST_VOTING_STATE_HPP

#include <string>
#include <iostream>

#include "action.hpp"
#include "serverInfo.hpp"
#include "serverControllerState.hpp"

namespace avalon {
namespace server {

class QuestVotingState : public ServerControllerState {

    public:

        /**
         * Constructor
         *
         * @param mod A pointer to the ServInfo struct that the clientController is using
         */
        QuestVotingState( ServInfo* mod );

        /**
         * Destructor
         */
        ~QuestVotingState( );

        /**
         * The workhorse that actually takes care of an action
         *
         * @param action_to_be_handled The action that this controllerState should parse
         */
        ControllerState* handleAction( Action* action_to_be_handled );

    private:

        /*
         * A helper to change the player's vote or add it if they hadn't voted
         *
         * @param player_id The id of the player who sent a vote
         * @param player_vote_t The vote that the player sent
         * @return Whether the vote was the player's first vote in this round
         */
        bool modifyVote( unsigned int player_id, avalon::player_vote_t );

        /*
         * A helper to send the vote results at the end of the voting phase
         * Also sends the state change, and returns our new state
         *
         */
        void sendVoteResults( );

        /*
         * A helper to decide the new server state based off the vote
         *
         * @return The state we should enter
         */
        ControllerState* decideNewState( );

        /*
         * A helper to figure out the vote results
         *
         * @return None
         */
        bool figureOutResultsOfVote( );

        avalon::player_vote_t getVote( unsigned int player_id );
};

} // server
} // avalon

#endif // _QUEST_VOTING_STATE_HPP

