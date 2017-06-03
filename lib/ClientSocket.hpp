#pragma once
#include <cstdio>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

class ClientSocket{
	private:
		int _socketFD, _portno;
		struct sockaddr_in _address, _serverAddress;
		struct hosten *_server;
	public:
		ClientSocket(int portno, const char *serverName);
		void sendMessage(const void *buffer, int *size);
		void sendDouble(const double number);
		void listenToMessage(const void *buffer, int *size);
		void listenToMessage(const double *number);
		void listenToMessage(const char *message);
		~ClientSocket();
};
