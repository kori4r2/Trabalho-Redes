#ifndef Pressure_H
#define Pressure_H

#include "Sensor.h"
#include <cstdlib>

using namespace std;
class Pressure : public Sensor{
	private:
		double ContactArea=10;
		double Force;
		double readMeasure();
	public:
		Pressure();
		~Pressure();	
};


#endif
