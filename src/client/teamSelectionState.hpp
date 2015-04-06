/**
 * ClientControllerState for when a leader is selecting a team
 *
 * @class TeamSelectionState
 * @author Ryan Kerr && Justin Koehler
 * @date 2015-03-16
 */

#ifndef _TEAM_SELECTION_STATE_HPP
#define _TEAM_SELECTION_STATE_HPP

#include <string>
#include <iostream>

#include "action.hpp"
#include "clientInfo.hpp"
#include "clientControllerState.hpp"
#include "globals.hpp"

namespace avalon {
namespace client {

class TeamSelectionState : public ClientControllerState {
    public:

        /**
         * Constructor
         *
         * @param mod A pointer to a ClientInfo struct with the clients data
         */
        TeamSelectionState( ClientInfo* mod );

        /**
         * Method to actually deal with the action we've received
         *
         * @param action_to_be_handled The action that we need to deal with in our current state
         */
        ControllerState* handleAction( Action* action_to_be_handled );

    private:

        /*
         * Helper to add or remove a team mate from the proposed team
         *
         * @param player The player to add or remove
         * @param add Whether the player should be added or removed
         * @return Whether the team changed
         */
        bool addOrRemoveTeamMember( unsigned int player, bool add );
};

} // client
} // avalon

#endif // _TEAM_SELECTION_STATE_HPP
