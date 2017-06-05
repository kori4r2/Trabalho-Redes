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
	std::cout << "type the server port number" << std::endl;
	scanf("%d", &portno);
	std::cout << "type the server machine name" << std::endl;
	scanf("%s", serverName);

	// Criar todos os sensores
	Sensor **sensors;
	sensors = (Sensor**)malloc(sizeof(Sensor) * N_SENSORS);
	::initalizeSensors(sensors, &time);

	// Criar todos os sockets (um para cada sensor)
	ClientSocket *sockets;
	sockets = (ClientSocket*)malloc(sizeof(ClientSocket) * N_SENSORS);
	// Conectar cada socket ao servidor na ordem esperada pelo servidor
	for(int i = 0; i < N_SENSORS; i++)
		sockets[i] = ClientSocket(portno, serverName);

	// Utiliza um for para enviar um numero maximo de mensagens
	bool allGood = true;
	double values[N_SENSORS];
	std::thread *threads = new thread[N_SENSORS];

	for(int j = 0; j < N_SENSORS; j++){
		values[j] = 0;
		threads[j] = std::thread(&ClientSocket::keepSendingMessage, sockets[j], &(values[j]), sizeof(double), &allGood);
	}
	std::thread readingInput = thread(checkFinish, &allGood);

	double startTime = std::time(NULL);
	while(allGood){
	// 	Recebe uma mensagem do server usando a funcao read (sai do loop caso nao consiga conectar)
		// Envia os novos valores dos sensores utilizando a funcao write e colocando cada chamada em uma thread
		time = std::time(NULL) - startTime;
		for(int i = 0; i < N_SENSORS; i++)
			sensors[i]->getMeasure();
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
	while(strcmp(input, "quit") != 0 && *allGood){
		std::cout << "type 'quit' to stop running: ";
		scanf("%s", input);
		input[255] = '\0';
	}
	(*allGood) = false;
}
