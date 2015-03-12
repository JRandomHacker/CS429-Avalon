#ifndef SERVERCUSTOMACTIONS_HPP
#define SERVERCUSTOMACTIONS_HPP

#include <string>

#include "action.hpp"
#include "player.hpp"
#include "../protos/settings.pb.h"

class NewPlayerAction : public Action {
public:
    /**
     * Public constructor
     *
     * @param client_id The id of the client that just connected
     */
    NewPlayerAction( unsigned int player_id );
    virtual ~NewPlayerAction( );

    /**
     * Getter
     *
     * @return The ID of the player that just joined
     */
    unsigned int getPlayerID( );

private:
    unsigned int player_id;
};

#endif // SERVERCUSTOMACTIONS_HPP
