#include "SensorManager.hpp"
#include "ClientSocket.hpp"
#include <cstdio>
#include <thread>
#include <vector>

#define NUMBER_OF_LOOPS 2000

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
	for(int i = 0; i < NUMBER_OF_LOOPS && allGood; i++){
	// 	Recebe uma mensagem do server usando a funcao read (sai do loop caso nao consiga conectar)
		std::thread *threads = new thread[N_SENSORS];
		char buffer[N_SENSORS][256];
		for(int j = 0; j < N_SENSORS; j++){
			memset(buffer[j], 0, 256);
			threads[j] = std::thread(&ClientSocket::listenToMessage, sockets[j], buffer[j], 16);
		}
		std::cout << "Waiting for messages from server" << std::endl;
		for(int j = 0; j < N_SENSORS; j++){
			threads[j].join();
			if(buffer[j] == 0) // This is an error
				allGood = false; // Maybe change this to alter a bool variable that exits the loop
		}
		if(allGood){
			std::cout << "Successfully received messages from server" << std::endl;
			delete[] threads;
			threads = new thread[N_SENSORS];
			// 	Envia os novos valores dos sensores utilizando a funcao write e colocando cada chamada em uma thread
			for(int j = 0; j < N_SENSORS; j++){
				double value = sensors[j]->getMeasure();
				threads[j] = std::thread(&ClientSocket::sendDouble, sockets[j], value);
			}
			std::cout << "Sending values to the server" << std::endl;
			// 	Da join em todas as threads criadas
			for(int j = 0; j < N_SENSORS; j++){
				threads[j].join();
			}
			delete[] threads;
			std::cout << "Successfully sent values to the server" << std::endl;
			time += 0.4;
		}
	}
	// Desconecta caso ainda esteja conectado
	for(int i = 0; i < N_SENSORS; i++)
		delete &sockets[i];
	// Apaga toda a memoria alocada
	free(sockets);
	::deleteSensors(sensors);
	free(sensors);
	
	return 0;
}
