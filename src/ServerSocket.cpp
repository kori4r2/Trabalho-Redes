#include "ServerSocket.hpp"

void ServerSocket::readDouble(double **vectorAddress, int clientIndex, bool *failure){
	// If a thread that started previously has already indicated failure, does nothing
	if(!(*failure)){
		// Clears the current vector position
		memset(&((*vectorAddress)[clientIndex]), 0, sizeof(double));
		// Stores the double value received in the current vector position
		int n = ::recvfrom(_clientSockets[clientIndex], &((*vectorAddress)[clientIndex]), sizeof(double), MSG_WAITALL, NULL, 0);

		// If it fails to receive message, changes the failure flag
		if(n <= 0){
			// This creates a race condition, but since we just want to detect any failure in any thread
			// the execution will not be affected by it
			*failure = true;
		}
	}
}

void ServerSocket::exitError(const char *message){
	shutdownServer();
	if(_buffer)
		free(_buffer);
	std::cerr << message << std::endl;
	exit(1);
}

void ServerSocket::shutdownServer(){
	// Closes all sockets
	for(int i = 0; i < _clientCount; i++){
		::shutdown(_clientSockets[i], 2);
	}
	_hasClients = false;
	::shutdown(_socketFD, 2);
	// Frees allocated memory for the vectors
	free(_clientSockets);
	free(_clientAddresses);
}

ServerSocket::ServerSocket(int portno, int listenSize)
	// Sets up the reference for _hasClients variable
	: hasClients(_hasClients){

	// Initiating variables
	_portno = portno;
	// In this particular case the buffer could be only the size of a double
	_buffer = (char*)malloc(sizeof(char) * 256);
	memset(&_buffer, 0, sizeof(char) * 256);
	_clientSockets = NULL;
	_clientAddresses = NULL;
	_clientCount = 0;

	// Creates a new socket, binds it and readies it to listen to connections
	_socketFD = socket(AF_INET, SOCK_STREAM, 0);
	memset(&_address, 0, sizeof(_address));
	_portno = portno;
	_address.sin_family = AF_INET;
	_address.sin_addr.s_addr = INADDR_ANY;
	_address.sin_port = htons(_portno);
	::bind(_socketFD, (struct sockaddr*)&_address, sizeof(_address));
	::listen(_socketFD, listenSize);
}

void ServerSocket::acceptClient(){
	// Accepts a new connection (blocks execution while doing so)
	struct sockaddr_in newClientAddress;
	unsigned int clientAddrSize = sizeof(newClientAddress);
	int newSocket = ::accept(_socketFD, (struct sockaddr*)&newClientAddress, &clientAddrSize);
	// If the connection was successful
	if(newSocket >= 0){
		// Makes sure the _hasClients variable is true
		_hasClients = true;
		// Increments counter
		_clientCount++;
		// Enlarges vectors and stores the relevant variables
		_clientSockets = (int*)realloc(_clientSockets, sizeof(int) * _clientCount);
		_clientSockets[_clientCount - 1] = newSocket;
		_clientAddresses = (struct sockaddr_in*)realloc(_clientAddresses, sizeof(struct sockaddr_in) * _clientCount);
		_clientAddresses[_clientCount - 1] = newClientAddress;
	}else{
		// In case of error, stops execution
		exitError("Failed to get new client socket");
	}
}

int ServerSocket::broadcastMessage(const void *message, std::size_t size){
	int counting[_clientCount];
	return 0;
}

int ServerSocket::sendMessage(const void *message, std::size_t size, int index){
	return 0;
}

void ServerSocket::listenToClients(double **vectorAddress, int *size){
	// Updates vector size
	*size = _clientCount;
	*vectorAddress = (double*)realloc(*vectorAddress, sizeof(double) * _clientCount);
	// Sets the error flag
	bool failure = false;
	// Creates thread vector
	std::vector<std::thread> threads;
	// Start a thread for each message receiving function
	for(int i = 0; i < _clientCount; i++){
		threads.push_back(std::thread(&ServerSocket::readDouble, vectorAddress, i, &failure));
	}
	// Wait for all of them to finish
	for(auto& thread : threads)
		thread.join();

	if(failure)
		exitError("Error when receiving message from client");
}

ServerSocket::~ServerSocket(){
	shutdownServer();
	free(_buffer);
}
