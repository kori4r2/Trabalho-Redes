#ifndef Sensor_H
#define Sensor_H

#include<iostream>
#include<math.h>

using namespace std;

class  Sensor {
	protected:
		double measure;
		virtual double readMeasure()=0;
	public:
		Sensor();
		double getMeasure();
		~Sensor();
};



#endif 
