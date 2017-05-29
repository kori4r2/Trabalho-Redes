#pragma once

#include "Sensor.hpp"
#include <cstdlib>

using namespace std;
class Pressure : public Sensor{
	private:
		double _contactArea;
		double _force;
		double readMeasure();
	public:
		Pressure(double *time);
		Pressure(double *time, double contactArea);
		~Pressure();
};
