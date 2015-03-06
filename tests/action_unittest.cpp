/**
 * Tests for ActionHandler.cpp && Action.cpp
 *
 * @author Mohammad Askari && Matt Hoffman
 * @date 2015-03-06
 * @file action_unittest.cpp
 */

#include "ActionHandler.hpp"
#include "Action.hpp"
#include "CustomActions.hpp"
#include "gtest/gtest.h"

#include <string>
#include <semaphore.h>

class ActionTests : public ::testing::Test {
public:
};

TEST_F( ActionTests, testActionClassesExist) {
    ActionHandler ah(NULL);
    Action a("invalidActionType");
}

TEST_F( ActionTests, testAddingActions) {
    sem_t mut;
    sem_init(&mut, 1, 1);
    ActionHandler ahandler(&mut);
    Action act("CreateGame");
    ASSERT_TRUE(act.GetMessage() == "CreateGame");
    Action act2("JoinGame");
    ASSERT_TRUE(act2.GetMessage() == "JoinGame");
    ahandler.AddAction(&act);
    ahandler.AddAction(&act2);
    ASSERT_EQ(2,2);
    auto p = ahandler.FreezeFrontActions();
    ASSERT_EQ(std::get<0>(p), 2);
    ahandler.ReleaseFrozenActions();
    ASSERT_EQ(2,2);
    p = ahandler.FreezeFrontActions();
    ASSERT_EQ(std::get<0>(p), 0);
}

/*
TEST_F( ActionTests, testCustomActions) {
    GameSettings settings;
    GameSettingsAction gsact(&settings);
    ASSERT_TRUE(gsact.GetMessage() == "GameSettings");
    ASSERT_EQ(gsact.getSettings(), &settings);
    Player play;
    AddPlayerAction plact(2, &play);
    ASSERT_TRUE(plact.GetMessage() == "AddPlayer");
    ASSERT_EQ(plact.getPlayerNumber(), 2);
    ASSERT_EQ(plact.getPlayerInfo(), &play);
}
*/
