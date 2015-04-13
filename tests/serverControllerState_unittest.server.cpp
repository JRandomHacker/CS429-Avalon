/**
 * Tests for serverControllerState.cpp
 *
 * @author Justin Koehler
 * @date 2015-3-17
 * @file serverControllerState_unittest.cpp
 */

#include "serverControllerState.hpp"
#include "../src/server/questVotingState.hpp"
#include "../src/server/teamSelectionState.hpp"
#include "../src/server/teamVotingState.hpp"
#include "../src/server/waitingForClientsState.hpp"
#include "../src/server/questVotingState.hpp"
#include "model.hpp"
#include "mockServer.hpp"
#include "serverInfo.hpp"
#include "serverCustomActions.hpp"
#include "subscriber.hpp"
#include "player.hpp"
#include "globals.hpp"
#include "settings.pb.h"
#include "teamselection.pb.h"
#include "vote.pb.h"
#include "teamvoteresults.pb.h"

#include "gtest/gtest.h"

#include <string>
#include <vector>
#include <random>

class ServerControllerStateTestFixture: public ::testing::Test {

    public:
        // Variables you want your tests to access must be public
        MockServer* testServer = NULL;
        ServInfo* testInfo = NULL;
        Action* testAction = NULL;

        // Run before every test
        void SetUp( void ) {
            testServer = new MockServer( );
            testInfo = new ServInfo;

            testInfo->num_clients = 3;
            testInfo->server = testServer;
            testInfo->leader = 2;
            testInfo->vote_track_length = 5;
            testInfo->players_per_quest = {{ 2, 2, 3, 4, 4 }};
            testInfo->quest_track = 0;

            std::vector< Player* > testPlayers;
            testPlayers.push_back( new Player( "player1", avalon::NONE, avalon::GOOD ) );
            testPlayers.push_back( new Player( "player2", avalon::NONE, avalon::GOOD ) );
            testPlayers.push_back( new Player( "player3", avalon::NONE, avalon::GOOD ) );
            testInfo->players = testPlayers;

            avalon::network::GameSettings testSettings;
            testSettings.set_players( 3 );
            testSettings.set_client( 2 );
            testSettings.set_evil_count( 2 );
            testSettings.set_quest_track_len( 5 );
            testSettings.set_vote_track_len( 5 );
            testSettings.set_merlin( false );
            testSettings.set_percival( false );
            testSettings.set_mordred( false );
            testSettings.set_morgana( false );
            testSettings.set_assassin( false );
            testSettings.set_oberon( false );
            testInfo->settingsBuf = testSettings;
        }

        void create_team( int team_size ) {
            testInfo->team.clear();
            for ( int i = 0; i < team_size; i++ ) {
                testInfo->team.push_back(i);
            }
        }

        // Run after every test
        void TearDown( void ) {
            if( testServer != NULL ) {
                delete testServer;
                testServer = NULL;
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

TEST_F( ServerControllerStateTestFixture, TestWaitingStateNewplayerAction ) {
    avalon::server::WaitingForClientsState testState( testInfo );

    testAction = new NewPlayerAction( 0, "Test" );
    ControllerState* resultState = testState.handleAction( testAction );

    ASSERT_EQ( NULL, resultState );
    ASSERT_EQ( avalon::network::PLAYER_BUF, testServer->getLastBufType( ) );

    std::string sentBuf = testServer->getLastProtobuf( );
    avalon::network::Player sentPlayer;
    sentPlayer.ParseFromString( sentBuf );

    ASSERT_EQ( 0, sentPlayer.id( ) );
}

TEST_F( ServerControllerStateTestFixture, TestWaitingStateTeamStateAction ) {
    avalon::server::WaitingForClientsState testState( testInfo );

    testAction = new EnterTeamSelectionAction( );
    ControllerState* resultState = testState.handleAction( testAction );

    ASSERT_EQ( "TeamSelection", resultState->getType( ) );

    ASSERT_EQ( avalon::network::ENTER_TEAM_SELECTION_BUF, testServer->getLastState( ) );
}

TEST_F( ServerControllerStateTestFixture, TestTeamStateToggleActionNotLeader ) {
    avalon::server::TeamSelectionState testState( testInfo );

    testAction = new ToggleTeamMemberAction( 1, 1 );
    ControllerState* resultState = testState.handleAction( testAction );

    ASSERT_EQ( NULL, resultState );
    ASSERT_EQ( "", testServer->getLastProtobuf( ) );
}

TEST_F( ServerControllerStateTestFixture, TestTeamStateToggleActionLeader ) {
    avalon::server::TeamSelectionState testState( testInfo );

    testAction = new ToggleTeamMemberAction( 2, 2 );
    ControllerState* resultState = testState.handleAction( testAction );

    ASSERT_EQ( NULL, resultState );
    ASSERT_EQ( avalon::network::TEAM_SELECTION_BUF, testServer->getLastBufType( ) );

    std::string sentBuf = testServer->getLastProtobuf( );
    avalon::network::TeamSelection sentSelection;
    sentSelection.ParseFromString( sentBuf );

    ASSERT_EQ( 2, sentSelection.id( ) );
    ASSERT_EQ( true, sentSelection.selected( ) );
}

TEST_F( ServerControllerStateTestFixture, TestTeamStateToggleActionDeselect ) {
    avalon::server::TeamSelectionState testState( testInfo );

    std::vector< unsigned int > testTeam { 2, 3 };
    testInfo->team = testTeam;

    testAction = new ToggleTeamMemberAction( 2, 2 );
    ControllerState* resultState = testState.handleAction( testAction );

    ASSERT_EQ( NULL, resultState );
    ASSERT_EQ( avalon::network::TEAM_SELECTION_BUF, testServer->getLastBufType( ) );

    std::string sentBuf = testServer->getLastProtobuf( );
    avalon::network::TeamSelection sentSelection;
    sentSelection.ParseFromString( sentBuf );

    ASSERT_EQ( 2, sentSelection.id( ) );
    ASSERT_EQ( false, sentSelection.selected( ) );
}

TEST_F( ServerControllerStateTestFixture, TestTeamStateConfirmActionNotLeader ) {
    avalon::server::TeamSelectionState testState( testInfo );

    create_team(2);

    testAction = new ConfirmTeamSelectionAction( 3 );
    ControllerState* resultState = testState.handleAction( testAction );

    ASSERT_EQ( NULL, resultState );
    ASSERT_EQ( "" , testServer->getLastProtobuf( ) );
}

TEST_F( ServerControllerStateTestFixture, TestTeamStateConfirmActionLeader ) {
    avalon::server::TeamSelectionState testState( testInfo );

    create_team(2);

    testAction = new ConfirmTeamSelectionAction( 2 );
    ControllerState* resultState = testState.handleAction( testAction );

    ASSERT_EQ( "Voting", resultState->getType( ) );
    ASSERT_EQ( avalon::network::ENTER_TEAM_VOTE_BUF, testServer->getLastState( ) );
}

TEST_F( ServerControllerStateTestFixture, TestVotingStateVoteActionNew ) {
    avalon::server::TeamVotingState testState( testInfo );

    std::vector< std::pair< unsigned int, avalon::player_vote_t > > testVotes;
    testInfo->votes = testVotes;

    testAction = new TeamVoteAction( 2, avalon::YES );
    ControllerState* resultState = testState.handleAction( testAction );

    ASSERT_EQ( NULL, resultState );

    std::string sentBuf = testServer->getLastProtobuf( );
    avalon::network::Vote sentVote;
    sentVote.ParseFromString( sentBuf );

    ASSERT_EQ( 2, sentVote.id( ) );
    ASSERT_EQ( avalon::HIDDEN, sentVote.vote( ) );
}

TEST_F( ServerControllerStateTestFixture, TestVotingStateVoteActionRepeat ) {
    avalon::server::TeamVotingState testState( testInfo );

    std::vector< std::pair< unsigned int, avalon::player_vote_t > > testVotes;
    testVotes.push_back( std::pair< unsigned int, avalon::player_vote_t >( 2, avalon::YES ) );
    testInfo->votes = testVotes;

    testAction = new TeamVoteAction( 2, avalon::YES );
    ControllerState* resultState = testState.handleAction( testAction );

    ASSERT_EQ( NULL, resultState );
    ASSERT_EQ( "" , testServer->getLastProtobuf( ) );
}

TEST_F( ServerControllerStateTestFixture, TestVotingStateVoteActionChange ) {
    avalon::server::TeamVotingState testState( testInfo );

    std::vector< std::pair< unsigned int, avalon::player_vote_t > > testVotes;
    testVotes.push_back( std::pair< unsigned int, avalon::player_vote_t >( 2, avalon::NO ) );
    testInfo->votes = testVotes;

    testAction = new TeamVoteAction( 2, avalon::YES );
    ControllerState* resultState = testState.handleAction( testAction );

    ASSERT_EQ( NULL, resultState );

    std::string sentBuf = testServer->getLastProtobuf( );
    avalon::network::Vote sentVote;
    sentVote.ParseFromString( sentBuf );

    ASSERT_EQ( 2, sentVote.id( ) );
    ASSERT_EQ( avalon::HIDDEN, sentVote.vote( ) );
}

TEST_F( ServerControllerStateTestFixture, TestVotingStateVoteActionSuccess ) {
    avalon::server::TeamVotingState testState( testInfo );

    std::vector< std::pair< unsigned int, avalon::player_vote_t > > testVotes;
    testVotes.push_back( std::pair< unsigned int, avalon::player_vote_t >( 1, avalon::YES ) );
    testVotes.push_back( std::pair< unsigned int, avalon::player_vote_t >( 3, avalon::NO ) );
    testInfo->votes = testVotes;

    testAction = new TeamVoteAction( 2, avalon::YES );
    ControllerState* resultState = testState.handleAction( testAction );

    EXPECT_EQ( "QuestVoting", resultState->getType( ) );

    std::string sentBuf = testServer->getLastProtobuf( );
    avalon::network::VoteResults sentResults;
    sentResults.ParseFromString( sentBuf );

    ASSERT_EQ( true, sentResults.passed( ) );
    EXPECT_EQ( avalon::network::ENTER_QUEST_VOTE_BUF, testServer->getLastState( ) );
}

TEST_F( ServerControllerStateTestFixture, TestVotingStateVoteActionFailure ) {
    avalon::server::TeamVotingState testState( testInfo );

    std::vector< std::pair< unsigned int, avalon::player_vote_t > > testVotes;
    testVotes.push_back( std::pair< unsigned int, avalon::player_vote_t >( 1, avalon::YES ) );
    testVotes.push_back( std::pair< unsigned int, avalon::player_vote_t >( 3, avalon::NO ) );
    testInfo->votes = testVotes;
    testInfo->vote_track = 1;

    testAction = new TeamVoteAction( 2, avalon::NO );
    ControllerState* resultState = testState.handleAction( testAction );

    ASSERT_EQ( "TeamSelection", resultState->getType( ) );

    std::string sentBuf = testServer->getLastProtobuf( );
    avalon::network::VoteResults sentResults;
    sentResults.ParseFromString( sentBuf );

    ASSERT_EQ( false, sentResults.passed( ) );
    ASSERT_EQ( avalon::network::ENTER_TEAM_SELECTION_BUF, testServer->getLastState( ) );
}

TEST_F( ServerControllerStateTestFixture, TestVotingStateVoteActionLoss ) {
    avalon::server::TeamVotingState testState( testInfo );

    std::vector< std::pair< unsigned int, avalon::player_vote_t > > testVotes;
    testVotes.push_back( std::pair< unsigned int, avalon::player_vote_t >( 1, avalon::YES ) );
    testVotes.push_back( std::pair< unsigned int, avalon::player_vote_t >( 3, avalon::NO ) );
    testInfo->votes = testVotes;
    testInfo->vote_track = 5;

    testAction = new TeamVoteAction( 2, avalon::NO );
    ControllerState* resultState = testState.handleAction( testAction );

    ASSERT_EQ( NULL, resultState );

    std::string sentBuf = testServer->getLastProtobuf( );
    avalon::network::VoteResults sentResults;
    sentResults.ParseFromString( sentBuf );

    ASSERT_EQ( false, sentResults.passed( ) );
    ASSERT_EQ( avalon::network::ENTER_END_GAME_BUF, testServer->getLastState( ) );
}

TEST_F( ServerControllerStateTestFixture, TestQuestStateVoteActionNew ) {
    avalon::server::QuestVotingState testState( testInfo );

    testInfo->team = { 2, 3, 4 };
    std::vector< std::pair< unsigned int, avalon::player_vote_t > > testVotes;
    testInfo->votes = testVotes;

    testAction = new QuestVoteAction( 2, avalon::YES );
    ControllerState* resultState = testState.handleAction( testAction );

    EXPECT_EQ( NULL, resultState );

    std::string sentBuf = testServer->getLastProtobuf( );
    avalon::network::Vote sentVote;
    sentVote.ParseFromString( sentBuf );

    EXPECT_EQ( 2, sentVote.id( ) );
    EXPECT_EQ( avalon::HIDDEN, sentVote.vote( ) );
}

TEST_F( ServerControllerStateTestFixture, TestQuestStateVoteActionRepeat ) {
    avalon::server::QuestVotingState testState( testInfo );

    testInfo->team = { 2, 3, 4 };
    std::vector< std::pair< unsigned int, avalon::player_vote_t > > testVotes;
    testVotes.push_back( std::pair< unsigned int, avalon::player_vote_t >( 2, avalon::YES ) );
    testInfo->votes = testVotes;

    testAction = new QuestVoteAction( 2, avalon::YES );
    ControllerState* resultState = testState.handleAction( testAction );

    EXPECT_EQ( NULL, resultState );
    EXPECT_EQ( "" , testServer->getLastProtobuf( ) );
}

TEST_F( ServerControllerStateTestFixture, TestQuestStateVoteActionReject ) {
    avalon::server::QuestVotingState testState( testInfo );

    testInfo->team = { 2, 3, 4 };
    std::vector< std::pair< unsigned int, avalon::player_vote_t > > testVotes;
    testInfo->votes = testVotes;

    testAction = new QuestVoteAction( 1, avalon::YES );
    ControllerState* resultState = testState.handleAction( testAction );

    EXPECT_EQ( NULL, resultState );
    EXPECT_EQ( "" , testServer->getLastProtobuf( ) );
}

TEST_F( ServerControllerStateTestFixture, TestQuestStateVoteActionSuccess ) {
    avalon::server::QuestVotingState testState( testInfo );

    testInfo->team = { 2, 3, 4 };
    testInfo->quest_track = 0;
    testInfo->fails_per_quest = { 1, 1, 1, 1, 1 };
    testInfo->quests_failed = 0;
    testInfo->quest_track_length = 5;
    std::random_device rd; // Seed
    testInfo->rng = new std::mt19937( rd( ) );
    std::vector< std::pair< unsigned int, avalon::player_vote_t > > testVotes;
    testVotes.push_back( std::pair< unsigned int, avalon::player_vote_t >( 2, avalon::YES ) );
    testVotes.push_back( std::pair< unsigned int, avalon::player_vote_t >( 3, avalon::YES ) );
    testInfo->votes = testVotes;

    testAction = new QuestVoteAction( 4, avalon::YES );
    ControllerState* resultState = testState.handleAction( testAction );

    EXPECT_EQ( "TeamSelection", resultState->getType( ) );

    std::string sentBuf = testServer->getLastProtobuf( );
    avalon::network::VoteResults sentResults;
    sentResults.ParseFromString( sentBuf );

    ASSERT_EQ( true, sentResults.passed( ) );
    EXPECT_EQ( avalon::network::ENTER_TEAM_SELECTION_BUF, testServer->getLastState( ) );
}

TEST_F( ServerControllerStateTestFixture, TestQuestStateVoteActionFailure ) {
    avalon::server::QuestVotingState testState( testInfo );

    testInfo->team = { 2, 3, 4 };
    testInfo->quest_track = 0;
    testInfo->fails_per_quest = { 1, 1, 1, 1, 1 };
    testInfo->quests_failed = 0;
    testInfo->quest_track_length = 5;
    std::random_device rd; // Seed
    testInfo->rng = new std::mt19937( rd( ) );
    std::vector< std::pair< unsigned int, avalon::player_vote_t > > testVotes;
    testVotes.push_back( std::pair< unsigned int, avalon::player_vote_t >( 2, avalon::YES ) );
    testVotes.push_back( std::pair< unsigned int, avalon::player_vote_t >( 3, avalon::NO ) );
    testInfo->votes = testVotes;

    testAction = new QuestVoteAction( 4, avalon::YES );
    ControllerState* resultState = testState.handleAction( testAction );

    EXPECT_EQ( "TeamSelection", resultState->getType( ) );

    std::string sentBuf = testServer->getLastProtobuf( );
    avalon::network::VoteResults sentResults;
    sentResults.ParseFromString( sentBuf );

    ASSERT_EQ( false, sentResults.passed( ) );
    EXPECT_EQ( avalon::network::ENTER_TEAM_SELECTION_BUF, testServer->getLastState( ) );
}

TEST_F( ServerControllerStateTestFixture, TestChatMessageReceived ) {
    avalon::server::ServerControllerState testState( "testState", testInfo );

    avalon::common::ChatMessage msg( 3, "Test Message Text", 5 );
    testAction = new ChatMessageRecvAction( msg );
    ControllerState* resultState = testState.handleAction( testAction );

    EXPECT_EQ( NULL, resultState );

    std::string sentBuf = testServer->getLastProtobuf( );
    avalon::network::ChatMessage sentMessage;
    sentMessage.ParseFromString( sentBuf );

    ASSERT_EQ( 3, sentMessage.sender_id( ) );
    ASSERT_EQ( "Test Message Text", sentMessage.message_text( ) );
    ASSERT_EQ( 5, sentMessage.timestamp( ) );
}

TEST_F( ServerControllerStateTestFixture, TestChatMessageReceivedChildState ) {
    avalon::server::TeamSelectionState testState( testInfo );

    avalon::common::ChatMessage msg( 4, "Test Message Text", 6 );
    testAction = new ChatMessageRecvAction( msg );
    ControllerState* resultState = testState.handleAction( testAction );

    EXPECT_EQ( NULL, resultState );

    std::string sentBuf = testServer->getLastProtobuf( );
    avalon::network::ChatMessage sentMessage;
    sentMessage.ParseFromString( sentBuf );

    ASSERT_EQ( 4, sentMessage.sender_id( ) );
    ASSERT_EQ( "Test Message Text", sentMessage.message_text( ) );
    ASSERT_EQ( 6, sentMessage.timestamp( ) );
}

TEST_F( ServerControllerStateTestFixture, TestChatMessagesReceivedInOrder ) {
    avalon::server::ServerControllerState testState( "testState", testInfo );

    avalon::common::ChatMessage msg1( 3, "Test Message Text1", 5 );
    avalon::common::ChatMessage msg2( 4, "Test Message Text2", 6 );
    avalon::common::ChatMessage msg3( 5, "Test Message Text3", 7 );
    testAction = new ChatMessageRecvAction( msg1 );
    ControllerState* resultState = testState.handleAction( testAction );

    EXPECT_EQ( NULL, resultState );

    std::string sentBuf = testServer->getLastProtobuf( );
    avalon::network::ChatMessage sentMessage;
    sentMessage.ParseFromString( sentBuf );

    ASSERT_EQ( 3, sentMessage.sender_id( ) );
    ASSERT_EQ( "Test Message Text1", sentMessage.message_text( ) );
    ASSERT_EQ( 5, sentMessage.timestamp( ) );

    testAction = new ChatMessageRecvAction( msg2 );
    resultState = testState.handleAction( testAction );

    EXPECT_EQ( NULL, resultState );

    sentBuf = testServer->getLastProtobuf( );
    sentMessage.ParseFromString( sentBuf );

    ASSERT_EQ( 4, sentMessage.sender_id( ) );
    ASSERT_EQ( "Test Message Text2", sentMessage.message_text( ) );
    ASSERT_EQ( 6, sentMessage.timestamp( ) );

    testAction = new ChatMessageRecvAction( msg3 );
    resultState = testState.handleAction( testAction );

    EXPECT_EQ( NULL, resultState );

    sentBuf = testServer->getLastProtobuf( );
    sentMessage.ParseFromString( sentBuf );

    ASSERT_EQ( 5, sentMessage.sender_id( ) );
    ASSERT_EQ( "Test Message Text3", sentMessage.message_text( ) );
    ASSERT_EQ( 7, sentMessage.timestamp( ) );
}
