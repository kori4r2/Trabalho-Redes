#include "SensorManager.hpp"
#include "ClientSocket.hpp"
#include <cstdio>
#include <thread>

void checkFinish(bool*);

int main(int argc, char *argv[]){


	double time = 0.0;
	int portno;
	char serverName[256];
	std::cout << "type the server port number: ";
	scanf("%d", &portno);
	std::cout << "type the server machine name: ";
	scanf("%s", serverName);

	// Creates and initializes the sensors
	Sensor **sensors;
	sensors = (Sensor**)malloc(sizeof(Sensor) * N_SENSORS);
	::initalizeSensors(sensors, &time);

	// Creates all sockets (one for each sensor)
	ClientSocket **sockets;
	sockets = new ClientSocket*[N_SENSORS];

	bool allGood = true;
	double values[N_SENSORS];
	// Creates a vector of threads to send the info of all sensors
	std::thread *threads = new thread[N_SENSORS];
	// Creates a vector of threads to make all sockets get the server shutdown message
	std::thread *checkServerShutdown = new thread[N_SENSORS];

	// For every sensor
	for(int j = 0; j < N_SENSORS; j++){
		// Creates a new client socket
		sockets[j] = new ClientSocket(portno, serverName);
		// saves an initial value to the vector
		values[j] = 0.0;
		// Starts a thread to keep sending the contents of that vector position to the server
		threads[j] = std::thread(&ClientSocket::keepSendingMessage, std::ref(*sockets[j]), &(values[j]), j, &allGood);
		// Starts a thread to keep waiting for a shutdown message from the server
		checkServerShutdown[j] = std::thread(&ClientSocket::getServerShutdown, std::ref(*sockets[j]), &allGood);
	}
	// Starts a thread to see if user closes through standard input
	std::thread readingInput = std::thread(checkFinish, &allGood);

	// While the program is still running, updates the values inside the vector
	double startTime = std::time(NULL);
	while(allGood){
		time = std::time(NULL) - startTime;
		for(int i = 0; i < N_SENSORS; i++){
			values[i] = sensors[i]->getMeasure();
		}
	}

	// Joins all threads created
	for(int i = 0; i < N_SENSORS; i++){
		threads[i].join();
		checkServerShutdown[i].join();
	}
	readingInput.join();
	// Frees all allocated memory
	for(int i = 0; i < N_SENSORS; i++)
		delete sockets[i];
	delete[] sockets;
	delete[] threads;
	delete[] checkServerShutdown;
	::deleteSensors(sensors);
	free(sensors);
	
	return 0;
}

void checkFinish(bool *allGood){
	char input[256];
	do{
		std::cout << "type 'q' to stop running: ";
		scanf("%s", input);
		input[255] = '\0';
	}while(strcmp(input, "q") != 0 && *allGood);
	(*allGood) = false;
}
