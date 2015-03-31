/**
 * Tests for clientControllerState.cpp
 *
 * @author Justin Koehler
 * @date 2015-03-16
 * @file clientControllerState_unittest.cpp
 */

#include "clientControllerState.hpp"
#include "model.hpp"
#include "voteHistory.hpp"
#include "mockClient.hpp"
#include "clientInfo.hpp"
#include "clientCustomActionsFromGUI.hpp"
#include "clientCustomActionsFromNet.hpp"
#include "vote.pb.h"
#include "teamselection.pb.h"
#include "settings.pb.h"
#include "player.pb.h"
#include "subscriber.hpp"
#include "player.hpp"

#include "gtest/gtest.h"

#include <string>
#include <vector>
#include <climits>

class ClientControllerStateTestFixture: public ::testing::Test {

    public:
        // Variables you want your tests to access must be public
        MockClient* testClient = NULL;
        Model* testModel = NULL;
        ClientInfo* testInfo = NULL;
        Action* testAction = NULL;

        // Run before every test
        void SetUp( void ) {
            testClient = new MockClient( );
            testModel = new Model( );
            testInfo = new ClientInfo;

            testInfo->client = testClient;
            testInfo->model = testModel;
            testInfo->model->addData< unsigned int >( "myID", 3 );
            testInfo->model->addData< unsigned int >( "leaderID", 2 );
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
            if( testAction != NULL ) {
                delete testAction;
                testAction = NULL;
            }

        }
};

TEST_F( ClientControllerStateTestFixture, TestVotingStateVoteAction ) {
    avalon::client::VotingState testState( testInfo );

    testAction = new TeamVoteAction( avalon::YES );
    ControllerState* resultState = testState.handleAction( testAction );

    ASSERT_EQ( NULL, resultState );
    ASSERT_EQ( avalon::network::VOTE_BUF, testClient->getLastBufType( ) );

    std::string sentBuf = testClient->getLastProtobuf( );
    avalon::network::Vote sentVote;
    sentVote.ParseFromString( sentBuf );

    ASSERT_EQ( avalon::YES, sentVote.vote( ) );
    ASSERT_EQ( 3, sentVote.id( ) );
}

TEST_F( ClientControllerStateTestFixture, TestVotingStateResultsAction ) {
    avalon::client::VotingState testState( testInfo );

    testModel->addData< unsigned int >( "currentVoteTrack" , 0);
    testModel->addData< unsigned int >( "voteTrackLength", 5 );
    testModel->addData< unsigned int >( "questTrackLength", 5 );
    testModel->addData( "questingTeam", std::vector< unsigned int >( ) );
    testModel->addData( "voteHistory", std::vector< VoteHistory >( ) );
    std::vector< avalon::player_vote_t >* testVotes = new std::vector< avalon::player_vote_t >( );
    testVotes->push_back( avalon::YES );
    testVotes->push_back( avalon::NO );
    testVotes->push_back( avalon::YES );
    testAction = new VoteResultsAction( true, 2, testVotes );
    ControllerState* resultState = testState.handleAction( testAction );

    ASSERT_EQ( NULL, resultState );

    ClosureSubscriber* sub = new ClosureSubscriber( [&](Subscriber*){ }, [&](Subscriber*){ } );
    testModel->subscribe( "currentVoteTrack", sub );

    ASSERT_EQ( 2, *( sub->getData< unsigned int >( ) ) );

    delete sub;
}

TEST_F( ClientControllerStateTestFixture, TestVotingStateTeamStateAction ) {
    avalon::client::VotingState testState( testInfo );

    std::vector< unsigned int > testTeam { 2, 3, 5 };
    testInfo->model->addData( "questingTeam", testTeam );

    testAction = new EnterTeamSelectionAction( 4 );
    ControllerState* resultState = testState.handleAction( testAction );

    ASSERT_EQ( "TeamSelection", resultState->getType( ) );
    ASSERT_EQ( 4, *testInfo->model->referenceData< unsigned int >( "leaderID" ) );
    ASSERT_EQ( 0, testInfo->model->referenceData< std::vector< unsigned int > >( "questingTeam" )->size( ) );

}

TEST_F( ClientControllerStateTestFixture, TestTeamStateSelectActionNotLeader ) {
    avalon::client::TeamSelectionState testState( testInfo );

    testAction = new SelectQuestGoerAction( true, 1 );
    ControllerState* resultState = testState.handleAction( testAction );

    ASSERT_EQ( NULL, resultState );
    ASSERT_EQ( "", testClient->getLastProtobuf( ) );
}

TEST_F( ClientControllerStateTestFixture, TestTeamStateSelectActionLeader ) {
    avalon::client::TeamSelectionState testState( testInfo );

    testInfo->model->updateData< unsigned int >( "leaderID", 3 );

    testAction = new SelectQuestGoerAction( true, 1 );
    ControllerState* resultState = testState.handleAction( testAction );

    ASSERT_EQ( NULL, resultState );
    ASSERT_EQ( avalon::network::TEAM_SELECTION_BUF, testClient->getLastBufType( ) );

    std::string sentBuf = testClient->getLastProtobuf( );
    avalon::network::TeamSelection sentSelection;
    sentSelection.ParseFromString( sentBuf );

    ASSERT_EQ( true, sentSelection.selected( ) );
    ASSERT_EQ( 1, sentSelection.id( ) );
}

TEST_F( ClientControllerStateTestFixture, TestTeamStateFinalizeActionNotLeader ) {
    avalon::client::TeamSelectionState testState( testInfo );

    testAction = new FinalizeTeamAction( );
    ControllerState* resultState = testState.handleAction( testAction );

    ASSERT_EQ( NULL, resultState );
    ASSERT_EQ( "", testClient->getLastProtobuf( ) );
}

TEST_F( ClientControllerStateTestFixture, TestTeamStateFinalizeActionLeader ) {
    avalon::client::TeamSelectionState testState( testInfo );

    testInfo->model->updateData< unsigned int >( "leaderID", 3 );

    testAction = new FinalizeTeamAction( );
    ControllerState* resultState = testState.handleAction( testAction );

    ASSERT_EQ( NULL, resultState );
    ASSERT_EQ( avalon::network::TEAM_SELECTION_BUF, testClient->getLastBufType( ) );

    std::string sentBuf = testClient->getLastProtobuf( );
    avalon::network::TeamSelection sentSelection;
    sentSelection.ParseFromString( sentBuf );

    ASSERT_EQ( true, sentSelection.finished( ) );
}

TEST_F( ClientControllerStateTestFixture, TestTeamStateModifyAction ) {
    avalon::client::TeamSelectionState testState( testInfo );

    std::vector< unsigned int > testTeam { 2, 3, 5 };
    testInfo->model->addData( "questingTeam", testTeam );

    testAction = new ModifyTeamSelectionAction( 1, true );
    ControllerState* resultState = testState.handleAction( testAction );

    ASSERT_EQ( NULL, resultState );

    unsigned int member = testInfo->model->referenceData< std::vector< unsigned int > >( "questingTeam" )->back( );
    ASSERT_EQ( 1, member );
}

TEST_F( ClientControllerStateTestFixture, TestLobbyStateSettingsAction ) {
    avalon::client::LobbyState testState( testInfo );

    avalon::network::GameSettings* testSettings = new avalon::network::GameSettings( );
    testSettings->set_players( 4 );
    testSettings->set_client( 2 );
    testSettings->set_evil_count( 2 );
    testSettings->set_quest_track_len( 5 );
    testSettings->set_vote_track_len( 5 );
    testModel->addData( "hasGameSettings", false );

    testAction = new GameSettingsAction( testSettings );
    ControllerState* resultState = testState.handleAction( testAction );

    ASSERT_EQ( NULL, resultState );

    unsigned int players = *testInfo->model->referenceData< unsigned int >( "numberOfPlayers" );
    ASSERT_EQ( 4, players );

    ClosureSubscriber* sub = new ClosureSubscriber( [&](Subscriber*){ }, [&](Subscriber*){ } );
    testModel->subscribe( "hasGameSettings", sub );

    ASSERT_TRUE( *( sub->getData< bool >( ) ) );

    delete sub;
}

TEST_F( ClientControllerStateTestFixture, TestLobbyStatePlayerAction ) {
    avalon::client::LobbyState testState( testInfo );

    Player* testPlayer = new Player( "Test", avalon::NONE, avalon::GOOD );
    testModel->addData( "player2" );

    testAction = new AddPlayerAction( 2, testPlayer );
    ControllerState* resultState = testState.handleAction( testAction );

    ASSERT_EQ( NULL, resultState );

    ClosureSubscriber* sub = new ClosureSubscriber( [&](Subscriber*){ }, [&](Subscriber*){ } );
    testModel->subscribe( "player2", sub );

    ASSERT_EQ( "Test", sub->getData< Player >( )->getName( ) );
    ASSERT_EQ( avalon::GOOD, sub->getData< Player >( )->getAlignment( ) );
    ASSERT_EQ( avalon::NONE, sub->getData< Player >( )->getRole( ) );

    delete sub;
}

TEST_F( ClientControllerStateTestFixture, TestLobbyStateNameAction ) {
    avalon::client::LobbyState testState( testInfo );

    testAction = new SetNameAction( "Test" );
    ControllerState* resultState = testState.handleAction( testAction );

    ASSERT_EQ( NULL, resultState );
    ASSERT_EQ( avalon::network::PLAYER_BUF, testClient->getLastBufType( ) );

    std::string sentBuf = testClient->getLastProtobuf( );
    avalon::network::Player sentPlayer;
    sentPlayer.ParseFromString( sentBuf );

    ASSERT_EQ( 3, sentPlayer.id( ) );
    ASSERT_EQ( "Test", sentPlayer.name( ) );
}

TEST_F( ClientControllerStateTestFixture, TestLobbyStateTeamStateAction ) {
    avalon::client::LobbyState testState( testInfo );

    std::vector< unsigned int > testTeam { 2, 3, 5 };
    testModel->addData( "leaderID", UINT_MAX );
    testModel->addData( "questingTeam", testTeam );

    testAction = new EnterTeamSelectionAction( 1 );
    ControllerState* resultState = testState.handleAction( testAction );

    ASSERT_EQ( "TeamSelection", resultState->getType( ) );

    unsigned int size = testInfo->model->referenceData< std::vector< unsigned int > >( "questingTeam" )->size( );
    unsigned int leader = *testInfo->model->referenceData< unsigned int >( "leaderID" );
    ASSERT_EQ( 0, size );
    ASSERT_EQ( 1, leader );
}
