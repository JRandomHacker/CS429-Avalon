
#include "guihelpers.hpp"
#include "player.hpp"
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

std::string avalon::gui::roleToFlavorText(avalon::special_roles_t role, avalon::alignment_t align) {
    switch( role ){
        case avalon::MERLIN:
            return "You are Merlin, the Great Wizard. You know who is evil, and must stay hidden.";
        case avalon::MORDRED:
            return "You are Mordred, Master of Evil. Your presence is unknown to Merlin.";
        case avalon::MORGANA:
            return "You are Morgana, the Deciever. You appear as Merlin.";
        case avalon::ASSASSIN:
            return "You are Assassin, Agent of Evil. You have an extra chance to eliminate Merlin.";
        case avalon::OBERON:
            return "You are Oberon, a Strange Man. Your presence is unknown to Evil.";
        case avalon::PERCIVAL:
            return "You are Percival, Guardian of Good. You know who Merlin is.";
        case avalon::NONE:
            break;
        case avalon::UNKNOWN_ROLE:
            break;
    }

    if(role == avalon::NONE && align == avalon::GOOD)
            return "You are a Loyal Servant of Arthur.";
    else if(role == avalon::NONE && align == avalon::EVIL)
            return "You are a Minion of Mordred.";
    else
            return "You are Hodor, One Who Does Not Exist. This role should not exist.";
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

std::string avalon::gui::getGameResultString( avalon::alignment_t player, bool assassin, Player* assassinTarget,
                                              avalon::alignment_t winner ) {
    if( winner == avalon::GOOD && player == avalon::GOOD && !assassin )
        return "Rejoice!  A new day has dawned, and good has triumphed over evil, as you always knew it would.  Are there those among "
                "you who are agents of evil still?  Maybe.  I bet they feel pretty dumb right now.";
    else if( winner == avalon::GOOD && player == avalon::EVIL && !assassin )
        return "Good has triumphed over evil, and the kingdom rejoices!  You can't help but be swept away by the reveling.  It "
                "literally cannot be helped.  After all, you were on their side all along, right?";
    else if( winner == avalon::GOOD && player == avalon::GOOD && assassin )
        return "The minions of Mordred are scattered, and the kingdom is safe.  Nothing can bring you down today, not even the "
                "news of " + assassinTarget->getName( ) + "'s tragic and bloody death.  " + assassinTarget->getName( )
                + " would definitely want you to keep partying.";
    else if( winner == avalon::GOOD && player == avalon::EVIL && assassin )
        return "Foiled again!  Why does evil never get its day?  Oh, well.  At least you and your friends brought "
                + assassinTarget->getName( ) + " down with you.  The memory of " + assassinTarget->getName( ) + "'s death "
                "brings a big smile to your face, and you fit right in with the revelers.";
    else if( winner == avalon::EVIL && player == avalon::GOOD && !assassin )
        return "Things look bleak.  You tremble as loud footsteps march past the entrance to the broom closet you have hidden "
                "yourself in.  A loud scream announces the death of another of your allies.  Maybe you can escape the castle "
                "if you go now...";
    else if( winner == avalon::EVIL && player == avalon::EVIL && !assassin )
        return "At last!  The kingdom is yours!  You wonder briefly why no one ever believed you when you said that evil would "
                "always triumph.  Then you shrug, wipe the blood of your enemies off your hands, and join the feast.  It "
                "tastes like victory.";
    else if( winner == avalon::EVIL && player == avalon::GOOD && assassin )
        return "How could a night of such triumph turn to terror?  You were celebrating; good had triumphed, as was just.  "
                "But the next thing you knew, Merlin--he had been " + assassinTarget->getName( ) + " all along--lay dead on the "
                "floor.  The blood--it's everywhere.  Is it yours?  You can't remember.";
    else
        return "Yes!  Your last hope, your last chance to plunge the kingdom into darkness, came through!  Merlin--who had been "
                + assassinTarget->getName( ) + " all along--is dead at your feet, and the night of celebration has turned to "
                "beautiful chaos.  Ah, victory is sweet.";
}
