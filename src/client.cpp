#include "Pressure.hpp"
#include<stdio.h>

// compile using : g++ main.cpp -o ./src/Sensor.o ./src/Pressure.o -o main

using namespace std;

int main(){
	Pressure * sensor =NULL;
	char c ='1';
		sensor = new Pressure();
		while(c!='d'){
			printf("Pressure: %lf\n", sensor->getMeasure());
			scanf("%c", &c);
		}
		delete sensor;
	return 0;

}
