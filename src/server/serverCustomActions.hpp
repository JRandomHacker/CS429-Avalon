#ifndef SERVERCUSTOMACTIONS_HPP
#define SERVERCUSTOMACTIONS_HPP

#include <string>

#include "action.hpp"
#include "player.hpp"
#include "settings.pb.h"

class NewPlayerAction : public Action {
public:
    /**
     * Public constructor
     *
     * @param player_id The id of the player that just connected
     * @param player_name The name the connecting player requested
     */
    NewPlayerAction( unsigned int player_id, std::string player_name );
    virtual ~NewPlayerAction( );

    /**
     * Getter
     *
     * @return The ID of the player that just joined
     */
    unsigned int getPlayerID( );

    /**
     * Getter
     *
     * @return The name that the player requested
     */
    std::string getPlayerName( );

private:
    unsigned int player_id;
    std::string player_name;
};

#endif // SERVERCUSTOMACTIONS_HPP
