/*
 * Implementation of VoteHistory class
 *
 * @file voteHistory.cpp
 * @author Mohammad Askari && Matt Hoffman
 * @date 2015-03-18
*/

#include "voteHistory.hpp"

VoteHistory::VoteHistory( bool didVotePass, std::vector< avalon::player_vote_t > playerVotesVec, std::vector< unsigned int > teamVotedOn, unsigned int voteTrackDuringVote, unsigned int questTrackDuringVote ) {
    votePassed = didVotePass;
    playerVotes = playerVotesVec;
    proposedTeam = teamVotedOn;
    voteTrackNum = voteTrackDuringVote;
    questTrackNum = questTrackDuringVote;
}

avalon::player_vote_t VoteHistory::getPlayerVote ( unsigned int playerNum ) const {
    if (playerNum < playerVotes.size( ))
        return playerVotes.at( playerNum );
    std::cerr << "getPlayerVote received bad playerNum!";
    return avalon::NO_VOTE;
}

std::vector< avalon::player_vote_t > VoteHistory::getPlayerVotes( ) const {
    return playerVotes;
}

std::vector< unsigned int > VoteHistory::getProposedTeam( ) const {
    return proposedTeam;
}

unsigned int VoteHistory::getVoteTrackNum( ) const {
    return voteTrackNum;
}

unsigned int VoteHistory::getQuestTrackNum( ) const {
    return questTrackNum;
}

bool VoteHistory::getVotePassed( ) const {
    return votePassed;
}
