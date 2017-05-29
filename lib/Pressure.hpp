#pragma once

#include "Sensor.h"
#include <cstdlib>

using namespace std;
class Pressure : public Sensor{
	private:
		double _contactArea = 10;
		double _force;
		double _readMeasure();
	public:
		Pressure();
		Pressure(double contactArea);
		~Pressure();
};
