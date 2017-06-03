#pragma once

#include "Sensor.hpp"
#include <cstdlib>

using namespace std;
class Altitude : public Sensor{
	public:
		double readMeasure();
		Altitude(double *time);
		~Altitude();
};

