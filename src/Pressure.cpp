#include "../lib/Pressure.h"

Pressure::Pressure():Sensor(){
}

double Pressure::readMeasure(){

	double deltaForce = (rand()%10); // Need something close to reality, then a small variation is better
	
	if(rand()%2==0) // oscilation simulator
		_force += deltaForce;
	else
		_force -= deltaForce;

	_measure =  _force / _contactArea; // P = F/A 
	return _measure;
}

Pressure::~Pressure(){
}
