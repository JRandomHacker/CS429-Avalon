/**
 * @file example_program.hpp
 * @author Ryan Kerr
 * @date 02/02/2015
 */
#ifndef EXAMPLE_PROGRAM_HPP
#define EXAMPLE_PROGRAM_HPP

#include <string>

/**
 *  A function which simply returns the integer 1
 *  Used for example gtest unittests
 *
 *  @return 1
 */
int returnOne( void );


/**
 *  A function which simply returns the string "Hello"
 *  Used for example gtest unittests
 *
 *  @return An std::string containing "Hello"
 */
std::string returnHello( void );

/**
 * A function which throws an error
 * Used for example gtest unittests
 *
 * @return None
 */
void throwError( void );

#endif // EXAMPLE_PROGRAM_HPP
