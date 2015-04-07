/*
 * Implementation for all of the clientControllerStates
 *
 * @author Ryan Kerr && Matt Hoffman
 * @date 2015-03-14
 * @file clientControllerState.cpp
 */
#include "clientControllerState.hpp"
#include "clientCustomActionsFromGUI.hpp"
#include "clientCustomActionsFromNet.hpp"
#include "clientCustomActionsForChat.hpp"
#include "clientInfo.hpp"
#include "voteHistory.hpp"
#include "chat_message.hpp"

#include "teamselection.pb.h"
#include "vote.pb.h"
#include "chatmessage.pb.h"

#include <climits>

namespace avalon {
namespace client {

// ClientControllerState {
    // Constructor for the parent class, ClientControllerState.
    // Sets the description and model internally to whatever it gets passed
    ClientControllerState::ClientControllerState( std::string state_type_desc, ClientInfo* dat )
        : ControllerState(state_type_desc), data( dat ) {

            std::cerr << "[ ClientController ] Entered " << state_type_desc << " state" << std::endl;
    }

    // Default action handler for all actions unhandled by individual client controller states.
    ControllerState* ClientControllerState::handleAction( Action* action_to_be_handled ) {

        std::string action_type = action_to_be_handled->getMessage( );

        if ( action_type == "ChatMessageSent" ) {
            // Handle a chat message sent to this client

            /*auto action = dynamic_cast< ChatMessageSentAction* >( action_to_be_handled );
            auto buf = action->getMessage( ).toProtoBuf( );
            data->client->sendProtobuf( avalon::network::CHAT_MSG_BUF, buf.SerializeAsString( ) );
            */

        } else if ( action_type == "ChatMessageRecv" ) {
            // Handle sending a chat message to the server 

            // auto action = dynamic_cast< ChatMessageRecvAction* >( action_to_be_handled );
            
        } else if ( action_type == "EnterQuestVoteState" ) {
            return ControllerState::handleAction( action_to_be_handled );
        }

        return NULL;
    }
// }

} // client
} // avalon
