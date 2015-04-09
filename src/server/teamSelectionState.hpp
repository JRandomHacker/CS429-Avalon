/**
 * Second state
 * Waits for the leader to send us team members they want on a mission
 *
 * @class TeamSelectionState
 * @author Ryan Kerr
 * @date 2015-03-12
 */

#ifndef _TEAM_SELECTION_STATE_HPP
#define _TEAM_SELECTION_STATE_HPP

#include <string>
#include <iostream>

#include "action.hpp"
#include "serverInfo.hpp"
#include "serverControllerState.hpp"

namespace avalon {
namespace server {
    
class TeamSelectionState : public ServerControllerState {

    public:

        /**
         * Constructor
         *
         * @param mod A pointer to the ServInfo struct that the clientController is using
         */
        TeamSelectionState( ServInfo* mod );

        /**
         * Destructor
         */
        ~TeamSelectionState( );

        /**
         * The workhorse that actually takes care of an action
         *
         * @param action_to_be_handled The action that this controllerState should parse
         */
        ControllerState* handleAction( Action* action_to_be_handled );

    private:
        /*
         * A helper to toggle the selection of a player
         * Removes the player from the team selection vector if they're in it
         * Adds them otherwise
         *
         * @param player_id The id of the player to toggle
         * @return Whether the player is now selected or not
         */
        bool toggleSelection( unsigned int player_id );
};

} // server
} // avalon

#endif // TEAM_SELECTION_STATE_HPP
