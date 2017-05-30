#include "Sensor.hpp"

Sensor::Sensor(double *time){
	_time = time;
}

Sensor::~Sensor(){
	// Nothing to destroy
}

double Sensor::getMeasure(){
	return readMeasure();
}

