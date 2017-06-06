#include "SensorManager.hpp"
#include "ClientSocket.hpp"
#include <cstdio>
#include <thread>
#include <vector>

#define NUMBER_OF_LOOPS 2000

void checkFinish(bool*);

int main(int argc, char *argv[]){


	double time = 0.0;
	int portno;
	char serverName[256];
	std::cout << "type the server port number: ";
	scanf("%d", &portno);
	std::cout << "type the server machine name: ";
	scanf("%s", serverName);

	// Criar todos os sensores
	Sensor **sensors;
	sensors = (Sensor**)malloc(sizeof(Sensor) * N_SENSORS);
	::initalizeSensors(sensors, &time);

	// Criar todos os sockets (um para cada sensor)
	ClientSocket *sockets;
	sockets = (ClientSocket*)malloc(sizeof(ClientSocket) * N_SENSORS);

	bool allGood = true;
	double values[N_SENSORS];
	std::thread *threads = new thread[N_SENSORS];

	for(int j = 0; j < N_SENSORS; j++){
		values[j] = 0;
		threads[j] = std::thread(&ClientSocket::keepSendingMessage, sockets[j], &(values[j]), j, sizeof(double), &allGood);
	}
	std::thread readingInput = thread(checkFinish, &allGood);

	double startTime = std::time(NULL);
	while(allGood){
		time = std::time(NULL) - startTime;
		for(int i = 0; i < N_SENSORS; i++)
			values[i] = sensors[i]->getMeasure();
	}
	for(int i = 0; i < N_SENSORS; i++)
		threads[i].join();
	delete[] threads;
	readingInput.join();

	// Desconecta caso ainda esteja conectado
	for(int i = 0; i < N_SENSORS; i++)
		delete &sockets[i];
	// Apaga toda a memoria alocada
	free(sockets);
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
