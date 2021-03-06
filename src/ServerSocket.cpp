#include "ServerSocket.hpp"

void ServerSocket::exitError(const char *message){
	shutdownServer();
	std::cerr << message << std::endl;
	exit(1);
}

void ServerSocket::shutdownServer(){
	// Sends shutdown message to all clients saved (Does not seem to work)
	for(int j = 0; j < 5; j++){
		for(int i = 0; i < _clientCount; i++){
			::sendto(_socketFD, "shutdown", 9 * sizeof(char), 0, &_clientAddresses[i], 14 * sizeof(char));
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
	// hints is used to determine how the server address will be configured on the call to getaddrinfo():
	// any valid protocol; datagram socket; any address family; allows the use of listen;
	// port information will be passed as a number;
	struct addrinfo hints;
	::memset(&hints, 0, sizeof(hints));
	hints.ai_protocol = 0;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_family = AF_UNSPEC;
	hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;
	char portStr[256];
	memset(portStr, 0, 256 * sizeof(char));
	snprintf(portStr, 256, "%d", _portno);
	// If it fails, exits with error message
	int n = ::getaddrinfo(NULL, portStr, &hints, &_address);
	if(n != 0)
		exitError("Falied to assign IP address");
	// Creates the socket for the server
	_socketFD = socket(_address->ai_family, _address->ai_socktype, _address->ai_protocol);
	// In case of failure, exits program
	if(_socketFD < 0)
		exitError("Failed to open server socket");
	// Makes sure the call to listen will not block execution
	::fcntl(_socketFD, F_SETFL, O_NONBLOCK);
	// And binds the socket so it can start listening
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
	// returns desired value when parameters are valid, 0 when not
	if(client >= 0 && client < _clientCount && index >= 0 && index < _nSensors)
		return _values[client][index];
	else
		return 0.0;
}

void ServerSocket::listenToClients(bool *allGood){
	while(*allGood){
		// Declaration and initialization of necessary variables
		char buffer[256];
		memset(buffer, 0, 256 * sizeof(char));
		struct sockaddr clientAddress = sockaddr();
		socklen_t sockLength = socklen_t();
		bool isNewClient = true;
		int clientPos = _clientCount;

		// Try to receive a message from socket
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
			// The number is converted back from string format to be stored
			char numberBuffer[255];
			memcpy(&index, buffer, sizeof(unsigned char));
			memcpy(numberBuffer, buffer + 1, 255 * sizeof(char));
			value = atof(numberBuffer);
			if(index >= 0 && index < _nSensors)
				_values[clientPos][(int)index] = value;
		}
	}
}

ServerSocket::~ServerSocket(){
	shutdownServer();
}
