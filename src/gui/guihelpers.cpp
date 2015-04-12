
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


std::string avalon::gui::roleToImage( avalon::special_roles_t role ) {

    std::string desire = ":/images/images/";

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
            return desire += "GENERIC.jpg";
        case avalon::UNKNOWN_ROLE:
            return desire += "GENERIC.jpg";
    }

    return desire += "GENERIC.jpg";

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
