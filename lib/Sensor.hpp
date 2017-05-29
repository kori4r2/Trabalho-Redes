#pragma once

#include<iostream>
#include<math.h>

using namespace std;

class  Sensor {
	protected:
		double _measure, *_time;
		virtual double readMeasure()=0;
	public:
		Sensor(double *time);
		double getMeasure();
		~Sensor();
};
