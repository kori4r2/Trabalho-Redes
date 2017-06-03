#pragma once

#include "Sensor.hpp"
#include <cstdlib>

using namespace std;
class Longitude : public Sensor{
	public:
		double readMeasure();
		Longitude(double *time);
		~Longitude();
};

