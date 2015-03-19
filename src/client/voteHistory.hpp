/**
 * A class for the vote history on the client side
 *
 * @class VoteHistory
 * @author Mohammad Askari && Matt Hoffman
 * @date 2015-03-18
 */

#ifndef _VOTEHISTORY_HPP
#define _VOTEHISTORY_HPP

#include <string>
#include <iostream>

#include "globals.hpp"

#include "vote.pb.h"
#include "voteresults.pb.h"

class VoteHistory {
    public:
        /**
         * Constructor that receives and stores vote vector
         *
         * @param didVotePass boolean for whether vote passed or not
         * @param playerVotesVec vector containing the votes of all players indexed by player id
         * @param teamVotedOn vector containing the player ids for each player on the team being voted on
         * @param voteTrackDuringVote position of the vote track during the vote
         * @param questTrackDuringVote position of the quest track during the vote
         */
        VoteHistory ( bool didVotePass,
            std::vector< avalon::player_vote_t > playerVotesVec,
            std::vector< unsigned int > teamVotedOn,
            unsigned int voteTrackDuringVote,
            unsigned int questTrackDuringVote );
        
        /**
         * Returns the vote of the requested player
         *
         * @param playerNum the index of the player whose vote should be returned
         * @return vote at the index of the requested player
         */
        avalon::player_vote_t getPlayerVote ( unsigned int playerNum ) const;

        /**
         * Getter
         *
         * @return the votes for each player in a vector
         */
        std::vector< avalon::player_vote_t > getPlayerVotes( ) const;

        /**
         * Getter
         *
         * @return the proposed team
         */
        std::vector< unsigned int > getProposedTeam( ) const;

        /**
         * Getter
         *
         * @return the position on the vote track at the time of the vote
         */
        unsigned int getVoteTrackNum( ) const;

        /**
         * Getter
         *
         * @return the position on the quest track at the time of the vote
         */
        unsigned int getQuestTrackNum( ) const;
        
        /**
         * Getter
         * 
         * @return bool for whether vote passed or not
         */
        bool getVotePassed( ) const;

        
        
    private:
    
        bool votePassed;
        
        std::vector< avalon::player_vote_t > playerVotes;
        
        std::vector< unsigned int > proposedTeam;
        
        unsigned int voteTrackNum;
        
        unsigned int questTrackNum;
};

#endif
