#pragma once

#include<iostream>
#include<cmath>

using namespace std;

class  Sensor {
	protected:
		double _measure, *_time;
	public:
		virtual double readMeasure()=0;
		double getMeasure();
		Sensor(double *time);
		virtual ~Sensor(); // Must be declared as virtual
};
