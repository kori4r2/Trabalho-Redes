#include "../lib/Pressure.h"

Pressure::Pressure():Sensor(){
	
}

double Pressure::readMeasure(){	
	
	double deltaForce = (rand()%10); // Need something close to reality, then a small variation is better
	
	if(rand()%2==0) // oscilation simulator
		Force+=deltaForce;
	else
		Force-=deltaForce;

	measure =  Force / ContactArea; // P = F/A 
	return measure;
}

Pressure::~Pressure(){



}
