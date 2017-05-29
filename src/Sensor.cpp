#include "../lib/Sensor.h"




Sensor::Sensor(){
		
}
Sensor::~Sensor(){

}
double Sensor::getMeasure(){
	return readMeasure();
}

