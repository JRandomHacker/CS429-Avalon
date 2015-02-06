/** Anything in the gtest source directory,
 * or our programs source directory,
 * is already in the path (From Makefile)
 *
 * I've tried to add every test type I expect we might need,
 * however, there are several I've omitted, which can be found at:
 * https://code.google.com/p/googletest/wiki/V1_7_AdvancedGuide
 *
 * gtest/gtest.h must be included for tests to work
 *
 * @file example_unittest.cpp
 * @author Ryan Kerr
 * @date 02/02/2015
 */
#include "example_program.hpp"
#include "gtest/gtest.h"

#include <stdexcept>
#include <string>

/* NOTE: for each given example, there is an equivilent ASSERT_*.
 * If an EXPECT is failed, the test case will continue running
 * However, if an ASSERT is failed, the rest of the test case is ignored
 *
 * Additionally, you can add a custom failure message as follows:
 * EXPECT_TRUE( false ) << "This is my custom error message";
 */
TEST( ExampleTestSuite, BooleanTests ) {

    EXPECT_TRUE( returnOne() );
    EXPECT_FALSE( !returnOne() );
}

TEST( ExampleTestSuite, EqualityTests ) {

    EXPECT_EQ( 1, returnOne() );
    EXPECT_NE( 0, returnOne() );
}

TEST( ExampleTestSuite, LogicalTests ) {

    EXPECT_LT( 0, returnOne() ); // Less than
    EXPECT_LE( 1, returnOne() ); // Less than or equal
    EXPECT_GT( 2, returnOne() ); // Greater than
    EXPECT_GE( 1, returnOne() ); // Greater than or equal
}

// NOTE all string tests expect a C string
TEST( ExampleTestSuite, StringTests ) {

    // Case sensitive
    EXPECT_STREQ( "Hello", returnHello().c_str() );
    EXPECT_STRNE( "hello", returnHello().c_str() );
    // Case insensitive
    EXPECT_STRCASEEQ( "Hello", returnHello().c_str() );
    EXPECT_STRCASENE( "world", returnHello().c_str() );
}

// Test to see if a peice of code throws an exception
TEST( ExampleTestSuite, ThrowsTests ) {

    EXPECT_THROW( throwError(), std::overflow_error );
    EXPECT_ANY_THROW( throwError() );
    EXPECT_NO_THROW( returnOne() );
}
