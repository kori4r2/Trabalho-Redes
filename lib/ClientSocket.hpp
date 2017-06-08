#pragma once
#include <iostream>
#include <iomanip>
#include <cstring>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <fcntl.h>
#include <arpa/inet.h>

class ClientSocket{
	private:
		int _socketFD, _portno;
		struct addrinfo *_serverCandidates, *_server;

		void exitError(const char *message);

	public:
		// Basic constructor
		ClientSocket(int portno, const char *serverName);
		// Keeps sending a datagram consisting of the index parameter and the content of
		// number parsed to a string to the saved server address
		void keepSendingMessage(double *number, unsigned char index, bool *allGood);
		// Sends size bytes stored in buffer to the saved server address
		int sendMessage(const void *buffer, std::size_t size);
		// Receives a message from the server and stores it to buffer
		int listenToMessage(void *buffer, std::size_t size);
		// Keeps listening to the server and checks if there was an order to shutdown (donesn't seem to work)
		void getServerShutdown(bool *allGood);
		// Simple destructor
		~ClientSocket();
};
