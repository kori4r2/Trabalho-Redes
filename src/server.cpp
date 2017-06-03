#include "ServerSocket.hpp"
#include<stdio.h>
#include <math.h>

#define a 0 
#define b 1
#define C 2
#define d 3
#define e 4
#define f 5 
#define g 6
#define h 7
#define p 8
#define q 9
#define r 10 
#define l 11

#define N_SENSORS 12

void error(const char *msg);
double gastoCombustivel(double altura, double velocidade, double peso);
int perigoColisao(double xA, double yA, double zA, double xB, double yB, double zB);
double temperaturaMediaInterna(double t1, double t2, double t3);
double tempoEstimado(double x, double y, double v);

int main(int argc, char *argv[]){

	double *values = NULL;
	int size, portno;

	std::cout << "Select the port number" << std::endl;
	scanf("%d", &portno);

	// Criar lista de sockets em ordem pre-definida (1 socket para cada sensor)
	ServerSocket *serv = new ServerSocket(portno, N_SENSORS);

	// Conectar todos os sockets, em ordem
	for(int i = 0; i < N_SENSORS; i++){
		serv->acceptClient();
	}

	// Enquanto houver ao menos um socket conectado
	// Enviar mensagem de inicio checando se foi bem sucedida
	while(serv->hasClients && (serv->broadcastMessage("send the values", 16) > 0)){
		
		// 	Iniciar um read em cada socket, com cada chamada em uma thread
		// 	join nas threads criadas
		serv->listenToClients(&values, &size);

		// 	Calcular sensores virtuais de acordo com os alores recebidos
		double v1 = gastoCombustivel(values[1], values[2], values[3]);
		int v2 = perigoColisao(0, 0, 0, 0, 0, 0);
		double v3 = temperaturaMediaInterna(0, 0, 0);
		double v4 = 1;

		// 	Exibir as informacoes na tela
		std::cout << "============================================" << std::endl;
		std::cout << "Combustivel gasto por hora: " << v1 << std::endl;
		std::cout << "Perigo de colisao com aeronave B: ";
		if(v2 == 0)
			std::cout << "Nao" << std::endl;
		else
			std::cout << "Sim" << std::endl;
		std::cout << "Temperatura media interna: " << v3 << std::endl;
		std::cout << "Tempo estimado de chegada: " << v4 << std::endl;
		std::cout << "============================================" << std::endl;
	}

	// Apaga toda a memoria alocada
	delete serv;

	return 0;

}

void error(const char *msg){
    perror(msg);
    exit(1);
}

double gastoCombustivel(double altura, double velocidade, double peso){
	//Note that this constant value doesn't reflect the reallity.
	double c = 24;

	return c * ((velocidade*peso)/altura);
}

int perigoColisao(double xA, double yA, double zA, double xB, double yB, double zB){
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
	return sqrt( (x - y/2)*(x - y/2) + (y - x/3)*(y - x/3) );
}
