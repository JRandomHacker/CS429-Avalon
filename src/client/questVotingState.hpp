/**
 * ClientControllerState for when we're voting on something
 *
 * @class QuestVotingState
 * @author Ryan Kerr && Matt Hoffman
 * @date 2015-03-14
 */

#ifndef _QUEST_VOTING_STATE_HPP
#define _QUEST_VOTING_STATE_HPP

#include <string>
#include <iostream>

#include "action.hpp"
#include "clientInfo.hpp"
#include "clientControllerState.hpp"
#include "globals.hpp"

namespace avalon {
namespace client {

class QuestVotingState : public ClientControllerState {
    public:

        /**
         * Constructor
         *
         * @param mod A pointer to a ClientInfo struct with the clients data
         */
        QuestVotingState( ClientInfo* mod );

        /**
         * Create the values in the model saying who has already voted
         */
        virtual void setupState( );

        /**
         * Remove the "who has voted" values from the model
         */
        virtual void teardownState( );

        /**
         * Method to actually deal with the action we've received
         *
         * @param action_to_be_handled The action that we need to deal with in our current state
         */
        ControllerState* handleAction( Action* action_to_be_handled );
};

} // client
} // avalon

#endif // _VOTING_STATE_HPP

