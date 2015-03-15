/**
 * File with a helper function to pop up an error message
 *
 * @file displayerrors.hpp
 * @author Ryan Kerr
 * @date 2015-03-12
 */
#ifndef DISPLAYERRORS_HPP
#define DISPLAYERRORS_HPP
#include <QWidget>

/**
 * A function to pop up an error dialog box based on the error code returned
 * All error codes should be added here, and all errors should be created with this
 *
 * @param errorCode The error code to display
 * @param parent The GUI window spawning the error
 * @return None
 */
void displayError( int errorCode, QWidget* parent );

#endif
