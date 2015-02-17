/**
 * Tests for player.cpp
 * 
 * @author Justin Koehler && Ryan Kerr
 * @date 2015-02-16
 * @file player_unittest.cpp
 */

#include "globals.hpp"
#include "gtest/gtest.h"

#include <string>

TEST( GlobalTestSuite, areEvilRolesEvil) {
    EXPECT_EQ( avalon::EVIL, avalon::getRoleAlignment( avalon::MORGANA ) );
    EXPECT_EQ( avalon::EVIL, avalon::getRoleAlignment( avalon::ASSASSIN ) );
    EXPECT_EQ( avalon::EVIL, avalon::getRoleAlignment( avalon::MORDRED ) );
    EXPECT_EQ( avalon::EVIL, avalon::getRoleAlignment( avalon::OBERON ) );
    
    EXPECT_NE( avalon::EVIL, avalon::getRoleAlignment( avalon::PERCIVAL ) );
    EXPECT_NE( avalon::EVIL, avalon::getRoleAlignment( avalon::MERLIN ) );
}

TEST( GlobalTestSuite, areGoodRolesGood) {
    EXPECT_EQ( avalon::GOOD, avalon::getRoleAlignment( avalon::MERLIN ) );
    EXPECT_EQ( avalon::GOOD, avalon::getRoleAlignment( avalon::PERCIVAL ) );
    
    EXPECT_NE( avalon::GOOD, avalon::getRoleAlignment( avalon::MORDRED ) );
    EXPECT_NE( avalon::GOOD, avalon::getRoleAlignment( avalon::OBERON ) );
    EXPECT_NE( avalon::GOOD, avalon::getRoleAlignment( avalon::MORGANA ) );
    EXPECT_NE( avalon::GOOD, avalon::getRoleAlignment( avalon::ASSASSIN ) );
}

TEST( GlobalTestSuite, correctEvilCount ) {
    EXPECT_EQ( 2, avalon::getEvilCount( 5 ) );
    EXPECT_EQ( 2, avalon::getEvilCount( 6 ) );
    EXPECT_EQ( 3, avalon::getEvilCount( 7 ) );
    EXPECT_EQ( 3, avalon::getEvilCount( 8 ) );
    EXPECT_EQ( 3, avalon::getEvilCount( 9 ) );
    EXPECT_EQ( 4, avalon::getEvilCount( 10 ) );
}
