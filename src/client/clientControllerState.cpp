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
#include "clientInfo.hpp"
#include "voteHistory.hpp"

#include "teamselection.pb.h"
#include "vote.pb.h"

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
// }

// VotingState {

    // Constructor for the VotingState, simply sets the correct state name
    VotingState::VotingState( ClientInfo* dat ) : ClientControllerState( "Voting", dat ) {
    }

    // Figures out what needs to be done with any given action, when we're in the voting state
    ControllerState* VotingState::handleAction( Action* action_to_be_handled ) {

        std::string action_type = action_to_be_handled->getMessage( );

        if( action_type == "TeamVote" ) {

            auto action = dynamic_cast< TeamVoteAction* >( action_to_be_handled );

            avalon::network::Vote buf;
            buf.set_id( FROMMODEL(unsigned int, "myID") );
            buf.set_vote( action->getPlayerVote( ) );
            data->client->sendProtobuf( avalon::network::VOTE_BUF, buf.SerializeAsString( ) );

        } else if( action_type == "VoteResults" ) {

            auto action = dynamic_cast< VoteResultsAction* >( action_to_be_handled );

            // Construct a new VoteHistory from the current data
            VoteHistory record( action->getVoteResult(), *action->getVotes(),
                FROMMODEL( std::vector< unsigned int >, "questingTeam" ),
                FROMMODEL( unsigned int, "voteTrackLength" ),
                FROMMODEL( unsigned int, "questTrackLength" ) );

            // Pull the vote history vector from the model, append our new
            // history, and flag it as updated
            auto voteHistories = data->model->getDataForUpdate< std::vector< VoteHistory > >( "voteHistory" );
            voteHistories->push_back( record );
            data->model->flagDataForUpdate( "voteHistory" );

            data->model->updateData( "currentVoteTrack", action->getVoteTrack( ) );

            // TODO Put everything in the model instead of printing
            if( action->getVoteResult( ) ) {
                std::cout << "[ ClientController ] Vote passed." << std::endl;
            } else {
                std::cout << "[ ClientController ] Vote failed." << std::endl;
            }

            std::cout << "[ ClientController ]";
            auto votes = action->getVotes( );
            for( unsigned int i = 0; i < votes->size( ); i++ ) {
                std::string pvote;
                switch( ( *votes )[ i ] ) {
                    case avalon::YES:
                        pvote = "pass";
                        break;
                    case avalon::NO:
                        pvote = "fail";
                        break;
                    case avalon::HIDDEN:
                        pvote = "hidden";
                        break;
                    case avalon::NO_VOTE:
                        pvote = "nothing";
                        break;
                }
                std::cout << " Player " << i << " voted " << pvote;
            }
            std::cout << std::endl;

        } else if( action_type == "ReceiveVote" ) {

            auto action = dynamic_cast< ReceiveVoteAction* >( action_to_be_handled );
            unsigned int votingPlayer = action->getVoter( );

            // TODO Update model with voting player instead of printing
            std::cout << "[ ClientController ] Received a vote from player " << votingPlayer << std::endl;

            auto currentVotes = data->model->getDataForUpdate< std::vector< avalon::player_vote_t > >( "currentVotes" );
            (*currentVotes)[votingPlayer] = avalon::HIDDEN;
            data->model->flagDataForUpdate( "currentVotes" );

        } else if( action_type == "EnterTeamSelection" ) {

            auto action = dynamic_cast< EnterTeamSelectionAction* >( action_to_be_handled );
            data->model->updateData( "leaderID", action->getLeader( ) );
            data->model->updateData( "questingTeam", std::vector< unsigned int >( ) );
            
            return new TeamSelectionState( data );
        } else {

            reportUnhandledAction( action_type );
        }

        // By default, we don't change states
        return NULL;
    }

    void VotingState::setupState( ) {
        std::vector< avalon::player_vote_t > votes;
        votes.resize( FROMMODEL( unsigned int, "numberOfPlayers" ), avalon::NO_VOTE );
        data->model->addData( "currentVotes", votes );
        data->model->updateData( "voteState", true );
    }

    void VotingState::teardownState( ) {
        data->model->updateData( "voteState", false );
        data->model->removeData( "currentVotes" );

    }
// }

// TeamSelectionState {

    // Constructor for the TeamSelectionState, simply sets the correct state name
    TeamSelectionState::TeamSelectionState( ClientInfo* dat ) : ClientControllerState( "TeamSelection", dat ) { }

    // Figures out what needs to be done with any given action, when we're in the voting state
    ControllerState* TeamSelectionState::handleAction( Action* action_to_be_handled ) {

        std::string action_type = action_to_be_handled->getMessage( );

        if( action_type == "SelectQuestGoer" ) {
            if( FROMMODEL( unsigned int, "leaderID" ) == FROMMODEL( unsigned int, "myID" ) ) {

                auto action = dynamic_cast< SelectQuestGoerAction* >( action_to_be_handled );
                avalon::network::TeamSelection buf;
                buf.set_selected( action->getSelected( ) );
                buf.set_id( action->getSelectedPlayer( ) );

                data->client->sendProtobuf( avalon::network::TEAM_SELECTION_BUF, buf.SerializeAsString( ) );
            } else {
                std::cerr << "[ ClientController ] You attempted to select a quest goer, but you're not the leader. Asshole." << std::endl;
            }

        } else if( action_type == "ModifyTeamSelection" ) {

            auto action = dynamic_cast< ModifyTeamSelectionAction* >( action_to_be_handled );

            unsigned int player = action->getSelection( );
            bool added = action->getSelected( );
            bool changed = addOrRemoveTeamMember( player, added );

            if ( changed ) {
                data->model->flagDataForUpdate( "questingTeam" );
            }

        } else if( action_type == "FinalizeTeam" ) {
            if( FROMMODEL( unsigned int, "leaderID" ) == FROMMODEL( unsigned int, "myID" ) ) {

                avalon::network::TeamSelection buf;
                buf.set_selected( true );
                buf.set_id( ( int ) 1189998819991197253 );
                buf.set_finished( true );

                data->client->sendProtobuf( avalon::network::TEAM_SELECTION_BUF, buf.SerializeAsString( ) );
            } else {
                std::cerr << "[ ClientController ] You attempted to finalize team selection, but you're not the leader. Asshole." << std::endl;
            }

        } else if( action_type == "EnterVoteState" ) {

            return new VotingState( data );
        } else {

            reportUnhandledAction( action_type );
        }

        // By default, we don't change states
        return NULL;
    }

    // Adds or removes the team member, if it corresponds to a change and returns whether it changed anything
    bool TeamSelectionState::addOrRemoveTeamMember( unsigned int player, bool add ) {

        bool changed = false;
        unsigned int location;
        bool exists = false;

        auto team = data->model->getDataForUpdate< std::vector< unsigned int > >( "questingTeam" );

        // Check to see if the player is already proposed
        for( unsigned int i = 0; i < team->size( ); i++ ) {
            if( (*team)[ i ] == player ) {
                location = i;
                exists = true;
            }
        }

        // If the players status in the team has changed, make the change
        if( add && !exists ) {
            team->push_back( player );
            changed = true;
        } else if( !add && exists ) {
            team->erase( team->begin( ) + location );
            changed = true;
        }

        return changed;
    }
// }

// LobbyState {
    // Constructor for the LobbyState, simply sets the correct state name
    LobbyState::LobbyState( ClientInfo* dat ) : ClientControllerState( "Lobby", dat ) { }

    // Figures out what needs to be done with any given action, when we're in the lobby state
    ControllerState* LobbyState::handleAction( Action* action_to_be_handled ) {

        std::string action_type = action_to_be_handled->getMessage( );

        // We need to add all the information about the game to the model
        if( action_type == "GameSettings" ) {

            auto action = dynamic_cast< GameSettingsAction* >( action_to_be_handled );
            avalon::network::GameSettings* sBuf = action->getSettings( );
            
            data->model->addData< unsigned int >( "numberOfPlayers", sBuf->players( ) );
            data->model->addData< unsigned int >( "myID", sBuf->client( ) );
            data->model->addData< unsigned int >( "numEvilChars", sBuf->evil_count( ) );
            data->model->addData< unsigned int >( "questTrackLength", sBuf->quest_track_len( ) );
            data->model->addData< unsigned int >( "voteTrackLength", sBuf->vote_track_len( ) );
            data->model->addData< unsigned int >( "leaderID", UINT_MAX );
            data->model->addData( "questingTeam", std::vector< unsigned int >() );
            data->model->addData< unsigned int >( "currentVoteTrack", 0 );
            data->model->addData< unsigned int >( "currentQuestTrack", 0 );
            data->model->addData( "voteHistory", std::vector< VoteHistory >( ) );
            data->model->addData( "voteState", false );
            populateSpecialRoles( sBuf );

            for ( unsigned int i = 0; i < ( unsigned int )sBuf->players( ); i++ ) {
                data->model->addData( std::string( "player" ) + std::to_string( i ), NULL );
            }

            delete sBuf;
            data->model->updateData("hasGameSettings", true);

        // We need to update one of the players in the model with the real player
        } else if( action_type == "AddPlayer" ) {

            auto action = dynamic_cast< AddPlayerAction* >( action_to_be_handled );
            unsigned int player_number = action->getPlayerNumber( );
            Player* p = action->getPlayerInfo( );
            
            data->model->updateData( std::string( "player" ) + std::to_string( player_number ), *p );
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

            reportUnhandledAction( action_type );
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
// }

} // client
} // avalon