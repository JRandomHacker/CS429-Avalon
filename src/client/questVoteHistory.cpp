/*
 * Implementation of QuestVoteHistory class
 *
 * @file questVoteHistory.cpp
 * @author Ryan Kerr && Justin Koehler
 * @date 2015-04-06
*/

#include "questVoteHistory.hpp"

QuestVoteHistory::QuestVoteHistory( bool didVotePass, std::vector< avalon::player_vote_t > playerVotesVec, unsigned int questTrackDuringVote ) {
    votePassed = didVotePass;
    playerVotes = playerVotesVec;
    questTrackNum = questTrackDuringVote;
}

std::vector< avalon::player_vote_t > QuestVoteHistory::getPlayerVotes( ) const {
    return playerVotes;
}

unsigned int QuestVoteHistory::getQuestTrackNum( ) const {
    return questTrackNum;
}

bool QuestVoteHistory::getVotePassed( ) const {
    return votePassed;
}

