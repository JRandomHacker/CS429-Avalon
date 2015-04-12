/**
 * Tests for clientControllerState.cpp
 *
 * @author Justin Koehler
 * @date 2015-03-16
 * @file clientControllerState_unittest.cpp
 */

#include "clientControllerState.hpp"
#include "lobbyState.hpp"
#include "../src/client/questVotingState.hpp"
#include "../src/client/teamSelectionState.hpp"
#include "../src/client/teamVotingState.hpp"
#include "../src/client/questVotingState.hpp"
#include "model.hpp"
#include "voteHistory.hpp"
#include "questVoteHistory.hpp"
#include "mockClient.hpp"
#include "clientInfo.hpp"
#include "clientCustomActionsFromGUI.hpp"
#include "clientCustomActionsFromNet.hpp"
#include "clientCustomActionsForChat.hpp"
#include "vote.pb.h"
#include "teamselection.pb.h"
#include "settings.pb.h"
#include "player.pb.h"
#include "subscriber.hpp"
#include "player.hpp"
#include "chat_message.hpp"

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

TEST_F( ClientControllerStateTestFixture, TestTeamVotingStateVoteAction ) {
    avalon::client::TeamVotingState testState( testInfo );

    testAction = new TeamVoteAction( avalon::YES );
    ControllerState* resultState = testState.handleAction( testAction );

    ASSERT_EQ( NULL, resultState );
    ASSERT_EQ( avalon::network::TEAM_VOTE_BUF, testClient->getLastBufType( ) );

    std::string sentBuf = testClient->getLastProtobuf( );
    avalon::network::Vote sentVote;
    sentVote.ParseFromString( sentBuf );

    ASSERT_EQ( avalon::YES, sentVote.vote( ) );
    ASSERT_EQ( 3, sentVote.id( ) );
}

TEST_F( ClientControllerStateTestFixture, TestTeamVotingStateResultsAction ) {
    avalon::client::TeamVotingState testState( testInfo );

    testModel->addData< unsigned int >( "currentVoteTrack" , 0);
    testModel->addData< unsigned int >( "voteTrackLength", 5 );
    testModel->addData< unsigned int >( "questTrackLength", 5 );
    testModel->addData( "questingTeam", std::vector< unsigned int >( ) );
    testModel->addData( "voteHistory", std::vector< VoteHistory >( ) );
    std::vector< avalon::player_vote_t > testVotes;
    testVotes.push_back( avalon::YES );
    testVotes.push_back( avalon::NO );
    testVotes.push_back( avalon::YES );
    testAction = new TeamVoteResultsAction( true, 2, testVotes );
    ControllerState* resultState = testState.handleAction( testAction );

    ASSERT_EQ( NULL, resultState );

    ClosureSubscriber* sub = new ClosureSubscriber( [&](Subscriber*){ }, [&](Subscriber*){ } );
    testModel->subscribe( "currentVoteTrack", sub );

    ASSERT_EQ( 2, *( sub->getData< unsigned int >( ) ) );

    delete sub;
}

TEST_F( ClientControllerStateTestFixture, TestTeamVotingStateTeamStateAction ) {
    avalon::client::TeamVotingState testState( testInfo );

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

TEST_F( ClientControllerStateTestFixture, TestQuestVotingStateYesVote ) {
    avalon::client::QuestVotingState testState( testInfo );
    
    testAction = new QuestVoteAction( avalon::YES );
    ControllerState* resultState = testState.handleAction( testAction );
    
    EXPECT_EQ( NULL, resultState );
    EXPECT_EQ( avalon::network::QUEST_VOTE_BUF, testClient->getLastBufType( ) );
    
    std::string sentBuf = testClient->getLastProtobuf( );
    avalon::network::Vote voteBuf;
    voteBuf.ParseFromString( sentBuf );
    
    EXPECT_EQ( 3, voteBuf.id( ) );
    EXPECT_EQ( avalon::YES, voteBuf.vote( ) );
}

TEST_F( ClientControllerStateTestFixture, TestQuestVotingStateResultsAction ) {
    avalon::client::QuestVotingState testState( testInfo );
    testInfo->model->addData< unsigned int >( "currentQuestTrack", 0 );
    testInfo->model->addData( "questHistory", std::vector< QuestVoteHistory >( ) );
    
    std::vector< avalon::player_vote_t > votes { avalon::YES, avalon::YES, avalon::NO };
    testAction = new QuestVoteResultsAction( true, 2, votes );
    ControllerState* resultState = testState.handleAction( testAction );
    
    EXPECT_EQ( NULL, resultState );
    
    auto history = *testInfo->model->referenceData< std::vector< QuestVoteHistory > >( "questHistory" );
    
    EXPECT_EQ( 1, history.size( ) );
    EXPECT_TRUE( history[ 1 ].getVotePassed( ) );
    EXPECT_EQ( 2, *testInfo->model->referenceData< unsigned int >( "currentQuestTrack" ) );
}

TEST_F( ClientControllerStateTestFixture, TestQuestVotingStateNetworkVote ) {
    avalon::client::QuestVotingState testState( testInfo );
    testInfo->model->addData< unsigned int >( "numberOfPlayers", 3 );
    testState.setupState( );
    
    testAction = new ReceiveQuestVoteAction( 1 );
    ControllerState* resultState = testState.handleAction( testAction );
    
    EXPECT_EQ( NULL, resultState );
    
    auto votes = *testInfo->model->referenceData< std::vector< avalon::player_vote_t > >( "currentVotes" );
    EXPECT_EQ( avalon::HIDDEN, votes[ 1 ] );
}

TEST_F( ClientControllerStateTestFixture, TestQuestVotingStateNewTeamAction ) {
    avalon::client::QuestVotingState testState( testInfo );
    std::vector< unsigned int > testTeam { 2, 3, 5 };
    testInfo->model->addData( "questingTeam", testTeam );

    testAction = new EnterTeamSelectionAction( 4 );
    ControllerState* resultState = testState.handleAction( testAction );

    EXPECT_EQ( "TeamSelection", resultState->getType( ) );
    EXPECT_EQ( 4, *testInfo->model->referenceData< unsigned int >( "leaderID" ) );
    EXPECT_EQ( 0, testInfo->model->referenceData< std::vector< unsigned int > >( "questingTeam" )->size( ) );
}

TEST_F( ClientControllerStateTestFixture, TestChatMessageSent ) {
    avalon::client::ClientControllerState testState( "testState", testInfo );

    avalon::common::ChatMessage msg( 3, "Test Message Text", 5 );
    testAction = new ChatMessageSentAction( msg );
    ControllerState* resultState = testState.handleAction( testAction );

    EXPECT_EQ( NULL, resultState );

    std::string sentBuf = testClient->getLastProtobuf( );
    avalon::network::ChatMessage sentMsgBuf;
    sentMsgBuf.ParseFromString( sentBuf );

    ASSERT_EQ( "Test Message Text", sentMsgBuf.message_text( ) );
    ASSERT_EQ( 3, sentMsgBuf.sender_id( ) );
    ASSERT_EQ( 5, sentMsgBuf.timestamp( ) );
}

TEST_F( ClientControllerStateTestFixture, TestChatMessageReceived ) {
    avalon::client::ClientControllerState testState( "testState", testInfo );
    testInfo->model->addData( "chatMessages", std::vector< avalon::common::ChatMessage >( ) );

    avalon::common::ChatMessage msg( 3, "Test Message Text", 5 );
    testAction = new ChatMessageRecvAction( msg );
    ControllerState* resultState = testState.handleAction( testAction );

    EXPECT_EQ( NULL, resultState );

    auto result_msg = (*testInfo->model->referenceData< std::vector< avalon::common::ChatMessage > >( "chatMessages" ))[0];

    ASSERT_EQ( "Test Message Text", result_msg.getMessageText( ) );
    ASSERT_EQ( 3, result_msg.getSenderId( ) );
    ASSERT_EQ( 5, result_msg.getTimestamp( ) );
}

TEST_F( ClientControllerStateTestFixture, TestChatMessageSentChildState ) {
    avalon::client::LobbyState testState( testInfo );

    avalon::common::ChatMessage msg( 3, "Test Message Text", 5 );
    testAction = new ChatMessageSentAction( msg );
    ControllerState* resultState = testState.handleAction( testAction );

    EXPECT_EQ( NULL, resultState );

    std::string sentBuf = testClient->getLastProtobuf( );
    avalon::network::ChatMessage sentMsgBuf;
    sentMsgBuf.ParseFromString( sentBuf );

    ASSERT_EQ( "Test Message Text", sentMsgBuf.message_text( ) );
    ASSERT_EQ( 3, sentMsgBuf.sender_id( ) );
    ASSERT_EQ( 5, sentMsgBuf.timestamp( ) );
}

TEST_F( ClientControllerStateTestFixture, TestChatMessageReceivedChildState ) {
    avalon::client::LobbyState testState( testInfo );
    testInfo->model->addData( "chatMessages", std::vector< avalon::common::ChatMessage >( ) );

    avalon::common::ChatMessage msg( 3, "Test Message Text", 5 );
    testAction = new ChatMessageRecvAction( msg );
    ControllerState* resultState = testState.handleAction( testAction );

    EXPECT_EQ( NULL, resultState );

    auto result_msg = (*testInfo->model->referenceData< std::vector< avalon::common::ChatMessage > >( "chatMessages" ))[0];

    ASSERT_EQ( "Test Message Text", result_msg.getMessageText( ) );
    ASSERT_EQ( 3, result_msg.getSenderId( ) );
    ASSERT_EQ( 5, result_msg.getTimestamp( ) );
}

TEST_F( ClientControllerStateTestFixture, TestChatMessagesReceivedInOrder ) {
    avalon::client::ClientControllerState testState( "testState", testInfo );
    testInfo->model->addData( "chatMessages", std::vector< avalon::common::ChatMessage >( ) );

    avalon::common::ChatMessage msg1( 1, "Test Message Text1", 3 );
    avalon::common::ChatMessage msg2( 2, "Test Message Text2", 4 );
    avalon::common::ChatMessage msg3( 3, "Test Message Text3", 5 );
    Action* testAction1 = new ChatMessageRecvAction( msg1 );
    Action* testAction2 = new ChatMessageRecvAction( msg2 );
    Action* testAction3 = new ChatMessageRecvAction( msg3 );
    ControllerState* resultState;
    resultState = testState.handleAction( testAction1 );
    EXPECT_EQ( NULL, resultState );
    resultState = testState.handleAction( testAction2 );
    EXPECT_EQ( NULL, resultState );
    resultState = testState.handleAction( testAction3 );
    EXPECT_EQ( NULL, resultState );
    
    auto result_msgs = *testInfo->model->referenceData< std::vector< avalon::common::ChatMessage > >( "chatMessages" );

    ASSERT_EQ( 3, result_msgs.size( ) );

    ASSERT_EQ( "Test Message Text1", result_msgs[0].getMessageText( ) );
    ASSERT_EQ( 1, result_msgs[0].getSenderId( ) );
    ASSERT_EQ( 3, result_msgs[0].getTimestamp( ) );

    ASSERT_EQ( "Test Message Text2", result_msgs[1].getMessageText( ) );
    ASSERT_EQ( 2, result_msgs[1].getSenderId( ) );
    ASSERT_EQ( 4, result_msgs[1].getTimestamp( ) );

    ASSERT_EQ( "Test Message Text3", result_msgs[2].getMessageText( ) );
    ASSERT_EQ( 3, result_msgs[2].getSenderId( ) );
    ASSERT_EQ( 5, result_msgs[2].getTimestamp( ) );
}
