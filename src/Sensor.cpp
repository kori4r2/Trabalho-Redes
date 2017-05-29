#include "Sensor.hpp"

Sensor::Sensor(double *time){
	_time = time;
}

Sensor::~Sensor(){
}

double Sensor::getMeasure(){
	return readMeasure();
}

