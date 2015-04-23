

#ifndef _SERVERSETTINGS_HPP
#define _SERVERSETTINGS_HPP

#include "globals.hpp"
#include <lua.hpp>

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

typedef struct GameSettings {
	GameSettings( ) { }
	GameSettings( unsigned int qtl,
		unsigned int vtl,
		std::vector< unsigned int > ppq,
		std::vector< unsigned int > fpq ) :
	    quest_track_length( qtl ), vote_track_length( vtl ),
	    players_per_quest( ppq ), fails_per_quest( fpq ) {};

	int loadFromFile( std::string file_name, unsigned int num_players );
	bool luaLoadUnsignedInteger( lua_State* L, unsigned int* result, const char * global_name );
	bool luaLoadUnsignedIntegerArray( lua_State* L, std::vector< unsigned int >* result, const char * global_name );

	unsigned int num_evil_players = 1;
	unsigned int quest_track_length = 5;
	unsigned int vote_track_length = 5;
	std::vector< unsigned int > players_per_quest = {{1, 1, 1, 1, 1}};
	std::vector< unsigned int > fails_per_quest = {{1, 1, 1, 1, 1}};

} GameSettings;

/**
 * A class for storing constant settings for the avalon boardgame, beyond just
 * this specific instance of the game. This is stuff like how many quests there
 * are in the game, and how many players per quest given the total number of
 * players.
 *
 * @class AvalonSettings
 * @author Matt Hoffman
 * @date 2015-04-21
 */
typedef struct AvalonSettings {
	public:
		AvalonSettings( std::string file_name );
		GameSettings generateGameSettings( int num_players );


	unsigned int a;

	private:
		int max_number_of_players;
		std::vector< unsigned int > all_quest_track_length;
		std::vector< unsigned int > all_vote_track_length;
		std::vector< std::vector< unsigned int > > all_players_per_quest;
		std::vector< std::vector< unsigned int > > all_fails_per_quest;
} AvalonSettings;



/**
 * A class for storing the settings given to the server on startup.
 * This includes both game specific settings (included special characters) and
 * general settings like the number of players per quest given the number of
 * players in the game.
 *
 * @class GameSettings
 * @author Matt Hoffman
 * @date 2015-04-17
 */
typedef struct ServerSettings {
	ServerSettings( ) { }

	//! Which port the server will listen on
	int port = 0;
	//! What classes are used in this game
	std::vector< avalon::special_roles_t > selected_roles;
	//! How many players this game will support
	int num_players = 0;
    //! Whether hidden voting is enabled
    bool hidden_votes = false;

	GameSettings game_settings;
} ServerSettings;

#endif //  _SERVERSETTINGS_HPP
