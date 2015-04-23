#include "lobbyState.hpp"
#include "teamSelectionState.hpp"
#include "clientCustomActionsFromGUI.hpp"
#include "clientCustomActionsFromNet.hpp"
#include "clientCustomActionsForChat.hpp"
#include "clientInfo.hpp"
#include "voteHistory.hpp"
#include "questVoteHistory.hpp"
#include "chat_message.hpp"

#include "teamselection.pb.h"
#include "vote.pb.h"
#include "chatmessage.pb.h"

#include <climits>
#include <vector>

namespace avalon {
namespace client {

// Constructor for the LobbyState, simply sets the correct state name
    LobbyState::LobbyState( ClientInfo* dat ) : ClientControllerState( "Lobby", dat ) { }

    // Figures out what needs to be done with any given action, when we're in the lobby state
    ControllerState* LobbyState::handleAction( Action* action_to_be_handled ) {

        std::string action_type = action_to_be_handled->getMessage( );

        // We need to add all the information about the game to the model
        if( action_type == "GameSettings" ) {

            auto action = dynamic_cast< GameSettingsAction* >( action_to_be_handled );
            avalon::network::GameSettings* sBuf = action->getSettings( );
            
            data->model->addData( "resetGame", false );
            data->model->addData< unsigned int >( "numberOfPlayers", sBuf->players( ) );
            data->model->addData< unsigned int >( "myID", sBuf->client( ) );
            data->model->addData< unsigned int >( "numEvilChars", sBuf->evil_count( ) );
            data->model->addData< unsigned int >( "questTrackLength", sBuf->quest_track_len( ) );
            data->model->addData< unsigned int >( "voteTrackLength", sBuf->vote_track_len( ) );
            data->model->addData< unsigned int >( "leaderID", UINT_MAX );
            data->model->addData( "questingTeam", std::vector< unsigned int >() );
            data->model->addData< unsigned int >( "currentVoteTrack", 0 );
            data->model->addData< unsigned int >( "currentQuestTrack", 0 );
            data->model->addData( "questHistory", std::vector< QuestVoteHistory >( ) );
            data->model->addData( "voteHistory", std::vector< VoteHistory >( ) );
            data->model->addData( "teamVoteState", false );
            data->model->addData( "questVoteState", false );
            data->model->addData( "assassinState", false );
            data->model->addData< unsigned int >( "assassinTargeted", 0 );
            data->model->addData( "endGameState", false );
            data->model->addData< avalon::alignment_t >( "winningTeam", avalon::UNKNOWN_ALIGN ); 
            data->model->addData( "endGamePlayers", std::vector< Player >( ) );
            data->model->addData( "chatMessages", std::vector< avalon::common::ChatMessage >( ) );
            
            std::vector< unsigned int > players_per_quest;
            for( auto it = sBuf->players_per_quest( ).begin( ); it != sBuf->players_per_quest( ).end( ); it++ ) {
                players_per_quest.push_back( ( unsigned int )( *it ) );
            }
            data->model->addData( "playersPerQuest", players_per_quest );
            
            std::vector< unsigned int > fails_per_quest;
            for( auto it = sBuf->fails_per_quest( ).begin( ); it != sBuf->fails_per_quest( ).end( ); it++ ) {
                fails_per_quest.push_back( ( unsigned int )( *it ) );
            }
            data->model->addData( "failsPerQuest", fails_per_quest );
            
            populateSpecialRoles( sBuf );

            data->model->addData( "players", std::vector< Player >( ) );

            delete sBuf;
            data->model->updateData("hasGameSettings", true);

        // We need to update one of the players in the model with the real player
        } else if( action_type == "AddPlayer" ) {

            auto action = dynamic_cast< AddPlayerAction* >( action_to_be_handled );
            unsigned int player_number = action->getPlayerNumber( );
            Player* p = action->getPlayerInfo( );
            
            auto players = data->model->getDataForUpdate< std::vector< Player > >( "players" );
            players->push_back( *p );
            data->model->flagDataForUpdate( "players" );

            delete p;

        // We need to send the Server our preferred name
        } else if ( action_type == "SetName" ) {

            auto action = dynamic_cast< SetNameAction* >( action_to_be_handled );
            Player p( action->getName( ), avalon::UNKNOWN_ROLE, avalon::UNKNOWN_ALIGN );
            avalon::network::Player buf = p.getBuf( );
            unsigned int my_id = -1;
            const unsigned int* my_id_ref = FROMMODELREF( unsigned int, "myID" );
            if ( my_id_ref != NULL ) {
                my_id = *my_id_ref;
            }
            buf.set_id( my_id );
            data->client->sendProtobuf( avalon::network::PLAYER_BUF, buf.SerializeAsString( ) );

        } else if ( action_type == "EnterTeamSelection" ) {
            auto action = dynamic_cast< EnterTeamSelectionAction* >( action_to_be_handled );
            data->model->updateData( "leaderID", action->getLeader( ) );
            data->model->updateData( "questingTeam", std::vector< unsigned int >( ) );

            return new TeamSelectionState( data );
        // We don't care about the action we received, since it isn't valid in this state
        } else {

            return ClientControllerState::handleAction( action_to_be_handled );
        }

        // By default, we haven't changed states
        return NULL;
    }

    void LobbyState::populateSpecialRoles( avalon::network::GameSettings* buf ) {
        std::vector< avalon::special_roles_t > roles;

        if( buf->merlin( ) ) {
            roles.push_back( avalon::MERLIN );
        }
        if( buf->percival( ) ) {
            roles.push_back( avalon::PERCIVAL );
        }
        if( buf->mordred( ) ) {
            roles.push_back( avalon::MORDRED );
        }
        if( buf->morgana( ) ) {
            roles.push_back( avalon::MORGANA );
        }
        if( buf->assassin( ) ) {
            roles.push_back( avalon::ASSASSIN );
        }
        if( buf->oberon( ) ) {
            roles.push_back( avalon::OBERON );
        }

        data->model->addData( "roleList", roles );
    }
    
} // client
} // avalon
