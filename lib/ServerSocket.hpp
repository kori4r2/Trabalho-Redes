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
#include <arpa/inet.h>
#include <netdb.h>

class ServerSocket{
	private:
		int _socketFD, _portno, _clientCount, _nSensors;
//		struct sockaddr_in _address, *_clientAddresses;
		struct addrinfo *_address;
		struct sockaddr *_clientAddresses;
		double **_values;

		void exitError(const char *message);
		void shutdownServer();

	public:
		// Indicates how many clients are connected (can only be accessed for reading)
		const int &clientCount;

		// Class constructor, receives port number and listen queue size as parameters
		ServerSocket(int portno, int listenSize, int nSensors);

		double getValue(int client, int index);

		// Receives a double and an idex from each client conected and either saves it to the
		// corresponding position inside the matrix, or expands it and saves the new client to
		// the client list
		void listenToClients(bool *allGood);

		// Class destructor
		~ServerSocket();
};
