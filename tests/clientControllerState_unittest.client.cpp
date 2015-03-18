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
            testInfo->my_id = 3;
            testInfo->leader = 2;
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
    VotingState testState( testInfo );
    
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
    VotingState testState( testInfo );
    
    testModel->addData( "currentVoteTrack" );
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
    VotingState testState( testInfo );
    
    std::vector< unsigned int > testTeam { 2, 3, 5 };
    testInfo->team = testTeam;
    
    testAction = new EnterTeamSelectionAction( 4 );
    ControllerState* resultState = testState.handleAction( testAction );
    
    ASSERT_EQ( "TeamSelection", resultState->getType( ) );
    ASSERT_EQ( 4, testInfo->leader );
    ASSERT_EQ( 0, testInfo->team.size( ) );
    
}

TEST_F( ClientControllerStateTestFixture, TestTeamStateSelectActionNotLeader ) {
    TeamSelectionState testState( testInfo );
    
    testAction = new SelectQuestGoerAction( true, 1 );
    ControllerState* resultState = testState.handleAction( testAction );
    
    ASSERT_EQ( NULL, resultState );
    ASSERT_EQ( "", testClient->getLastProtobuf( ) );
}

TEST_F( ClientControllerStateTestFixture, TestTeamStateSelectActionLeader ) {
    TeamSelectionState testState( testInfo );

    testInfo->leader = 3;
    
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
    TeamSelectionState testState( testInfo );
    
    testAction = new FinalizeTeamAction( );
    ControllerState* resultState = testState.handleAction( testAction );
    
    ASSERT_EQ( NULL, resultState );
    ASSERT_EQ( "", testClient->getLastProtobuf( ) );
}

TEST_F( ClientControllerStateTestFixture, TestTeamStateFinalizeActionLeader ) {
    TeamSelectionState testState( testInfo );

    testInfo->leader = 3;
    
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
    TeamSelectionState testState( testInfo );
    
    std::vector< unsigned int > testTeam { 2, 3, 5 };
    testInfo->team = testTeam;
    
    testAction = new ModifyTeamSelectionAction( 1, true );
    ControllerState* resultState = testState.handleAction( testAction );
    
    ASSERT_EQ( NULL, resultState );
    
    ASSERT_EQ( 1, testInfo->team.back( ) );
}

TEST_F( ClientControllerStateTestFixture, TestLobbyStateSettingsAction ) {
    LobbyState testState( testInfo );
    
    avalon::network::GameSettings* testSettings = new avalon::network::GameSettings( );
    testSettings->set_players( 4 );
    testSettings->set_client( 2 );
    testSettings->set_evil_count( 2 );
    testSettings->set_quest_track_len( 5 );
    testSettings->set_vote_track_len( 5 );
    testModel->addData( "hasGameSettings" );
    
    testAction = new GameSettingsAction( testSettings );
    ControllerState* resultState = testState.handleAction( testAction );
    
    ASSERT_EQ( NULL, resultState );

    ASSERT_EQ( 4, testInfo->num_players );
    
    ClosureSubscriber* sub = new ClosureSubscriber( [&](Subscriber*){ }, [&](Subscriber*){ } );
    testModel->subscribe( "hasGameSettings", sub );
    
    ASSERT_TRUE( *( sub->getData< bool >( ) ) );
    
    delete sub;
}

TEST_F( ClientControllerStateTestFixture, TestLobbyStatePlayerAction ) {
    LobbyState testState( testInfo );
    
    Player* testPlayer = new Player( "Test", avalon::NONE, avalon::GOOD );
    testModel->addData( "player2" );
    std::vector< Player* > testPlayers;
    testInfo->players = testPlayers;
    
    testAction = new AddPlayerAction( 2, testPlayer );
    ControllerState* resultState = testState.handleAction( testAction );
    
    ASSERT_EQ( NULL, resultState );
    
    ASSERT_EQ( 1, testInfo->players.size( ) );
    
    ClosureSubscriber* sub = new ClosureSubscriber( [&](Subscriber*){ }, [&](Subscriber*){ } );
    testModel->subscribe( "player2", sub );
    
    ASSERT_EQ( testPlayer, *( sub->getData< Player* >( ) ) );
    
    delete testPlayer;
    delete sub;
}

TEST_F( ClientControllerStateTestFixture, TestLobbyStateNameAction ) {
    LobbyState testState( testInfo );
    
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
    LobbyState testState( testInfo );
    
    std::vector< unsigned int > testTeam { 2, 3, 5 };
    testInfo->team = testTeam;
    testModel->addData( "leaderID" );
    testModel->addData( "questingTeam" );
    
    testAction = new EnterTeamSelectionAction( 1 );
    ControllerState* resultState = testState.handleAction( testAction );
    
    ASSERT_EQ( "TeamSelection", resultState->getType( ) );
    ASSERT_EQ( 0, testInfo->team.size( ) );
    ASSERT_EQ( 1, testInfo->leader );
}
