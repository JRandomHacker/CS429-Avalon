/*
 * @file example_program.cpp
 * @author Ryan Kerr
 * @date 02/02/2015
 */

#include <string>
#include <stdexcept>

#include "example_program.hpp"

int returnOne( void ) {

    return 1;
}

std::string returnHello( void ) {

    return std::string( "Hello" );
}

void throwError( void ) {

    throw std::overflow_error("Test error");
}
