#include "ServerSocket.hpp"

void ServerSocket::exitError(const char *message){
	shutdownServer();
	std::cerr << message << std::endl;
	exit(1);
}

void ServerSocket::shutdownServer(){
	// Sends shutdown message to all clients saved
	for(int j = 0; j < 5; j++){
		for(int i = 0; i < _clientCount; i++){
			::sendto(_socketFD, "shutdown\0", 9 * sizeof(char), 0, (struct sockaddr*)&_clientAddresses[i], sizeof(_clientAddresses[i]));
		}
	}
	// Closes server socket
	::shutdown(_socketFD, 2);
	// Frees allocated memory for the vectors
	free(_clientAddresses);
	for(int i = 0; i < _clientCount; i++)
		free(_values[i]);
	free(_values);
}

ServerSocket::ServerSocket(int portno, int listenSize, int nSensors)
	// Sets up the reference for _clientCount variable
	: clientCount(_clientCount){

	// Initiating variables
	_nSensors = nSensors;
	_portno = portno;
	_clientAddresses = NULL;
	_values = NULL;
	_clientCount = 0;

	// Creates a new socket, binds it and readies it to listen to connections without blocking
	_socketFD = socket(AF_INET, SOCK_DGRAM, 0);
	::fcntl(_socketFD, F_SETFL, O_NONBLOCK);
	// In case of failure, exits program
	if(_socketFD < 0)
		exitError("Failed to open server socket");
	memset(&_address, 0, sizeof(_address));
	_portno = portno;
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = INADDR_ANY;
	_address.sin_port = htons(_portno);
	int n = ::bind(_socketFD, (struct sockaddr*)&_address, sizeof(_address));
	// In case of failure, exits program
	if(n < 0)
		exitError("Failed to bind server socket");
	::listen(_socketFD, listenSize);
}

void ServerSocket::listenToClients(bool *allGood){
	while(*allGood){
		char buffer[256];
		struct sockaddr_in clientAddress/* = sockaddr_in()*/;
		socklen_t sockLength/* = socklen_t()*/;
		bool isNewClient = true;
		int clientPos = _clientCount;

		// Listen to socket
		int n = ::recvfrom(_socketFD, (void*)buffer, 256 * sizeof(char), 0, (struct sockaddr*)&clientAddress, &sockLength);
		// If a message was successfully received
		if(n > 0){
			// Checks if the client that sent the message is a new one
			for(int i = 0; i < _clientCount && isNewClient; i++){
				if(_clientAddresses[i].sin_addr.s_addr == clientAddress.sin_addr.s_addr/* && _clientAddresses[i].sin_port == clientAddress.sin_port*/){
					isNewClient = false;
					clientPos = i;
				}
			}
			// If it is a new client, stores their info and creates a new value vector for them
			if(!isNewClient){
				_clientCount++;
				_clientAddresses = (struct sockaddr_in*)realloc(_clientAddresses, sizeof(struct sockaddr_in) * _clientCount);
				_clientAddresses[_clientCount - 1] = clientAddress;
				_values = (double**)realloc(_values, sizeof(double*) * _clientCount);
				_values[_clientCount - 1] = (double*)calloc(_nSensors, sizeof(double));
			}
			// Copies the info to the correct position inside the vector
			unsigned char index;
			double value;
			memcpy(&index, buffer, sizeof(unsigned char));
			memcpy(&value, ((unsigned char*)buffer) + 1, sizeof(double));
			_values[clientPos][index] = value;
		}
	}
}

ServerSocket::~ServerSocket(){
	shutdownServer();
}
