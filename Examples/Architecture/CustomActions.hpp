#ifndef CUSTOMACTIONS_HPP
#define CUSTOMACTIONS_HPP

#include <string>

#include "Action.hpp"

class CreateGameAction : public Action {
public:
	/**
	 * Public constructor
	 * 
	 * @param mess_text The human readable message explaining the contents
	 *     of the action
	 * @param comm_args The string of command args to give to the server
	 */
	CreateGameAction(std::string comm_args);
	virtual ~CreateGameAction();
	 
	std::string getArgs();

private:
	std::string command_args;
};

class JoinGameAction : public Action {
public:
	/**
	 * Public constructor
	 * 
	 * @param mess_text The human readable message explaining the contents
	 *     of the action
	 * @param serv_info The string representing the IP and port of the server
	 *     to attempt to connect to
	 */
	JoinGameAction(std::string serv_info);
	virtual ~JoinGameAction();
	 
	std::string getServerInfo();

private:
	std::string server_info;
};

class EnterLobbyAction : public Action {
public:
	/**
	 * Public constructor
	 * 
	 * @param mess_text The human readable message explaining the contents
	 *     of the action
	 * @param settings The string representing the game settings for the
	 *     GUI to display
	 */
	EnterLobbyAction(std::string settings);
	virtual ~EnterLobbyAction();
	 
	std::string getSettings();

private:
	std::string game_settings;
};

class AddPlayerAction : public Action {
public:
	/**
	 * Public constructor
	 * 
	 * @param mess_text The human readable message explaining the contents
	 *     of the action
	 * @param player The string representing the player info for the GUI
	 *     to display
	 */
	AddPlayerAction(std::string player);
	virtual ~AddPlayerAction();
	 
	std::string getPlayerInfo();

private:
	std::string player_info;
};

#endif // CUSTOMACTIONS_HPP
