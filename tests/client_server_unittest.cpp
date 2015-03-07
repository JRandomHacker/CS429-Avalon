/**
 * Tests for client.cpp and server.cpp
 * (Must be done together since they are networking with each other)
 * 
 * @author Ryan Kerr
 * @date 2015-02-16
 * @file client_server_unittest.cpp
 */

#include "server.hpp"
#include "client.hpp"
#include "globals.hpp"
#include "ActionHandler.hpp"
#include "Action.hpp"

#include "gtest/gtest.h"

#include <vector>
#include <string>
#include <semaphore.h>

class ClientServerTestFixture: public ::testing::Test {

    public:
        // Variables you want your tests to access must be public
        Client* testClient = NULL;
        Server* testServer = NULL;
        ActionHandler* testQueue;
        sem_t testSemaphore;

        // Run before every test
        void SetUp( void ) {
            sem_init( &testSemaphore, 0, 0 );
            testQueue = new ActionHandler( &testSemaphore );
        }

        // Run after every test
        void TearDown( void ) {
            if( testClient != NULL ) {
                delete testClient;
                testClient = NULL;
            }
            if( testServer != NULL ) {
                delete testServer;
                testServer = NULL;
            }

            delete testQueue;
            testQueue = NULL;

            sem_destroy( &testSemaphore );
        }

        void startServerListening( void ) {

            pthread_t networkThread;
            pthread_create( &networkThread, NULL, listeningThreadFunc, testServer );
            pthread_detach( networkThread );
        }

        static void* listeningThreadFunc( void* targs ) {

            Server* serv = ( Server* )targs;
            serv->waitForClients( );
            pthread_exit( NULL );
            return NULL;
        }
};

//TEST_F( ClientServerTestFixture, ClientCanConnect ) {
//    std::vector< avalon::special_roles_t > special_roles;
//    testServer = new Server( 1, special_roles, DEFAULT_PORT );
//
//    startServerListening( );
//    testClient = new Client( "localhost", DEFAULT_PORT );
//    testClient->initQueue( testQueue );
//    testClient->waitForData( );
//
//    std::pair< int, std::list< Action* >::iterator > ret_actions = testQueue->FreezeFrontActions( );
//    int retVal = ret_actions.first;
//    testQueue->ReleaseFrozenActions( );
//
//    ASSERT_GT( retVal, 0 );
//}

TEST_F( ClientServerTestFixture, TooManyEvil ) {

    std::vector< avalon::special_roles_t > special_roles;
    special_roles.push_back( avalon::MORGANA );
    special_roles.push_back( avalon::MORDRED );

    ASSERT_EXIT( new Server( 1, special_roles, DEFAULT_PORT ), 
            ::testing::ExitedWithCode( EXIT_EVIL_ERROR ),
            "More evil roles than evil players. Exiting." );
}

TEST_F( ClientServerTestFixture, InvalidPort ) {

    std::vector< avalon::special_roles_t > special_roles;

    ASSERT_EXIT( new Server( 1, special_roles, 33345346 ), 
            ::testing::ExitedWithCode( EXIT_NETWORK_ERROR ),
            "Invalid port number" );
}

TEST_F( ClientServerTestFixture, BoundRootSocket ) {
    std::vector< avalon::special_roles_t > special_roles;

    ASSERT_EXIT( new Server( 1, special_roles, 443 ),
            ::testing::ExitedWithCode( EXIT_SOCKET_ERROR ),
           "Unable to bind network socket" );
}

TEST_F( ClientServerTestFixture, BoundSocketTwice ) {
    std::vector< avalon::special_roles_t > special_roles;

    testServer = new Server( 1, special_roles, DEFAULT_PORT );

    ASSERT_EXIT( new Server( 1, special_roles, DEFAULT_PORT ),
            ::testing::ExitedWithCode( EXIT_SOCKET_ERROR ),
            "Unable to bind network socket" );
}
