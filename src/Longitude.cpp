#include "Latitude.hpp"

Latitude::Latitude(double *time)
	: Sensor(time){
	_measure = 30;
		
}

double Latitude::readMeasure(){ 
	// circular variation	
	// r**2 = x**2 + y**2 
	/* 
	 *	y = sqrt ( radius*2 - x*2 )
	 * */	
	
	double time = fmod(*_time, 100);

	_measure = sqrt( 100 - (time)*2  ) + (3-rand()%4); 

	return _measure;
}

Latitude::~Latitude(){
	// Adds nothing to base class destructor
}
