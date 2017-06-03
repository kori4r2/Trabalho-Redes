#include "Weight.hpp"

Weight::Weight(double *time)
	: Sensor(time){
	_measure = 300;
}

double Weight::readMeasure(){
	if(fmod((*_time), 7200))
		_measure += ((rand()%100001) - 50000) / 100.0;
	return _measure;
}
