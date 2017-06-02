#pragma once
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

class ServerSocket{
	private:
		int _sockfd, _portno, _n, _clientCount, *_clientSockets;
		struct sockaddr_in _address, *_clientAdresses;
		char *_buffer;
		bool _hasClients;

	public:
		// Indicates whether there are clients connected (can only be accessed for reading)
		const bool &hasClients;

		// Class constructor, receives port number and listen queue size as parameters
		ServerSocket(int portno, int listenSize);
		// Waits until a new client connects and saves it to the list of connected clients
		void acceptClient();
		// Sends a message to all the clients connected
		void broadcastMessage(const char *message);
		// Sends a message to a specific client
		void sendMessage(const char *message);
		// Receives a double from each client conected, saves it to the vector passed
		void listenToClients(double **vectorAddress, int *size);
		// Class destructor
		~ServerSocket();
};
