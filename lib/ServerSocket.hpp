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
#include <unistd.h>
#include <fcntl.h>

class ServerSocket{
	private:
		int _socketFD, _portno, _clientCount, _nSensors;
		struct sockaddr_in _address, *_clientAddresses;
		bool _hasClients;
		double **_values;

		void exitError(const char *message);
		void shutdownServer();

	public:
		// Indicates whether there are clients connected (can only be accessed for reading)
		const int &clientCount;
		const double **values;


		// Class constructor, receives port number and listen queue size as parameters
		ServerSocket(int portno, int listenSize, int nSensors);

		// Waits until a new client connects and saves it to the list of connected clients
		void acceptClient();

		// Sends a message to all the clients connected, returns number of successful messages sent,
		// or zero if any of the messages fail
		int broadcastMessage(const void *message, std::size_t size);

		// Sends a message to a specific client, returns 1 for success and 0 for failure
		int sendMessage(const void *message, std::size_t size, int index);

		// Receives a double from each client conected, saves them to the vector pointed by the furst
		// parameter and stores the vector size on the location pointed by the second parameter
		void listenToClients(bool *allGood);

		void updateClientDouble(double **vectorAddress, int index, bool *failure);

		// Class destructor
		~ServerSocket();
};
