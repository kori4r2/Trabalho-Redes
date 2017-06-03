#include "ClientSocket.hpp"

void ClientSocket::exitError(const char *message){

	::shutdown(_socketFD, 2);
	
	std::cerr << message << std::endl;
	exit(1);
}

ClientSocket::ClientSocket(int portno, const char *serverName){
	_portno = portno;
	_socketFD = socket(AF_INET, SOCK_STREAM, 0);
	if(_socketFD < 0)
		exitError("Error creating client socket");
	_server = ::gethostbyname(serverName);
	if(_server == NULL)
		exitError("Error getting host info");
	memset(&_serverAddress, 0, sizeof(_serverAddress));
	_serverAddress.sin_family = AF_INET;
	memcpy(&_serverAddress.sin_addr.s_addr, _server->h_addr, _server->h_length);
	_serverAddress.sin_port = htons(_portno);
	int n = ::connect(_socketFD, (struct sockaddr *)&_serverAddress, sizeof(_serverAddress));
	if(n < 0)
		exitError("Error connecting to server");
}

int ClientSocket::sendMessage(const void *buffer, std::size_t size){
	int n = ::sendto(_socketFD, buffer, size, 0, NULL, 0);
	return n;
}

int ClientSocket::sendDouble(const double number){
	return sendMessage(&number, sizeof(number));
}

int ClientSocket::listenToMessage(void *buffer, std::size_t size){
	return ::recvfrom(_socketFD, buffer, size, MSG_WAITALL, NULL, 0);
}

int ClientSocket::listenToMessage(double *number){
	return listenToMessage(number, sizeof(*number));
}

int ClientSocket::listenToMessage(char *message){
	return listenToMessage(message, strlen(message));
}

ClientSocket::~ClientSocket(){
	::shutdown(_socketFD, 2);
}
