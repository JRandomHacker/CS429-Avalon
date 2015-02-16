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

            testPlayer = new Player("Test name", MORGANA, EVIL);
        }

        // Run after every test
        void TearDown( void ) {

            delete testPlayer;
            testPlayer = NULL;
        }
};

TEST_F(PlayerTestFixture, HasName) {
	ASSERT_STREQ("Test name", testPlayer->getName().c_str());
}
