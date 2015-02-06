/*
 * An example class to demonstrate gtest fixtures
 *
 * @author Ryan Kerr
 * @date 02-03-2015
 * @file example_class.cpp
 */

#include "example_class.hpp"

// Constructor
ExampleClass::ExampleClass( int input ) {

	this->input = input;
}

// Returns the value that was passed in during instantiation
int ExampleClass::getInput( void ) {

	return this->input;
}
