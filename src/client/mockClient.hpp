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
         * Function to send a protobuf to the server the client is connected to
         *
         * @param bufType The type of buffer that is being sent
         * @param message The protobuf to be sent, already serialized as a string (Using protobuf method)
         *
         * @return None
         */
        void sendProtobuf( avalon::network::buffers_t bufType, std::string message );
        
        MockClient( );
        
        std::string getLastProtobuf( );
        avalon::network::buffers_t getLastBufType( );
    private:
		avalon::network::buffers_t lastBufType;
		std::string lastBuf;

};

#endif // MOCKCLIENT_HPP
