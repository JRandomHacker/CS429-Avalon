/**
 * A simple class to allow an example of a gtest fixture
 *
 * @author Ryan Kerr
 * @date 02-03-2015
 * @class ExampleClass
 */

#ifndef EXAMPLE_CLASS_HPP
#define EXAMPLE_CLASS_HPP

class ExampleClass {

    private:
        int input;

    public:
        /**
         * Public constructor
         *
         * @param input The value to set input to
         */
        ExampleClass( int input );

        /**
         * Method to return the input given at class instantiation
         *
         * @return The integer you passed in earlier
         */
        int getInput( void );
};

#endif // EXAMPLE_CLASS_HPP
