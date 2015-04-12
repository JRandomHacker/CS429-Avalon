#ifndef GUIHELPERS_HPP
#define GUIHELPERS_HPP

#include "globals.hpp"
#include <string>

namespace avalon
{
    namespace gui
    {
        std::string roleToString(avalon::special_roles_t role);

        //std::string roleToImage(avalon::special_roles_t role);

        std::string alignmentToString(avalon::alignment_t align);
    }
}

#endif // GUIHELPERS_HPP

