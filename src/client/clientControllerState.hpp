/*
 * File with class definitions for all the clientControllerStates
 * May be refactored into separate files later?
 *
 */
#ifndef CLIENT_CONTROLLER_STATE_HPP
#define CLIENT_CONTROLLER_STATE_HPP

#include <string>
#include <iostream>

#include "action.hpp"
#include "clientInfo.hpp"
#include "controllerState.hpp"
#include "globals.hpp"

#define FROMMODEL(class, name) *data->model->referenceData< class >( name )
#define FROMMODELREF(class, name) data->model->referenceData< class >( name )

namespace avalon {
namespace client {

/**
 * Parent class for for the ClientControllerStates
 * Inherits from ControllerState
 *
 * @class ClientControllerState
 * @author Ryan Kerr && Matt Hoffman
 * @date 2015-03-14
 */
class ClientControllerState : public ControllerState {
    public:

        /**
         * Constructor
         *
         * @param state_type_desc A string representation of the state we're in
         * @param dat A pointer to a ClientInfo struct with the clients data
         */
        ClientControllerState( std::string state_type_desc, ClientInfo* dat );

        /**
         * Method that deals with actions in a way common to all client states.
         * 
         * @param action_to_be_handled The action that we need to deal with in our current state
         */
        ControllerState* handleAction( Action* action_to_be_handled );

    protected:
        //! The data that is being passed between the controller states
        ClientInfo* data;
};

/**
 * ClientControllerState for when we're sitting in the lobby
 *
 * @class LobbyState
 * @author Ryan Kerr && Matt Hoffman
 * @date 2015-03-14
 */
class LobbyState : public ClientControllerState {
    public:

        /**
         * Constructor
         *
         * @param dat A pointer to a ClientInfo struct with the clients data
         */
        LobbyState( ClientInfo* dat );

        /**
         * Method to actually deal with the action we've received
         *
         * @param action_to_be_handled The action that we need to deal with in our current state
         */
        ControllerState* handleAction( Action* action_to_be_handled );

    private:

        /*
         * Helper function to populate the roleList vector
         *
         * @param buf The GameSettings buffer to get the roles from
         * @return None
         */
        void populateSpecialRoles( avalon::network::GameSettings* buf );
};

/**
 * ClientControllerState for when a leader is selecting a team
 *
 * @class TeamSelectionState
 * @author Ryan Kerr && Justin Koehler
 * @date 2015-03-16
 */
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

/**
 * ClientControllerState for when we're voting on something
 *
 * @class VotingState
 * @author Ryan Kerr && Matt Hoffman
 * @date 2015-03-14
 */
class VotingState : public ClientControllerState {
    public:

        /**
         * Constructor
         *
         * @param mod A pointer to a ClientInfo struct with the clients data
         */
        VotingState( ClientInfo* mod );

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

#endif // CLIENT_CONTROLLER_STATE_HPP
