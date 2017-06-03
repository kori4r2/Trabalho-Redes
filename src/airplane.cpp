#include "SensorManager.hpp"
#include "ClientSocket.hpp"
#include <cstdio>
#include <thread>
#include <vector>

#define NUMBER_OF_LOOPS 1000

int main(int argc, char *argv[]){


	double time = 0.0;
	int portno;
	char serverName[256];
	std::cout << "type the server port number" << std::endl;
	scanf("%d", &portno);
	std::cout << "type the server machine name" << std::endl;
	scanf("%s", serverName);

	// Criar todos os sensores
	// Criar todos os sockets (um para cada sensor)
	ClientSocket *sockets;
	sockets = (ClientSocket*)malloc(sizeof(ClientSocket) * SENSORS_NUMBER);
	// Conectar cada socket ao servidor na ordem esperada pelo servidor
	for(int i = 0; i < SENSORS_NUMBER; i++)
		sockets[i] = ClientSocket(portno, serverName);
	// Utiliza um for para enviar um numero maximo de mensagens
	for(int i = 0; i < NUMBER_OF_LOOPS; i++){
	// 	Recebe uma mensagem do server usando a funcao read (sai do loop caso nao consiga conectar)
		std::vector<std::thread> threads;
		char buffer[SENSORS_NUMBER][256];
		for(int j = 0; j < SENSORS_NUMBER; j++){
			memset(buffer[j], 0, 256);
			threads.push_back(std::thread(sockets[j].listenToMessage(buffer[j], 16)));
		}
		for(int j = 0; j < SENSORS_NUMBER; j++){
			threads[j].join();
			if(buffer[j] == 0) // This is an error
				exit(1); // Maybe change this to alter a bool variable that exits the loop
		}
		threads.clear();
	// 	Envia os novos valores dos sensores utilizando a funcao write e colocando cada chamada em uma thread
	// 	Da join em todas as threads criadas
		time += 0.2;
	}
	// Desconecta caso ainda esteja conectado
	for(int i = 0; i < SENSORS_NUMBER; i++)
		delete &sockets[i];
	// Apaga toda a memoria alocada
	free(sockets);
	
	return 0;
}
