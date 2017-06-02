#pragma once
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

class ServerSocket{
	private:
		int _sockfd, _portno, _n, _clientCount, *_clientSockets;
		struct sockaddr_in _address, *_clientAdresses;
		char *buffer;

	public:
		// Class constructor, receives port number and listen queue size as parameters
		ServerSocket(int portno, int listenSize);
		// Waits until a new client connects and saves it to the list of connected clients
		void acceptClient();
		// Receives a double from each client conected, saves it to the vector passed
		void listenToClients(double **vectorAddress, int *size);
		// Class destructor
		~ServerSocket();
};
