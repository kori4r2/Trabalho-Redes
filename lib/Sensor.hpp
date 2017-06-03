#pragma once

#include<iostream>
#include<cmath>

using namespace std;

class  Sensor {
	private:
		virtual double readMeasure()=0;
	protected:
		double _measure, *_time;
	public:
		Sensor(double *time);
		double getMeasure();
		virtual ~Sensor(); // Must be declared as virtual
};
