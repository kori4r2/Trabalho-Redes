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
	bcopy(_server->h_addr, &_serverAddress.sin_addr.s_addr, _server->h_length);
	_serverAddress.sin_port = htons(_portno);
	int n = ::connect(_socketFD, (struct sockaddr *)&_serverAddress, sizeof(_serverAddress));
	if(n < 0)
		exitError("Error connecting to server");
}

int ClientSocket::sendMessage(const void *buffer, std::size_t size){
	return ::write(_socketFD, buffer, size);
}

int ClientSocket::sendDouble(const double number){
	return sendMessage(&number, sizeof(double));
}

int ClientSocket::listenToMessage(void *buffer, std::size_t size){
	return ::read(_socketFD, buffer, size);
}

ClientSocket::~ClientSocket(){
	::shutdown(_socketFD, 2);
}
