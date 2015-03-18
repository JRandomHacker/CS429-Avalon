/*
 * Implementation of VoteHistory class
 *
 * @file voteHistory.cpp
 * @author Mohammad Askari && Matt Hoffman
 * @date 2015-03-18
*/

#include "voteHistory.hpp"

VoteHistory::VoteHistory( std::vector< avalon::player_vote_t > playerVotesVec, std::vector< unsigned int > teamVotedOn, unsigned int voteTrackDuringVote, unsigned int questTrackDuringVote ) {
    playerVotes = playerVotesVec;
    proposedTeam = teamVotedOn;
    voteTrackNum = voteTrackDuringVote;
    questTrackNum = questTrackDuringVote;
}

avalon::player_vote_t getPlayerVote ( unsigned int playerNum ) {
    if (playerNum >=0 && playerNum < playerVotes.size())
        return playerVotes.at(playerNum);
    std::cout << "getPlayerVote received bad playerNum!";
    return NO_VOTE;
}

std::vector< avalon::player_vote_t > getPlayerVotes( ) {
    return playerVotes;
}

std::vector< unsigned int > getProposedTeam( ) {
    return proposedTeam;
}

unsigned int getVoteTrackNum( ) {
    return voteTrackNum;
}

unsigned int getQuestTrackNum( ) {
    return questTrackNum;
}
