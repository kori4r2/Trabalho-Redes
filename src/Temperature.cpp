#include "Temperature.hpp"

Temperature::Temperature(double *time)
	: Sensor(time){
	_measure = 24;	
}


double Temperature::readMeasure(){

	double variation = ((rand() % 1001) - 500)/1000.0; 

	double temperature_altitude = (2.71828182846)*((-0.5) * (*_time) * (*_time) ) / sqrt( 2.0); // speed follows a gaussian distribution

	_measure =  100+(temperature_altitude)*24/1000000 ; // as higher it takes, colder it comes 
	return _measure;
}

Temperature::~Temperature(){
	// Adds nothing to base class destructor
}
