/**
 * A class for mocking out network data for testing
 *
 * @class Client
 * @author Justin Koehler
 * @date 2015-03-16
 */

#ifndef _MOCKSERVER_HPP
#define _MOCKSERVER_HPP

#include "server.hpp"

#include <string>

class MockServer : public Server {
    public:
        /**
         * Normally a function to send a protobuf to the connected server
         * In the mock, allows us to examine the protobuf to ensure the proper data was inserted
         *
         * @param bufType The type of buffer that is being sent
         * @param message The protobuf to be sent, already serialized as a string (Using protobuf method)
         *
         * @return None
         */
        void sendProtobuf( avalon::network::buffers_t bufType, unsigned int destinationID, std::string message );

        /**
         * Constructor
         */
        MockServer( );

        /**
         * Returns the last protobuf that we were supposed to send
         *
         * @return The string representation of the protobuf (Using protobuf SerializeAsString)
         */
        std::string getLastProtobuf( );

        /**
         * Returns the type of the last protobuf we were supposed to send
         *
         * @return The type of protobuf
         */
        avalon::network::buffers_t getLastBufType( );

        /**
         * Returns the number of the last client we tried to send to
         *
         * @return The client index
         */
        unsigned int getLastClient( );
        
        /**
         * Returns the last random data from the state change
         *
         * @return The client index
         */
        unsigned int getLastRandomness( );

        /**
         * Returns the last state transition that was broadcast
         *
         * @return The state transition that was sent
         */
        avalon::network::buffers_t getLastState( );
        
        /**
         * Normally a function to send a state-change message to all clients
         * In the mock, allows us to examine the state-change to ensure the proper data was inserted
         *
         * @param bufType The type of state-change that is being sent
         * @param randomness Could be data. idunnolol.
         *
         * @return None
         */
         void broadcastStateChange( avalon::network::buffers_t bufType, unsigned int randomness );

    private:
        avalon::network::buffers_t lastBufType;
        std::string lastBuf;
        unsigned int lastClient;
        unsigned int lastRandomness;
        avalon::network::buffers_t lastState;

};

#endif // MOCKSERVER_HPP
