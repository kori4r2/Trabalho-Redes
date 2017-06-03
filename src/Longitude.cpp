#include "Latitude.hpp"

Latitude::Latitude(double *time)
	_time = time;
	_measure = 30;
		
}

double Latitude::readMeasure(){ 
	// circular variation	
	// r**2 = x**2 + y**2 
	/* 
	 *	y = sqrt ( radius*2 - x*2 )
	 * */	
	
	
	if ((*_time) >100)
		(*_time)=-100;

	_measure = sqt( 100 - (*_time)**2  ) + (3-rand()%4); 

	return _measure;
}

Latitude::~Latitude(){
	// Adds nothing to base class destructor
}
