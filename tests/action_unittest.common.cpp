/**
 * Tests for ActionHandler.cpp && Action.cpp
 *
 * @author Mohammad Askari && Matt Hoffman
 * @date 2015-03-06
 * @file action_unittest.cpp
 */

#include "actionHandler.hpp"
#include "action.hpp"
#include "clientCustomActionsFromNet.hpp"
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

    Action* a1 = new Action("CreateGame");
    ASSERT_TRUE(a1->getMessage() == "CreateGame");

    Action* a2 = new Action("JoinGame");
    ASSERT_TRUE(a2->getMessage() == "JoinGame");

    auto p = ahandler.freezeFrontActions();
    ASSERT_EQ(std::get<0>(p), 0);
    ahandler.releaseFrozenActions();

    ahandler.addAction(a1);
    ahandler.addAction(a2);

    p = ahandler.freezeFrontActions();
    ASSERT_EQ(std::get<0>(p), 2);
    ahandler.releaseFrozenActions();

    p = ahandler.freezeFrontActions();
    ASSERT_EQ(std::get<0>(p), 0);
    ahandler.releaseFrozenActions();
}
