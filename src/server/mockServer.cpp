/*
 * Implementation of mock server class
 *
 * @file mockServer.cpp
 * @author Justin Koehler
 * @date 2015-03-16
 */

#include "mockServer.hpp"
#include <string>
#include <iostream>

MockServer::MockServer( ) : Server( 11111 ) { 
    lastBuf = "";
};

void MockServer::sendProtobuf( avalon::network::buffers_t bufType, unsigned int destinationID, std::string message ) {
    lastBufType = bufType;
    lastBuf = message;
    lastClient = destinationID;
}

std::string MockServer::getLastProtobuf( ) {
    
    return lastBuf;
}

avalon::network::buffers_t MockServer::getLastBufType( ) {
    return lastBufType;
}

void MockServer::broadcastStateChange( avalon::network::buffers_t bufType, unsigned int randomness ) {
    lastState = bufType;
    lastRandomness = randomness;
}

avalon::network::buffers_t MockServer::getLastState( ) {
    return lastState;
}

unsigned int MockServer::getLastClient( ) {
    return lastClient;
}

unsigned int MockServer::getLastRandomness( ) {
    return lastRandomness;
}
