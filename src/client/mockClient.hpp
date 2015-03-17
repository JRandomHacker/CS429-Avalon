/**
 * A class for mocking out network data for testing
 *
 * @class Client
 * @author Justin Koehler
 * @date 2015-03-16
 */

#ifndef _MOCKCLIENT_HPP
#define _MOCKCLIENT_HPP

#include "client.hpp"

#include <string>

class MockClient : public Client {
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
        void sendProtobuf( avalon::network::buffers_t bufType, std::string message );
        
        /**
         * Constructor
         */
        MockClient( );
        
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

    private:
		avalon::network::buffers_t lastBufType;
		std::string lastBuf;

};

#endif // MOCKCLIENT_HPP
