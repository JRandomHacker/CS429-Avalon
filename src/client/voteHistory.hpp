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
#include globals.hpp

#include "vote.pb.h"
#include "voteresults.pb.h"

class VoteHistory {
    public:
    
        /**
         * Default constructor
         */
        VoteHistory ( );
        
        /**
         * Constructor that receives and stores vote vector
         *
         * @param myVoteVec vector containing the votes of all players
         */
        VoteHistory ( std::vector< avalon::player_vote_t > myVoteVec );
        
        /**
         * Returns the vote of the requested player
         *
         * @param playerNum the index of the player whose vote should be returned
         * @return vote at the index of the requested player
         */
        < avalon::player_vote_t > getPlayerVote ( unsigned int playerNum );
        
    private:
    
        std::vector< avalon::player_vote_t > voteVec;
        
        std::vector< unsigned int > playerNumVec;
        
        unsigned int voteTrackNum;
        
        unsigned int questTrackNum;
};

#endif
