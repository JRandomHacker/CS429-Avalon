/**
 * Tests for player.cpp
 * 
 * @author Justin Koehler && Ryan Kerr
 * @date 2015-02-16
 * @file player_unittest.cpp
 */

#include "model.hpp"
#include "gtest/gtest.h"

#include <string>

class ModelTests : public ::testing::Test {
public:
    void enterVotingState();

    Model model;
};

void ModelTests::enterVotingState() {

}

TEST_F( ModelTests, testAddingData) {
    // Adds data successfully twice, but then fails by trying to duplicate a
    // data name.
    ASSERT_TRUE(model.addData("data_name", 0));
    ASSERT_TRUE(model.addData("second_data_name", 0));
    ASSERT_FALSE(model.addData("data_name", 0));
}

TEST_F( ModelTests, testAbleToSubscribeWithData) {
    MockSubscriber sub1;
    MockSubscriber sub2;

    // Fails to subscribe as there is no data named garbage to subscribe to
    ASSERT_FALSE(model.subscribe("garbage", &sub1));

    model.addData("data_name", 0);

    // Successfully subscribe to the new data
    ASSERT_TRUE(model.subscribe("data_name", &sub1));
    ASSERT_TRUE(model.subscribe("data_name", &sub2));

    // Ensure that the subscribers' data reference was set
    ASSERT_NE((ModelData*)NULL, sub1.getModelData());
    ASSERT_EQ(sub1.getModelData(), sub2.getModelData());

    // Since the subscribers are being destroyed they need to be freed
    ASSERT_TRUE(model.unsubscribe("data_name", &sub1));
    ASSERT_TRUE(model.unsubscribe("data_name", &sub2));

    // Ensure the subscribers had their destruction method called
    ASSERT_TRUE(sub1.wasDestroyed);
    ASSERT_TRUE(sub2.wasDestroyed);
}
