#ifndef CUSTOMACTIONS_HPP
#define CUSTOMACTIONS_HPP

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
	 CreateGameAction(std::string mess_text, std::string comm_args);
	 
	 std::string GetArgs();

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
	 JoinGameAction(std::string mess_text, std::string serv_info);
	 
	 std::string GetServerInfo();

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
	 EnterLobbyAction(std::string mess_text, std::string settings);
	 
	 std::string GetSettings();

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
	 JoinGameAction(std::string mess_text, std::string player);
	 
	 std::string GetPlayerInfo();

private:
	std::string player_info;
};

#endif // CUSTOMACTIONS_HPP
