#include "Speed.hpp"

Speed::Speed(double *time)
	: Sensor(time){
	_measure = 0.0;
	_factor = 5.0;
}

double Speed::readMeasure(){
	_measure += _factor;
	if(fmod((*_time), 3600))
		_factor *= (-1.0);
	return _measure;
}

Speed::~Speed(){
}
