#include "ClientSocket.hpp"

void ClientSocket::exitError(const char *message){

	::freeaddrinfo(_serverCandidates);
	::shutdown(_socketFD, 2);
	
	std::cerr << message << std::endl;
	exit(1);
}

ClientSocket::ClientSocket(int portno, const char *serverName){
	// Stores server info and creates client socket
	_portno = portno;
	// Gets a reference to the server

	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = 0;
	hints.ai_flags = AI_NUMERICSERV;
	
	char portStr[256];
	snprintf(portStr, 256, "%d", _portno);
	int n =::getaddrinfo(serverName, portStr, &hints, &_serverCandidates);
	if(n != 0)
		exitError("Error getting host info");
	static int callNumber = 0;
	std::cout << "socket number" << callNumber << "------------------------------" << std::endl;
	_server = NULL;
	_socketFD = -1;
	for(_server = _serverCandidates; _server != NULL && _socketFD < 0; _server = _server->ai_next){
		char aux[256];
		std::cout << "server address: " << ::inet_ntop(_server->ai_family, _server->ai_addr->sa_data, aux, 256 * sizeof(char)) << std::endl;
		_socketFD = socket(_server->ai_family, _server->ai_socktype, _server->ai_protocol);
	}
	std::cout << "-----------------------------------------------" << std::endl;
	if(_socketFD >= 0)
		::fcntl(_socketFD, F_SETFL, O_NONBLOCK);
	if(_server == NULL)
		exitError("Error creating client socket");
	callNumber++;
}

void ClientSocket::keepSendingMessage(double *number, unsigned char index, bool *allGood){
	// Allocates memory for the buffer
	void *newBuffer = malloc(sizeof(unsigned char) + (255 * sizeof(char)));
	// Copies the relevant info
	memcpy(newBuffer, &index, sizeof(unsigned char));
	// While nothing is wrong, send the buffer info to the server
	while(*allGood){
		std::ostringstream strStream;
		strStream << std::fixed << std::setprecision(6) << *number;
		char buffer[256];
		memset(buffer, 0, sizeof(char) * 255);
		strcpy(buffer, strStream.str().c_str());
		memcpy( ((unsigned char*)newBuffer) + 1, buffer, 255 * sizeof(char));
		if((index) == 10)
			std::cout << "sending index = " << (int)index << "double = " << buffer << std::endl;
		sendMessage(newBuffer, sizeof(unsigned char) + (255 * sizeof(char)));
	}
	free(newBuffer);
}

// Sends a message to the saved server address
int ClientSocket::sendMessage(const void *buffer, std::size_t size){
	return ::sendto(_socketFD, buffer, size, 0, _server->ai_addr, _server->ai_addrlen);
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
	::freeaddrinfo(_serverCandidates);
	::shutdown(_socketFD, 2);
}
