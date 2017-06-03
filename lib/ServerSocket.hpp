#pragma once
#include <cstdio>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <thread>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

class ServerSocket{
	private:
		int _socketFD, _portno, _n, _clientCount, *_clientSockets;
		struct sockaddr_in _address, *_clientAddresses;
		char *_buffer;
		bool _hasClients;

		void readDouble(double **vectorAddress, int clientIndex, bool *failure);
		void exitError(const char *message);
		void shutdownServer();

	public:
		// Indicates whether there are clients connected (can only be accessed for reading)
		const bool &hasClients;

		// Class constructor, receives port number and listen queue size as parameters
		ServerSocket(int portno, int listenSize);
		// Waits until a new client connects and saves it to the list of connected clients
		void acceptClient();
		// Sends a message to all the clients connected
		int broadcastMessage(const void *message, std::size_t size);
		// Sends a message to a specific client
		int sendMessage(const void *message, std::size_t size, int index);
		// Receives a double from each client conected, saves it to the vector passed
		void listenToClients(double **vectorAddress, int *size);
		// Class destructor
		~ServerSocket();
};
