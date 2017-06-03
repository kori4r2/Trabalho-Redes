#pragma once

#include "Sensor.hpp"
#include <cstdlib>

using namespace std;
class Pressure : public Sensor{
	private:
		double _contactArea;
		double _force;
	public:
		double readMeasure();
		Pressure(double *time);
		Pressure(double *time, double contactArea);
		~Pressure();
};
