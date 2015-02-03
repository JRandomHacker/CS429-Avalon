/*
 * An example of using a gtest fixture to allow setting up and tearing down
 * before each test case.  Intended as a supplement to example_unittest.cpp
 */
#include "example_class.hpp"
#include "gtest/gtest.h"

// Your fixture name will be the name of the test suite, must extend ::testing:Test
class ExampleTestFixture: public ::testing::Test
{
    public:
        // Variables you want your tests to access must be public
        ExampleClass* myClass;

        // Run before every test
        void SetUp( void )
        {
            myClass = new ExampleClass( exampleValue );
        }

        // Run after every test
        void TearDown( void )
        {
            delete myClass;
            myClass = NULL;
        }

        // Run only during setup (before any test)
        ExampleTestFixture( void )
        {
            exampleValue = 5;
        }

        // Run during teardown (after every test)
        ~ExampleTestFixture( void )
        {
            exampleValue = 10;
        }

    private:
        int exampleValue;

};

// Test_F is to use a fixture, the test suite name must be the same as the fixture class
TEST_F( ExampleTestFixture, EqualityTest )
{
    ASSERT_EQ( 5, myClass->getInput() );
}
