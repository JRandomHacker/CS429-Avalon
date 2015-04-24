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


}
