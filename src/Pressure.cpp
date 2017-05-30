#include "Pressure.hpp"

Pressure::Pressure(double *time)
	: Sensor(time){
	_contactArea = 10.0;
	_force = 10.0;
}

Pressure::Pressure(double *time, double contactArea)
	: Pressure(time){
	_contactArea = contactArea;
}

double Pressure::readMeasure(){

	double variation = ((rand() % 1001) - 500)/100.0; // Need something close to reality, then a small variation from -5.00 to 5.00 is better

	double speed = exp((-0.05) * (*_time) * (*_time) ) / sqrt(M_PI * 2.0); // speed follows a gaussian distribution
	_force = 200 * speed; // Air resistance equals to a constant multipliying the speed;

	_measure = (_force / _contactArea) + variation; // P = F/A 
	return _measure;
}

Pressure::~Pressure(){
	// Adds nothing to base class destructor
}
