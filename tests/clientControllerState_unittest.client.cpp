/**
 * Tests for clientControllerState.cpp
 *
 * @author Justin Koehler
 * @date 2015-03-16
 * @file clientControllerState_unittest.cpp
 */

#include "clientControllerState.hpp"
#include "model.hpp"
#include "mockClient.hpp"
#include "clientInfo.hpp"
#include "clientCustomActionsFromGUI.hpp"
#include "vote.pb.h"

#include "gtest/gtest.h"

#include <string>

class ClientControllerStateTestFixture: public ::testing::Test {

    public:
        // Variables you want your tests to access must be public
        MockClient* testClient = NULL;
		Model* testModel = NULL;
		ClientInfo* testInfo = NULL;

        // Run before every test
        void SetUp( void ) {
			testClient = new MockClient( );
			testModel = new Model( );
			testInfo = new ClientInfo;
			
			testInfo->client = testClient;
			testInfo->model = testModel;
			testInfo->my_id = 3;
        }

        // Run after every test
        void TearDown( void ) {
            if( testClient != NULL ) {
                delete testClient;
                testClient = NULL;
            }
            if( testModel != NULL ) {
                delete testModel;
                testModel = NULL;
            }
			if( testInfo != NULL ) {
                delete testInfo;
                testInfo = NULL;
            }

        }
};

TEST_F( ClientControllerStateTestFixture, TestVotingStateVoteAction ) {
	VotingState testState( testInfo );
	
	Action* testAction = new TeamVoteAction( avalon::YES );
	ControllerState* resultState = testState.handleAction( testAction );
	
	ASSERT_EQ( NULL, resultState );
	ASSERT_EQ( avalon::network::VOTE_BUF, testClient->getLastBufType( ) );
	
	std::string sentBuf = testClient->getLastProtobuf( );
	avalon::network::Vote sentVote;
	sentVote.ParseFromString( sentBuf );
	
	ASSERT_EQ( avalon::YES, sentVote.vote( ) );
	ASSERT_EQ( 3, sentVote.id( ) );
}
