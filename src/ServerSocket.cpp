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
			::sendto(_socketFD, "shutdown\0", 9 * sizeof(char), 0, &_clientAddresses[i], 14 * sizeof(char));
		}
	}
	// Closes server socket
	::shutdown(_socketFD, 2);
	// Frees allocated memory for the vectors
	free(_clientAddresses);
	for(int i = 0; i < _clientCount; i++)
		free(_values[i]);
	free(_values);
	freeaddrinfo(_address);
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
	struct addrinfo hints;
	::memset(&hints, 0, sizeof(hints));
	hints.ai_protocol = 0;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_family = AF_UNSPEC;
	hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;
	char portStr[256];
	memset(portStr, 0, 256 * sizeof(char));
	snprintf(portStr, 256, "%d", _portno);
	int n = ::getaddrinfo(NULL, portStr, &hints, &_address);
	if(n != 0)
		exitError("Falied to assign IP address");
	_socketFD = socket(_address->ai_family, _address->ai_socktype, _address->ai_protocol);
	// In case of failure, exits program
	if(_socketFD < 0)
		exitError("Failed to open server socket");
	::fcntl(_socketFD, F_SETFL, O_NONBLOCK);
	n = ::bind(_socketFD, _address->ai_addr, _address->ai_addrlen);
	// In case of failure, exits program
	if(n < 0)
		exitError("Failed to bind server socket");
	char aux[256];
	::inet_ntop(_address->ai_family, &(_address->ai_addr->sa_data), aux, sizeof(char) * 256);
	std::cout << "current address: " << aux << std::endl;
	::listen(_socketFD, listenSize);
}

double ServerSocket::getValue(int client, int index){
	if(client >= 0 && client < _clientCount && index >= 0 && index < _nSensors)
		return _values[client][index];
	else
		return 0.0;
}

void ServerSocket::listenToClients(bool *allGood){
	while(*allGood){
		char buffer[256];
		memset(buffer, 0, 256 * sizeof(char));
		struct sockaddr clientAddress = sockaddr();
		socklen_t sockLength = socklen_t();
		bool isNewClient = true;
		int clientPos = _clientCount;

		// Listen to socket
		int n = ::recvfrom(_socketFD, (void*)buffer, 256 * sizeof(char), 0, &clientAddress, &sockLength);
		// If a message was successfully received
		if(n > 0){
			// Checks if the client that sent the message is a new one
			for(int i = 0; i < _clientCount && isNewClient; i++){
				if(::memcmp(_clientAddresses[i].sa_data, clientAddress.sa_data, 14 * sizeof(char)) == 0){
					isNewClient = false;
					clientPos = i;
				}
			}
			// If it is a new client, stores their info and creates a new value vector for them
			if(isNewClient){
				_clientAddresses = (struct sockaddr*)realloc(_clientAddresses, sizeof(struct sockaddr) * (_clientCount + 1));
				_clientAddresses[_clientCount] = clientAddress;
				_values = (double**)realloc(_values, sizeof(double*) * (_clientCount + 1));
				_values[_clientCount] = (double*)malloc(_nSensors * sizeof(double));
				for(int j = 0; j < _nSensors; j++)
					_values[_clientCount][j] = 0.0;
				_clientCount++;
			}
			// Copies the info to the correct position inside the vector
			unsigned char index;
			double value;
			memcpy(&index, buffer, sizeof(unsigned char));
			memcpy(&value, ((unsigned char*)buffer) + 1, sizeof(double));
			if(index >= 0 && index < _nSensors){
//				if((int)index == 1)
//					std::cout << "saved " << value << " to pos (" << clientPos << ", " << (int)index << ")" << std::endl;
				_values[clientPos][(int)index] = value;
			}
		}
	}
}

ServerSocket::~ServerSocket(){
	shutdownServer();
}
