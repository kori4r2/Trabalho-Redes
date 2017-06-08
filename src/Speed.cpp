#include "Speed.hpp"

Speed::Speed(double *time)
	: Sensor(time){
	_measure = 0.0;
	_factor = 5.0;
	_lastTime = -1.0;
}

double Speed::readMeasure(){
	if(_lastTime == -1.0)
		_lastTime = *_time;
	_measure += _factor * (*_time - _lastTime);
	_lastTime = *_time;
	if(_measure > MAX_SPEED)
		_measure = MAX_SPEED;
	else if(_measure < MIN_SPEED)
		_measure = MIN_SPEED;
	if((*_time > 10) && (fmod((*_time), 3600) == 0))
		_factor *= (-1.0);
	return _measure;
}

Speed::~Speed(){
}
