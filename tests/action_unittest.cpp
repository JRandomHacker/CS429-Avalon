/**
 * Tests for player.cpp
 * 
 * @author Justin Koehler && Ryan Kerr
 * @date 2015-02-16
 * @file player_unittest.cpp
 */

#include "ActionHandler.hpp"
#include "Action.hpp"
#include "gtest/gtest.h"

#include <string>

class ActionTests : public ::testing::Test {
public:
};

TEST_F( ActionTests, testActionClassesExist) {
    ActionHandler ah(NULL);
    Action a("invalidActionType");
}
