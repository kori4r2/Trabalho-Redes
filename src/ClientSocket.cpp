#include "ClientSocket.hpp"

void ClientSocket::exitError(const char *message){

	::shutdown(_socketFD, 2);
	
	std::cerr << message << std::endl;
	exit(1);
}

ClientSocket::ClientSocket(int portno, const char *serverName){
	_portno = portno;
	_socketFD = socket(AF_INET, SOCK_DGRAM, 0);
	if(_socketFD < 0)
		exitError("Error creating client socket");
	::fcntl(_socketFD, F_SETFL, O_NONBLOCK);
	_server = ::gethostbyname(serverName);
	if(_server == NULL)
		exitError("Error getting host info");
	memset(&_serverAddress, 0, sizeof(_serverAddress));
	_serverAddress.sin_family = AF_INET;
	memcpy(&_serverAddress.sin_addr.s_addr, _server->h_addr, _server->h_length);
	_serverAddress.sin_port = htons(_portno);
}

void ClientSocket::keepSendingMessage(void *buffer, unsigned char index, std::size_t size, bool *allGood){
	void *newBuffer = malloc(sizeof(unsigned char) + size);
	memcpy(newBuffer, &index, sizeof(unsigned char));
	memcpy( ((unsigned char*)newBuffer) + 1, buffer, size);
	while(*allGood){
		if(sendMessage(newBuffer, sizeof(unsigned char) + size) > 0)
			std::cout << "something was sent" << std::endl;
	}
	free(newBuffer);
}

int ClientSocket::sendMessage(const void *buffer, std::size_t size){
	return ::sendto(_socketFD, buffer, size, 0, (struct sockaddr*)&_serverAddress, _server->h_length);
}

int ClientSocket::listenToMessage(void *buffer, std::size_t size){
	return ::recv(_socketFD, buffer, size, 0);
}

void ClientSocket::getServerShutdown(bool *allGood){
	char buffer[9] = "testing1";
	do{
		if(listenToMessage(buffer , 9 * sizeof(char)) <= 0)
			strcpy(buffer, "testing1");
	}while(*allGood && strcmp(buffer, "shutdown") != 0);
}

ClientSocket::~ClientSocket(){
	::shutdown(_socketFD, 2);
}
