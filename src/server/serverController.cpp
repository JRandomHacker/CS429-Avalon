/*
 * Implementation of serverController class
 *
 * @file serverController.cpp
 * @author Ryan Kerr
 * @date 2015-03-11
 */
#include "serverController.hpp"
#include "serverControllerState.hpp"
#include "waitingForClientsState.hpp"
#include "settings.pb.h"
#include "globals.hpp"
#include "player.pb.h"
#include "serverInfo.hpp"

#include <semaphore.h>
#include <pthread.h>
#include <vector>
#include <random>
#include <string>

// Constructor
ServerController::ServerController( ServInfo* model, ServerSettings settings ) {

    this->model = model;
    this->model->server = new Server( settings.port );

    this->qSem = new sem_t;
    sem_init( qSem, 0, 0 ); // Initialize semaphore to 0 so wait immediately blocks

    // Set up the queue for actions so the network can send us actions
    action_queue = new ActionHandler( qSem );
    model->server->initQueue( action_queue );

    handling_state = NULL;
    setServerState( new avalon::server::WaitingForClientsState( model ) );

    std::random_device rd; // Seed
    model->rng = new std::mt19937( rd( ) ); // Actual generator
}

// Destructor
ServerController::~ServerController( ) {

    // We didn't allocate the model, so leave it for whoever passed it to us
    delete model->server;
    delete qSem;
    delete action_queue;
    delete model->rng;
    releaseServerState( );
}

// Actually initialize the network thread and connection
int ServerController::spawnNetwork( ) {

    int retval = model->server->initServer( );
    if( retval != EXIT_SUCCESS ) {
        return retval;
    }

    // Start the Server listening for clients
    if( pthread_create( &network_thread, NULL, &networkThreadHelper, this ) != 0 ) {
        std::cerr << "Unable to start network thread" << std::endl;
        return EXIT_THREAD_ERROR;
    }

    pthread_detach( network_thread );

    return EXIT_SUCCESS;
}

// Initializes the model
int ServerController::initModel( ServerSettings settings ) {
    model->num_clients = settings.num_players;
    model->vote_track = 0;
    model->quest_track = 0;
    model->quests_failed = 0;
    model->hidden_voting = settings.hidden_votes;
    model->vote_track_length = settings.game_settings.vote_track_length;
    model->quest_track_length = settings.game_settings.quest_track_length;
    model->players_per_quest = settings.game_settings.players_per_quest;
    model->fails_per_quest = settings.game_settings.fails_per_quest;

    initModelCharacters( settings );
    return initModelPlayer( settings );
}


// Helper function to start up the networking thread
void* ServerController::networkThreadHelper( void* obj ) {

    // Cast the obj to a pointer to ourself so we can start the method we actually wanted
    static_cast< ServerController* >( obj )->networkThreadFunc( );
    return NULL;
}

// Actual network thread function
void ServerController::networkThreadFunc( ) {
    model->server->waitForClients( model->num_clients );

    while( true ) {
        model->server->waitForData( );
    }
}

// Helper function to change the controller's state
void ServerController::setServerState( avalon::server::ServerControllerState* new_state ) {
    if( new_state != NULL ) {
        releaseServerState( ); // Clean up the old ControllerState before changing
        handling_state = new_state;
    }
}

// Helper function to clean up the ControllerState
void ServerController::releaseServerState( ) {

    // Make sure we'd actually GOTTEN a state
    if( handling_state != NULL ) {
        delete handling_state;
        handling_state = NULL;
    }
}

// Helper function to process an action
// Calls the ActionHandler to do the work and changes our state accordingly
void ServerController::processAction( Action* action ) {
    setServerState( static_cast< avalon::server::ServerControllerState* >( handling_state->handleAction( action ) ) );
}

// Infinite loop waiting for clients to do things
void ServerController::processActions( ) {
    while( true ) {
        sem_wait( qSem ); // Wait until there is an action to process, so we don't spinlock

        // Grab all the currently available actions
        std::pair< int, std::list< Action* >::iterator > available_actions = action_queue->freezeFrontActions( );

        // Iterate through the available actions and process them
        for( int i = 0; i < available_actions.first; i++ ) {
            processAction( *available_actions.second );
            available_actions.second++;
        }

        // Decrement the semaphore for as many actions as we got
        // This is a bit hacky, and should probably be changed later
        for( int i = 1; i < available_actions.first; i++ ) {
            sem_wait( qSem );
        }

        action_queue->releaseFrozenActions( );
    }
}

// Initializes the settingsBuf in the model
void ServerController::initModelCharacters( ServerSettings settings ) {

    // For some reason, the default of false in the protobuf file isn't working
    // So set them all to false ourselves
    model->settingsBuf.set_players( model->num_clients );
    model->settingsBuf.set_vote_track_len( model->vote_track_length );
    model->settingsBuf.set_quest_track_len( model->quest_track_length );
    model->settingsBuf.set_evil_count( settings.game_settings.num_evil_players );
    for( unsigned int i = 0; i < model->players_per_quest.size( ); i++ ) {
        model->settingsBuf.add_players_per_quest( model->players_per_quest[ i ] );
    }
    for( unsigned int i = 0; i < model->fails_per_quest.size( ); i++ ) {
        model->settingsBuf.add_fails_per_quest( model->fails_per_quest[ i ] );
    }
    model->settingsBuf.set_merlin( false );
    model->settingsBuf.set_percival( false );
    model->settingsBuf.set_mordred( false );
    model->settingsBuf.set_morgana( false );
    model->settingsBuf.set_assassin( false );
    model->settingsBuf.set_oberon( false );

    // Figure out which roles we have in the game
    for( std::vector< avalon::special_roles_t >::iterator it = settings.selected_roles.begin(); it != settings.selected_roles.end(); it++ ) {
        switch( *it ) {
            case avalon::MERLIN:
                model->settingsBuf.set_merlin( true );
                break;
            case avalon::PERCIVAL:
                model->settingsBuf.set_percival( true );
                break;
            case avalon::MORDRED:
                model->settingsBuf.set_mordred( true );
                break;
            case avalon::MORGANA:
                model->settingsBuf.set_morgana( true );
                break;
            case avalon::ASSASSIN:
                model->settingsBuf.set_assassin( true );
                break;
            case avalon::OBERON:
                model->settingsBuf.set_oberon( true );
                break;
            case avalon::NONE:
            default:
                break;
        }
    }

}
// Creates Players, and randomizes them around the Players vector
int ServerController::initModelPlayer( ServerSettings settings ) {

    if( settings.selected_roles.size( ) > model->num_clients ) {
        std::cerr << "More special roles than players. Exiting." << std::endl;
        return EXIT_SPECIAL_ERROR;
    }

    std::vector< avalon::special_roles_t > evilChars;
    std::vector< avalon::special_roles_t > goodChars;
    std::vector< avalon::alignment_t > possibleChars;

    // Add the characters from the special_roles array to the good or evil array
    for( std::vector< avalon::special_roles_t >::iterator it = settings.selected_roles.begin(); it != settings.selected_roles.end(); it++ ) {
        if( avalon::getRoleAlignment( *it ) == avalon::EVIL ) {
            evilChars.push_back( *it );
        } else {
            goodChars.push_back( *it );
        }
    }

    // Add the possible characters to the vectors
    unsigned int numEvil = settings.game_settings.num_evil_players;

    if( evilChars.size( ) > numEvil ) {
        std::cerr << "More evil roles than evil players. Exiting." << std::endl;
        return EXIT_EVIL_ERROR;
    }

    if( goodChars.size( ) > (model->num_clients - numEvil ) ) {
        std::cerr << "More good roles than good players. Exiting." << std::endl;
        return EXIT_GOOD_ERROR;
    }

    // possibleChars vector
    for( unsigned int i = 0; i < model->num_clients; i++ ) {
        if( i < numEvil ) {
            possibleChars.push_back( avalon::EVIL );
        } else {
            possibleChars.push_back( avalon::GOOD );
        }
    }

    // evilChars vector
    unsigned int plainEvilChars = numEvil - evilChars.size();
    for( unsigned int i = 0; i < plainEvilChars; i++ ) {
        evilChars.push_back( avalon::NONE );
    }

    // goodChars vector
    unsigned int plainGoodChars = model->num_clients - numEvil - goodChars.size();
    for( unsigned int i = 0; i < plainGoodChars; i++ ) {
        goodChars.push_back( avalon::NONE );
    }

    // Now randomize the vectors
    randomizeVector( possibleChars, model->rng );
    randomizeVector( goodChars, model->rng );
    randomizeVector( evilChars, model->rng );

    for( unsigned int i = 0; i < possibleChars.size(); i++ ) {
        avalon::alignment_t newAlign = possibleChars[ i ];
        avalon::special_roles_t newSpecial;

        if( newAlign == avalon::GOOD ) {
            newSpecial = goodChars.back();
            goodChars.pop_back();
        } else {
            newSpecial = evilChars.back();
            evilChars.pop_back();
        }

        std::string newName = "Player ";
        newName += std::to_string( i + 1 );

        model->players.push_back( new Player( newName, newSpecial, newAlign ) );
    }

    // Pick a leader
    model->leader = ( *model->rng )( ) % model->num_clients;

    return EXIT_SUCCESS;
}
