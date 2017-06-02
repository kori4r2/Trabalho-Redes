#include "ServerSocket.hpp"

ServerSocket::ServerSocket(int portno, int listenSize)
	: hasClients(_hasClients){
	_portno = portno;
	_buffer = (char*)malloc(sizeof(char) * 256);
	_clientSockets = NULL;
	_clientAdresses = NULL;
	_clientCount = 0;

	_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	memset(&_address, 0, sizeof(_address));
	_portno = portno;
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = INADDR_ANY;
	_address.sin_port = htons(_portno);

}

void ServerSocket::acceptClient(){
}

ServerSocket::~ServerSocket(){
	free(_buffer);
}
