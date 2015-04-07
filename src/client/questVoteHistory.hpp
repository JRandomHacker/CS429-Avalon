/**
 * A class for the quest vote history on the client side
 *
 * @class QuestVoteHistory
 * @author Ryan Kerr && Justin Koehler
 * @date 2015-04-06
 */

#ifndef _QUESTVOTEHISTORY_HPP
#define _QUESTVOTEHISTORY_HPP

#include <string>
#include <iostream>
#include <vector>

#include "globals.hpp"

class QuestVoteHistory {
    public:
        /**
         * Constructor that receives and stores vote vector
         *
         * @param didVotePass boolean for whether vote passed or not
         * @param playerVotesVec vector containing the votes of all players indexed by nothing in particular
         * @param questTrackDuringVote position of the quest track during the vote
         */
        QuestVoteHistory ( bool didVotePass,
            std::vector< avalon::player_vote_t > playerVotesVec,
            unsigned int questTrackDuringVote );

        /**
         * Getter
         *
         * @return the votes for each player in a vector
         */
        std::vector< avalon::player_vote_t > getPlayerVotes( ) const;

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
        
        unsigned int questTrackNum;
};

#endif
