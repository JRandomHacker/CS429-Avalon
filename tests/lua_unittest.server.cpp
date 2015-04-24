/**
 * Tests for serverControllerState.cpp
 *
 * @author Justin Koehler
 * @date 2015-3-17
 * @file serverControllerState_unittest.cpp
 */

#include "serverSettings.hpp"

#include "gtest/gtest.h"

#include <string>
#include <vector>
#include <random>

class LuaTestFixture: public ::testing::Test {

    public:
};

TEST_F( LuaTestFixture, TestLoadLua1Player ) {
    GameSettings gs;
    int result = gs.loadFromFile( "../src/config/settings.lua", 1 );

    ASSERT_EQ( EXIT_SUCCESS, result );

    ASSERT_EQ( std::vector< unsigned int >({{ 1, 1, 1, 1, 1}}), gs.players_per_quest );
    ASSERT_EQ( std::vector< unsigned int >({{ 1, 1, 1, 1, 1}}), gs.fails_per_quest );
    ASSERT_EQ( 1, gs.num_evil_players );
    ASSERT_EQ( 5, gs.quest_track_length );
    ASSERT_EQ( 5, gs.vote_track_length );
}

TEST_F( LuaTestFixture, TestLoadLua5Player ) {
    GameSettings gs;
    int result = gs.loadFromFile( "../src/config/settings.lua", 5 );

    ASSERT_EQ( EXIT_SUCCESS, result );

    ASSERT_EQ( std::vector< unsigned int >({{ 2, 3, 2, 3, 3}}), gs.players_per_quest );
    ASSERT_EQ( std::vector< unsigned int >({{ 1, 1, 1, 1, 1}}), gs.fails_per_quest );
    ASSERT_EQ( 2, gs.num_evil_players );
    ASSERT_EQ( 5, gs.quest_track_length );
    ASSERT_EQ( 5, gs.vote_track_length );
}


TEST_F( LuaTestFixture, TestLoadLua10Player ) {
    GameSettings gs;
    int result = gs.loadFromFile( "../src/config/settings.lua", 10 );

    ASSERT_EQ( EXIT_SUCCESS, result );

    ASSERT_EQ( std::vector< unsigned int >({{ 3, 4, 4, 5, 5}}), gs.players_per_quest );
    ASSERT_EQ( std::vector< unsigned int >({{ 1, 1, 1, 2, 1}}), gs.fails_per_quest );
    ASSERT_EQ( 4, gs.num_evil_players );
    ASSERT_EQ( 5, gs.quest_track_length );
    ASSERT_EQ( 5, gs.vote_track_length );
}
