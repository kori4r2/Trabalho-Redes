#include "ServerSocket.hpp"
#include <stdio.h>
#include <math.h>
#include "SensorManager.hpp"

void checkFinish(bool *allGood);
void error(const char *msg);
double gastoCombustivel(double altura, double velocidade, double peso);
int perigoColisao(double xA, double yA, double zA, double xB, double yB, double zB);
double temperaturaMediaInterna(double t1, double t2, double t3);
double tempoEstimado(double x, double y, double v);
void getSensorUpdate();

int main(int argc, char *argv[]){

	int portno;
	bool allGood = true;

	std::cout << "Select the port number" << std::endl;
	scanf("%d", &portno);

	// Creates the server socket
	ServerSocket *serv = new ServerSocket(portno, 5, N_SENSORS);

	// Creates a thread for listening to client messages and a thread for reading the standard input
	std::thread listenThread = std::thread(&ServerSocket::listenToClients, serv, &allGood);
	std::thread inputThread = std::thread(checkFinish, &allGood);
	std::cout << "Type \"q\" to halt execution" << std::endl;
	// Loops while the user has not input the necessary command
	while(allGood){
		// Loops through all clients(airplanes) of the server
		for(int i = 0; i < serv->clientCount; i++){
			// Updates the virtual sensors according to the new values inside the vectors
			double v1 = gastoCombustivel(serv->values[i][ALTITUDE1], serv->values[i][SPEED], serv->values[i][WEIGHT]);
			int v2 = perigoColisao(serv->values[i][LATITUDE1], serv->values[i][LONGITUDE1], serv->values[i][ALTITUDE1], serv->values[i][LATITUDE2], serv->values[i][LONGITUDE2], serv->values[i][ALTITUDE2]);
			double v3 = temperaturaMediaInterna(serv->values[i][TEMPERATURE1], serv->values[i][TEMPERATURE2], serv->values[i][TEMPERATURE3]);
			double v4 = tempoEstimado(serv->values[i][LATITUDE1], serv->values[i][LONGITUDE1], serv->values[i][SPEED]);

			// Shows information of the airplane on the screen
			std::cout << "============================================" << std::endl;
			std::cout << "Aeronave numero " << i << std::endl;
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
		// If at least one client has connected, makes sure the exit instruction is at the end
		if(serv->clientCount > 0)
			std::cout << "Type \"q\" to halt execution" << std::endl;
	}
	// Stops both threads
	inputThread.join();
	listenThread.join();

	// Frees allocated memory
	delete serv;

	return 0;

}

void checkFinish(bool *allGood){
	char input[256];
	do{
		scanf("%s", input);
		input[255] = '\0';
	}while(strcmp(input, "q") != 0 && *allGood);
	(*allGood) = false;
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
	return (sqrt( (x - y/2)*(x - y/2) + (y - x/3)*(y - x/3)) / v);
}
