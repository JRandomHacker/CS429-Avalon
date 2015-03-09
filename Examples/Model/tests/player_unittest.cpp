/**
 * Tests for player.cpp
 *
 * @author Justin Koehler && Ryan Kerr
 * @date 2015-02-16
 * @file player_unittest.cpp
 */

#include "player.hpp"
#include "gtest/gtest.h"

#include <string>
#include "globals.hpp"

class PlayerTestFixture: public ::testing::Test {

    public:
        // Variables you want your tests to access must be public
        Player* testPlayer;

        // Run before every test
        void SetUp( void ) {

            testPlayer = new Player( "Test name", avalon::MORGANA, avalon::EVIL );
        }

        // Run after every test
        void TearDown( void ) {

            delete testPlayer;
            testPlayer = NULL;
        }
};

TEST_F( PlayerTestFixture, HasName ) {
    ASSERT_STREQ( "Test name", testPlayer->getName().c_str() );
}

TEST_F( PlayerTestFixture, isCorrectRole ) {
    ASSERT_EQ( avalon::MORGANA, testPlayer->getRole() );
}

TEST_F( PlayerTestFixture, isCorrectAlignment ) {
    ASSERT_EQ( avalon::EVIL, testPlayer->getAlignment() );
}
