#pragma once
#include <cstdio>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <functional>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

class ClientSocket{
	private:
		int _socketFD, _portno;
		struct sockaddr_in _address, _serverAddress;
		struct hostent *_server;

		void exitError(const char *message);
	public:
		ClientSocket(int portno, const char *serverName);
		void keepSendingMessage(void *buffer, std::size_t size, bool *allGood);
		int sendMessage(const void *buffer, std::size_t size);
		int sendDouble(const double number);
		int listenToMessage(void *buffer, std::size_t size);
		~ClientSocket();
};
