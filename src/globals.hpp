/**
 * A file designed to hold any structs/enums/defines that should be used across all classes
 * All values are part of namespace avalon
 *
 * @author Ryan Kerr && Justin Koehler
 * @date 2015-02-16
 * @file globals.hpp
 */

#ifndef _GLOBALS_HPP
#define _GLOBALS_HPP

#define DEFAULT_PORT 42917

namespace avalon {

    static const int NUM_SPECIALS = 5;

    /*! An enumerator for the possible special roles */
    enum special_roles_t {
        MERLIN,   /*! Merlin (Good) Knows evil players */
        PERCIVAL, /*! Percival (Good) Knows of Morgana and Merlin*/
        MORDRED,  /*! Mordred (Evil) Unknown to Merlin */
        MORGANA,  /*! Morgana (Evil) Known to Percival (Disguises Merlin) */
        ASSASSIN, /*! Assassin (Evil) Gets a last ditch chance to kill Merlin, making Evil win */
        OBERON,   /*! Oberon (Evil) Unknown to other Evil players */
        NONE      /*! None Corresponds to being a generic good/evil person */
    };

    /*! An enumerator of the possible alignments */
    enum alignment_t {
        GOOD, /*! Good (Wants missions to pass) */
        EVIL /*! Evil (Wants missions to fail) */
    };

    /**
     * A function to return the alignment of a role
     *
     * @param avalon::special_roles_t the special role to examine - Passing in the special role NONE is undefined
     * @return avalon::alignment_t The alignment of the role
     */
    alignment_t roleAlignment( special_roles_t role );
}

#endif // _GLOBALS_HPP
