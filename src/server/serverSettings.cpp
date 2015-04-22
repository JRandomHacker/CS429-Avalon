#include "serverSettings.hpp"

void GameSettings::loadFromFile( std::string file_name, unsigned int num_players ) {
	std::fstream file;
	file.open("lua_out", std::ios::out);
	file << "AFEFE" << std::endl;
	
	lua_State* L = luaL_newstate( );
	luaL_openlibs( L );

	lua_pushinteger( L, num_players );
	lua_setglobal( L, "num_players" );

	if ( luaL_loadfile( L, file_name.c_str( ) ) ) {
		file << "ERROR IN LOADING LUA FILE" << std::endl;
		auto c = lua_tostring( L, -1 );
		file << c << std::endl;
	} else {
		file << "ABLE TO LOAD LUA FILE" << std::endl;
	}
	if ( lua_pcall( L, 0, 0, 0 ) ) {
		file << "ERROR CALLIN LUA FN" << std::endl;
	} else {
		file << "ABLE TO CALL LUA FN" << std::endl;
	}
	luaLoadUnsignedInteger( L, &vote_track_length, "vote_track_length" );
	luaLoadUnsignedInteger( L, &quest_track_length, "quest_track_length" );
	luaLoadUnsignedIntegerArray( L, &fails_per_quest, "fails_per_quest" );
	luaLoadUnsignedIntegerArray( L, &players_per_quest, "players_per_quest" );
	
	file << "Players per quest length: " << players_per_quest.size() << std::endl;
	for (int i = 0; i < players_per_quest.size(); i++) {
		file << players_per_quest[i] << ", ";
	}
	file << std::endl;
	file << "Fails per quest length: " << fails_per_quest.size() << std::endl;
	for (int i = 0; i < fails_per_quest.size(); i++) {
		file << fails_per_quest[i] << ", ";
	}
	file << std::endl;
	file << "vote_track_length: " << vote_track_length << std::endl;
	file << "quest_track_length: " << quest_track_length << std::endl;

	lua_close( L );
	file.close();

	//quest_track_length = 5;
	//vote_track_length = 5;
	//fails_per_quest = {{1, 1, 1, 1, 1}};
	//players_per_quest = {{1, 1, 1, 1, 1}};
}

void GameSettings::luaLoadUnsignedInteger( lua_State* L, unsigned int* result, const char * global_name ) {
	lua_getglobal( L, global_name );
	if ( !lua_isnumber( L, -1 ) ) {
		// Handle invalid load
	}
	*result = (unsigned int) lua_tonumber( L, -1 );
	lua_remove( L, -1 );
}

void GameSettings::luaLoadUnsignedIntegerArray( lua_State* L, std::vector< unsigned int >* result, const char * global_name ) {
	lua_getglobal( L, global_name );
	if ( lua_isnil( L, -1 ) ) {
		// Handle invalid load
	}
	result->clear( );
	lua_pushnil( L );
	while ( lua_next( L, -2 ) ) {
		result->push_back( (unsigned int)lua_tonumber( L, -1 ) );
		lua_pop( L, 1 );
	}
}