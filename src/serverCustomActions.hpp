#ifndef SERVERCUSTOMACTIONS_HPP
#define SERVERCUSTOMACTIONS_HPP

#include <string>

#include "action.hpp"
#include "player.hpp"
#include "../protos/settings.pb.h"

class client_info_temp {};

class AddClientAction : public Action {
public:
    /**
     * Public constructor
     *
     * @param mess_text The human readable message explaining the contents
     *     of the action
     * @param new_client_info The struct detailing the new client to be added
     */
    AddClientAction(client_info_temp* new_client_info);
    virtual ~AddClientAction();

    client_info_temp* getClientInfo();

private:
    client_info_temp* client_info;
};

#endif // SERVERCUSTOMACTIONS_HPP
