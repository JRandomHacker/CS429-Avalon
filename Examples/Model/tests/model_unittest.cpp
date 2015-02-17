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

    // Ensure the subscribers had their destruction method called and no longer
    // have references to the model data

    ASSERT_TRUE(sub1.wasDestroyed);
    ASSERT_EQ((ModelData*)NULL, sub1.getModelData());
    ASSERT_TRUE(sub2.wasDestroyed);
    ASSERT_EQ((ModelData*)NULL, sub2.getModelData());
}

TEST_F( ModelTests, testAbleToUpdateAndRetrieveData) {
    MockSubscriber sub1;
    MockSubscriber sub2;

    // Adds data to subscribe to
    model.addData("data_name", 0);

    // Successfully subscribe to the new data
    ASSERT_TRUE(model.subscribe("data_name", &sub1));

    // Ensure that the subscriber can access the data
    ASSERT_EQ(0, sub1.getModelData()->getPayload());

    // Updates the data and ensures that the subscriber is still accurate and
    // that its update method was called
    model.updateData("data_name", 1);
    ASSERT_EQ(1, sub1.getModelData()->getPayload());
    ASSERT_EQ(1, sub1.updatedCount);

    // Adds a second subscriber
    ASSERT_TRUE(model.subscribe("data_name", &sub2));
    ASSERT_EQ(1, sub2.getModelData()->getPayload());

    // Updates the data and ensures that both subscribers were updated
    model.updateData("data_name", 2);
    ASSERT_EQ(2, sub1.getModelData()->getPayload());
    ASSERT_EQ(2, sub1.updatedCount);
    ASSERT_EQ(2, sub2.getModelData()->getPayload());
    ASSERT_EQ(1, sub2.updatedCount);

    // Since the subscribers are being destroyed they need to be freed
    ASSERT_TRUE(model.unsubscribe("data_name", &sub1));
    ASSERT_TRUE(model.unsubscribe("data_name", &sub2));
}

TEST_F( ModelTests, testDeletingDataRemovesSubscribers) {
    MockSubscriber sub1;
    
    // Adds data to subscribe to
    model.addData("data_name", 0);

    // Successfully subscribe to the new data
    ASSERT_TRUE(model.subscribe("data_name", &sub1));

    // Remove the data from the model
    ASSERT_TRUE(model.removeData("data_name"));

    // Ensure that the subscriber now no longer has access to the data and that
    // its destruction method was called
    ASSERT_EQ((ModelData*)NULL, sub1.getModelData());
    ASSERT_TRUE(sub1.wasDestroyed);
}

TEST_F( ModelTests, testModelDestructed) {
    MockSubscriber sub1;

    // Creates a model that can be conveniently destructed on command
    Model* destroyable_model = new Model();

    // Adds data to subscribe to
    destroyable_model->addData("data_name", 0);

    // Successfully subscribe to the new data
    ASSERT_TRUE(destroyable_model->subscribe("data_name", &sub1));

    // Destroy the model
    delete destroyable_model;

    // Ensure that the subscriber now no longer has access to the data and that
    // its destruction method was called
    ASSERT_EQ((ModelData*)NULL, sub1.getModelData());
    ASSERT_TRUE(sub1.wasDestroyed);
}

TEST_F( ModelTests, testClosureSubscriber) {
    int subscriber_num_updates = 0;
    int subscriber_latest_value = 0;
    bool subscriber_destroyed = 0;
    ClosureSubscriber sub(
        [&](Subscriber* s){
            subscriber_num_updates++;
            subscriber_latest_value = s->getModelData()->getPayload();
        },
        [&](Subscriber* s){ subscriber_destroyed = true; });

    model.addData("data_name", 0);
    model.subscribe("data_name", &sub);

    model.updateData("data_name", 5);

    ASSERT_EQ(1, subscriber_num_updates);
    ASSERT_EQ(5, subscriber_latest_value);

    model.removeData("data_name");

    ASSERT_TRUE(subscriber_destroyed);
}