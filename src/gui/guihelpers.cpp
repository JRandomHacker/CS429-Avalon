
#include "guihelpers.hpp"
#include <string>

std::string avalon::gui::roleToString( avalon::special_roles_t role ) {
    switch( role ){

    case avalon::MERLIN:
        return "Merlin";
    case avalon::MORDRED:
        return "Mordred";
    case avalon::MORGANA:
        return "Morgana";
    case avalon::ASSASSIN:
        return "Assassin";
    case avalon::OBERON:
        return "Oberon";
    case avalon::PERCIVAL:
        return "Percival";
    case avalon::NONE:
        return "None";
    case avalon::UNKNOWN_ROLE:
        return "Unknown";
    }

    return "Role not recognized";
}

std::string avalon::gui::roleToImage( avalon::special_roles_t role, avalon::alignment_t align ) {

    std::string desire = ":/images/";

    switch( role ){
        case avalon::MERLIN:
            return desire += "MERLIN.jpg";
        case avalon::MORDRED:
            return desire += "MORDRED.jpg";
        case avalon::MORGANA:
            return desire += "MORGANA.jpg";
        case avalon::ASSASSIN:
            return desire += "ASSASSIN.jpg";
        case avalon::OBERON:
            return desire += "OBERON.jpg";
        case avalon::PERCIVAL:
            return desire += "PERCIVAL.jpg";
        case avalon::NONE:
            break;
        case avalon::UNKNOWN_ROLE:
            break;
    }

    if(role == avalon::NONE && align == avalon::GOOD)
            return desire += "GENERIC_GOOD.jpg";
    else if(role == avalon::NONE && align == avalon::EVIL)
            return desire += "GENERIC_EVIL.jpg";
    else
            return desire += "NONE.jpg";
}

std::string avalon::gui::alignmentToString( avalon::alignment_t align ) {
    switch( align ) {

    case avalon::GOOD:
        return "Good";
    case avalon::EVIL:
        return "Evil";
    case avalon::UNKNOWN_ALIGN:
        return "Unknown";
    }

    return "Alignment not recognized";
}

std::string avalon::gui::trimString( std::string in ) {
    int i = in.length( ) - 1;
    for( ; i >= 0; i-- ) {
        if( in[i] != ' ' && in[i] != '\n' && in[i] != '\t' )
            break;
    }
    std::string out = in.substr( 0, i + 1);
    return out;
}

std::string avalon::gui::getGameResultString( avalon::alignment_t player, avalon::alignment_t winner ) {
    if( winner == avalon::GOOD )
        return "Good guys won!";
    else
        return "Bad guys won!";
}
