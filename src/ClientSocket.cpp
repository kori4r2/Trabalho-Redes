#include "ClientSocket.hpp"

void ClientSocket::exitError(const char *message){

	::shutdown(_socketFD, 2);
	
	std::cerr << message << std::endl;
	exit(1);
}

ClientSocket::ClientSocket(int portno, const char *serverName){
	// Stores server info and creates client socket
	_portno = portno;
	_socketFD = socket(AF_INET, SOCK_DGRAM, 0);
	if(_socketFD < 0)
		exitError("Error creating client socket");
	::fcntl(_socketFD, F_SETFL, O_NONBLOCK);
	// Gets a reference to the server
	_server = ::gethostbyname(serverName);
	if(_server == NULL)
		exitError("Error getting host info");
//	memset(&_serverAddress, 0, sizeof(_serverAddress));
	_serverAddress.sin_family = AF_INET;
	memcpy(&_serverAddress.sin_addr.s_addr, _server->h_addr, _server->h_length);
	_serverAddress.sin_port = htons(_portno);
	// Binds the socket to an address
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = htonl(INADDR_ANY);
	_address.sin_port = htons(0);
	if(::bind(_socketFD, (struct sockaddr*)&_address, sizeof(_address)) < 0){
		exitError("Error binding socket");
	}
}

void ClientSocket::keepSendingMessage(void *buffer, unsigned char index, std::size_t size, bool *allGood){
	// Allocates memory for the buffer
	void *newBuffer = malloc(sizeof(unsigned char) + size);
	// Copies the relevant info
	memcpy(newBuffer, &index, sizeof(unsigned char));
	memcpy( ((unsigned char*)newBuffer) + 1, buffer, size);
	// While nothing is wrong, send the buffer info to the server
	while(*allGood){
		if(sendMessage(newBuffer, sizeof(unsigned char) + size) > 0)
			std::cout << "something was sent" << std::endl;
	}
	free(newBuffer);
}

// Sends a message to the saved server address
int ClientSocket::sendMessage(const void *buffer, std::size_t size){
	return ::sendto(_socketFD, buffer, size, 0, (struct sockaddr*)&_serverAddress, _server->h_length);
}

// Receives messages
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
