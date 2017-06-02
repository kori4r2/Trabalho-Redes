//#include "Pressure.hpp"
#include "ServerSocket.hpp"
#include<stdio.h>
#include <thread>

using namespace std;

#define N_SENSORS 12
#define PORTNO 2354 //port number

void error(const char *msg){
    perror(msg);
    exit(1);
}

int main2(){
	double *values = NULL;
	int size;

	// Criar lista de sockets em ordem pre-definida (1 socket para cada sensor)
	ServerSocket serv(PORTNO, N_SENSORS);

	// Conectar todos os sockets, em ordem
	int i;

	for(i = 0; i < N_SENSORS; i++){
		serv.acceptClient();
	}

	// Enquanto houver ao menos um socket conectado
	while(serv.hasClients){
		// 	Enviar mensagem de inicio
		serv.broadcastMessage("send the values");
		
		// 	Iniciar um read em cada socket, com cada chamada em uma thread
		// 	join nas threads criadas
		serv.listenToClients(&values, &size)

		// 	Calcular sensores virtuais de acordo com os alores recebidos
		// 	Exibir as informacoes na tela
	}

	// Apaga toda a memoria alocada
	delete[] serv;

	return 0;

}
