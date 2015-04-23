#include "serverSettings.hpp"

int GameSettings::loadFromFile( std::string file_name, unsigned int num_players ) {
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
		return EXIT_LUA_LOAD_ERROR;
	} else {
		file << "ABLE TO LOAD LUA FILE" << std::endl;
	}
	if ( lua_pcall( L, 0, 0, 0 ) ) {
		file << "ERROR CALLIN LUA FN" << std::endl;
	} else {
		file << "ABLE TO CALL LUA FN" << std::endl;
	}
	bool correct_out = true;
	correct_out = correct_out && luaLoadUnsignedInteger( L, &num_evil_players, "num_evil_players" );
	correct_out = correct_out && luaLoadUnsignedInteger( L, &vote_track_length, "vote_track_length" );
	correct_out = correct_out && luaLoadUnsignedInteger( L, &quest_track_length, "quest_track_length" );
	correct_out = correct_out && luaLoadUnsignedIntegerArray( L, &fails_per_quest, "fails_per_quest" );
	correct_out = correct_out && luaLoadUnsignedIntegerArray( L, &players_per_quest, "players_per_quest" );
	if (!correct_out) {
		return EXIT_LUA_NO_VALUES;
	}

	file << "Players per quest length: " << players_per_quest.size( ) << std::endl;
	for ( unsigned int i = 0; i < players_per_quest.size( ); i++ ) {
		file << players_per_quest[i] << ", ";
	}
	file << std::endl;
	file << "Fails per quest length: " << fails_per_quest.size( ) << std::endl;
	for ( int i = 0; i < fails_per_quest.size( ); i++ ) {
		file << fails_per_quest[i] << ", ";
	}
	file << std::endl;
	file << "vote_track_length: " << vote_track_length << std::endl;
	file << "quest_track_length: " << quest_track_length << std::endl;
	file << "num_evil_players: " << num_evil_players << std::endl;

	lua_close( L );
	file.close();

	return EXIT_SUCCESS;
}

bool GameSettings::luaLoadUnsignedInteger( lua_State* L, unsigned int* result, const char * global_name ) {
	lua_getglobal( L, global_name );
	if ( !lua_isnumber( L, -1 ) ) {
		return false;
	}
	*result = (unsigned int) lua_tonumber( L, -1 );
	lua_remove( L, -1 );
	return true;
}

bool GameSettings::luaLoadUnsignedIntegerArray( lua_State* L, std::vector< unsigned int >* result, const char * global_name ) {
	lua_getglobal( L, global_name );
	if ( lua_isnil( L, -1 ) ) {
		return false;
	}
	result->clear( );
	lua_pushnil( L );
	while ( lua_next( L, -2 ) ) {
		result->push_back( (unsigned int)lua_tonumber( L, -1 ) );
		lua_pop( L, 1 );
	}
	return true;
}