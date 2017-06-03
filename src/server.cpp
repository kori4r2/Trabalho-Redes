//#include "Pressure.hpp"
#include "ServerSocket.hpp"
#include<stdio.h>
#include <thread>
#include <math.h>

using namespace std;

#define N_SENSORS 12
#define PORTNO 2354 //port number

void error(const char *msg){
    perror(msg);
    exit(1);
}

double gastoCombustivel(double altura, double velocidade, double peso){
	//Note that this constant value doesn't reflect the reallity.
	const double c = 24;

	return c*(altura/(velocidade*peso));
}

int perigoColisão(double xA, double yA, double zA, double xB, double yB, double zB){
	double x, y, z, dist;

	x = xA - xB;
	x = x*x;
	
	y = yA - yB;
	y = y*y;
	
	z = zA - zB;
	z = z*z;

	dist = sqrt(x + y + z);

	if(dist < 500){
		return 1;
	}

	return 0;
}

double temperaturaMediaInterna(double t1, double t2, double t3){
	return (t1+t2+t3)/3.0;
}

double tempoEstimado(double x, double y, double v){
	//return sqrt( (x - y/2)*(x - y/2) + (y - x/3)*(y - x/3) )
	return 1.0;
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
		double v1 = gastoCombustivel(values[1], values[2], values[3]);
		int v2 = perigoColisão;
		double v3 = temperaturaMediaInterna;
		double v4 = 1;

		// 	Exibir as informacoes na tela
		cout << "====================" << endl;
		cout << "Combustivel gasto por hora: " << v1 << endl;
		cout << "Perigo de colisão com aeronave B: ";
		if(v2 == 0){
			cout << "Não" << endl;
		} else {
			cout << "Sim" << endl;
		}
		cout << "Temperatura média interna: " << v3 << endl;
		cout << "Tempo estimado de chegada: " << v4 << endl;
		cout << "====================" << endl;
	}

	// Apaga toda a memoria alocada
	delete[] serv;

	return 0;

}
