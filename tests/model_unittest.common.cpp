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

class ConDestructTester {
public:
    static int constcount, destcount, copycount;

    ConDestructTester() { constcount++; }
    ~ConDestructTester() {
        destcount++;
    }
    ConDestructTester(const ConDestructTester& that) { copycount++; }
};
int ConDestructTester::constcount = 0;
int ConDestructTester::destcount = 0;
int ConDestructTester::copycount = 0;

TEST_F( ModelTests, testAddingData ) {
    // Adds data successfully twice, but then fails by trying to duplicate a
    // data name.
    ASSERT_TRUE(model.addData<int>("data_name", 0));
    ASSERT_TRUE(model.addData<bool>("second_data_name", false));
    ASSERT_FALSE(model.addData<int>("data_name", 0));
}

TEST_F( ModelTests, testRetrievingData ) {
    // Adds data
    ASSERT_TRUE( model.addData< int >( "data_name", 0 ) );
    ASSERT_TRUE( model.addData< bool >( "second_data_name", false ) );

    // Ensures that the data is referenced correctly, and that attempting to
    // get data with either a bad id or a wrong type gets NULL
    ASSERT_EQ( 0, *model.referenceData< int >( "data_name" ) );
    ASSERT_EQ( false, *model.referenceData< bool >( "second_data_name" ) );
    ASSERT_EQ( NULL, model.referenceData< unsigned int >( "data_name" ) );
    ASSERT_EQ( NULL, model.referenceData< bool >( "not a data name" ) );

    // Does an update and gets a new reference, ensuring its still correct
    ASSERT_TRUE( model.updateData< int >( "data_name", 10 ) );
    ASSERT_EQ( 10, *model.referenceData< int >( "data_name" ) );
}

TEST_F( ModelTests, testAddingConstructedData ) {
    ConDestructTester::constcount = 0;
    ConDestructTester::destcount = 0;
    ConDestructTester::copycount = 0;

    Model* mod = new Model();

    // Adds data that has a constructor and a destructor
    ConDestructTester cdt;
    EXPECT_EQ(1, ConDestructTester::constcount);
    EXPECT_EQ(0, ConDestructTester::destcount);
    EXPECT_EQ(0, ConDestructTester::copycount);

    ASSERT_TRUE(mod->addData<ConDestructTester>("constructed_data", cdt));
    EXPECT_EQ(1, ConDestructTester::constcount);
    EXPECT_EQ(0, ConDestructTester::destcount);
    EXPECT_EQ(1, ConDestructTester::copycount);

    ASSERT_TRUE(mod->removeData("constructed_data"));
    EXPECT_EQ(1, ConDestructTester::constcount);
    EXPECT_EQ(1, ConDestructTester::destcount);
    EXPECT_EQ(1, ConDestructTester::copycount);

    ConDestructTester::constcount = 0;
    ConDestructTester::destcount = 0;
    ConDestructTester::copycount = 0;

    ConDestructTester* cdt2 = new ConDestructTester();
    EXPECT_EQ(1, ConDestructTester::constcount);
    EXPECT_EQ(0, ConDestructTester::destcount);
    EXPECT_EQ(0, ConDestructTester::copycount);

    ASSERT_TRUE(mod->addData<ConDestructTester>("constructed_data", *cdt2));
    EXPECT_EQ(1, ConDestructTester::constcount);
    EXPECT_EQ(0, ConDestructTester::destcount);
    EXPECT_EQ(1, ConDestructTester::copycount);

    delete mod;
    EXPECT_EQ(1, ConDestructTester::constcount);
    EXPECT_EQ(1, ConDestructTester::destcount);
    EXPECT_EQ(1, ConDestructTester::copycount);
}

TEST_F( ModelTests, testAbleToSubscribeWithData) {
    MockSubscriber sub1;
    MockSubscriber sub2;

    // Fails to subscribe as there is no data named garbage to subscribe to
    ASSERT_FALSE(model.subscribe("garbage", &sub1));

    model.addData<int>("data_name", 0);

    // Successfully subscribe to the new data
    ASSERT_TRUE(model.subscribe("data_name", &sub1));
    ASSERT_TRUE(model.subscribe("data_name", &sub2));

    // Ensure that the subscribers' data reference was set
    ASSERT_NE((int*)NULL, sub1.getData<int>());
    ASSERT_EQ(sub1.getData<int>(), sub2.getData<int>());

    // Since the subscribers are being destroyed they need to be freed
    ASSERT_TRUE(model.unsubscribe("data_name", &sub1));
    ASSERT_TRUE(model.unsubscribe("data_name", &sub2));

    // Ensure the subscribers had their destruction method called and no longer
    // have references to the model data

    ASSERT_TRUE(sub1.wasDestroyed);
    ASSERT_EQ((int*)NULL, sub1.getData<int>());
    ASSERT_TRUE(sub2.wasDestroyed);
    ASSERT_EQ((int*)NULL, sub2.getData<int>());
}

TEST_F( ModelTests, testAbleToUpdateAndRetrieveData) {
    MockSubscriber sub1;
    MockSubscriber sub2;

    // Adds data to subscribe to
    model.addData<int>("data_name", 0);

    // Successfully subscribe to the new data
    ASSERT_TRUE(model.subscribe("data_name", &sub1));

    // Ensure that the subscriber can access the data
    ASSERT_EQ(0, *sub1.getData<int>());

    // Updates the data and ensures that the subscriber is still accurate and
    // that its update method was called
    model.updateData<int>("data_name", 1);
    ASSERT_EQ(1, *sub1.getData<int>());
    ASSERT_EQ(1, sub1.updatedCount);

    // Adds a second subscriber
    ASSERT_TRUE(model.subscribe("data_name", &sub2));
    ASSERT_EQ(1, *sub2.getData<int>());

    // Updates the data and ensures that both subscribers were updated
    model.updateData<int>("data_name", 2);
    ASSERT_EQ(2, *sub1.getData<int>());
    ASSERT_EQ(2, sub1.updatedCount);
    ASSERT_EQ(2, *sub2.getData<int>());
    ASSERT_EQ(1, sub2.updatedCount);

    // Since the subscribers are being destroyed they need to be freed
    ASSERT_TRUE(model.unsubscribe("data_name", &sub1));
    ASSERT_TRUE(model.unsubscribe("data_name", &sub2));
}

TEST_F( ModelTests, testAbleToUpdateAndRetrieveDataWithoutReplacement) {
    MockSubscriber sub1;
    MockSubscriber sub2;

    // Adds data to subscribe to
    model.addData<int>("data_name", 0);

    // Successfully subscribe to the new data
    ASSERT_TRUE(model.subscribe("data_name", &sub1));

    // Ensure that the subscriber can access the data
    ASSERT_EQ(0, *sub1.getData<int>());

    // Updates the data by reference rather than replacement and ensures that
    // the subscriber is still accurate and that its update method was called
    int* data_ref1 = model.getDataForUpdate<int>("data_name");
    *data_ref1 = 1;
    model.flagDataForUpdate("data_name");
    ASSERT_EQ(1, *sub1.getData<int>());
    ASSERT_EQ(1, sub1.updatedCount);

    // Adds a second subscriber
    ASSERT_TRUE(model.subscribe("data_name", &sub2));
    ASSERT_EQ(1, *sub2.getData<int>());

    // Updates the data and ensures that both subscribers were updated
    int* data_ref2 = model.getDataForUpdate<int>("data_name");
    *data_ref2 = 2;
    model.flagDataForUpdate("data_name");
    ASSERT_EQ(2, *sub1.getData<int>());
    ASSERT_EQ(2, sub1.updatedCount);
    ASSERT_EQ(2, *sub2.getData<int>());
    ASSERT_EQ(1, sub2.updatedCount);

    // Since the subscribers are being destroyed they need to be freed
    ASSERT_TRUE(model.unsubscribe("data_name", &sub1));
    ASSERT_TRUE(model.unsubscribe("data_name", &sub2));
}

TEST_F( ModelTests, testDeletingDataRemovesSubscribers) {
    MockSubscriber sub1;

    // Adds data to subscribe to
    model.addData<int>("data_name", 0);

    // Successfully subscribe to the new data
    ASSERT_TRUE(model.subscribe("data_name", &sub1));

    // Remove the data from the model
    ASSERT_TRUE(model.removeData("data_name"));

    // Ensure that the subscriber now no longer has access to the data and that
    // its destruction method was called
    ASSERT_EQ((int*)NULL, sub1.getData<int>());
    ASSERT_TRUE(sub1.wasDestroyed);
}

TEST_F( ModelTests, testModelDestructed) {
    MockSubscriber sub1;

    // Creates a model that can be conveniently destructed on command
    Model* destroyable_model = new Model();

    // Adds data to subscribe to
    destroyable_model->addData<int>("data_name", 0);

    // Successfully subscribe to the new data
    ASSERT_TRUE(destroyable_model->subscribe("data_name", &sub1));

    // Destroy the model
    delete destroyable_model;

    // Ensure that the subscriber now no longer has access to the data and that
    // its destruction method was called
    ASSERT_EQ((int*)NULL, sub1.getData<int>());
    ASSERT_TRUE(sub1.wasDestroyed);
}

TEST_F( ModelTests, testSubscribersUnsubscribeOnDeconstruction ) {
    // Create a model and subscriber and pair the two
    MockSubscriber* sub = new MockSubscriber();
    Model* mod = new Model();
    mod->addData<int>("data_name", 0);
    mod->subscribe("data_name", (Subscriber*)sub);

    // Delete the subscriber, which should unsubscribe it
    delete sub;

    // Assert that the unsubscription fails as subscriber should be unpaired
    ASSERT_FALSE(mod->unsubscribe("data_name", (Subscriber*)sub));

    delete mod;
}

TEST_F( ModelTests, testClosureSubscriber) {
    int subscriber_num_updates = 0;
    int subscriber_latest_value = 0;
    bool subscriber_destroyed = 0;
    ClosureSubscriber sub(
        [&](Subscriber* s){
            subscriber_num_updates++;
            subscriber_latest_value = *s->getData<int>();
        },
        [&](Subscriber* s){ subscriber_destroyed = true; });

    model.addData<int>("data_name", 0);
    model.subscribe("data_name", &sub);

    model.updateData<int>("data_name", 5);

    ASSERT_EQ(1, subscriber_num_updates);
    ASSERT_EQ(5, subscriber_latest_value);

    model.removeData("data_name");

    ASSERT_TRUE(subscriber_destroyed);
}

TEST_F( ModelTests, testClosureSubscriberAutoFree ) {
    ClosureSubscriber* sub = new ClosureSubscriber(
        [&](Subscriber* s){},
        [&](Subscriber* s){}, true);

    model.addData<int>("data_name", 0);
    model.subscribe("data_name", sub);

    model.updateData<int>("data_name", 5);

    model.removeData("data_name");
}