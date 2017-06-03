#include "Altitude.hpp"

Altitude::Altitude(double *time)
	: Sensor(time){
	_measure = 30;
}

double Altitude::readMeasure(){ 
	
	// altitude = (x)*(x-10e6)  -> altitude = 0  <-> ocean 
	// 			    -> altitude 500**2 <-> meters from ocean 
	
	_measure = (*_time)*((*_time)-10e6) + (10-rand()%11);

	return _measure;
}

Altitude::~Altitude(){
	// Adds nothing to base class destructor
}
