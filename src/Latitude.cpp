#include "Longitude.hpp"

Longitude::Longitude(double *time)
	: Sensor(time){
	_measure = 30;
		
}

double Longitude::readMeasure(){ 
	
	// r**2 = x**2 + y**2 
	/* 
	 *	y = sqrt ( radius*2 - x*2 )
	 * */

	double time = fmod(*_time, 90);

	_measure = sqrt( 90 - (time)*2  )+ (3-rand()%4); // circular path with small variation

	return _measure;
}

Longitude::~Longitude(){
	// Adds nothing to base class destructor
}
