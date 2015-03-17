/*
 * Implementation of mock client class
 *
 * @file client.cpp
 * @author Justin Koehler
 * @date 2015-03-16
 */

#include "mockClient.hpp"
#include <string>
#include <iostream>

MockClient::MockClient( ) { 
    lastBuf = "";
};

void MockClient::sendProtobuf( avalon::network::buffers_t bufType, std::string message) {
    lastBufType = bufType;
    lastBuf = message;
}

std::string MockClient::getLastProtobuf( ) {
    
    return lastBuf;
}

avalon::network::buffers_t MockClient::getLastBufType( ) {
    return lastBufType;
}
