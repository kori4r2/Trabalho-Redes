#pragma once

#include "Sensor.hpp"
#include <cstdlib>

using namespace std;
class Longitude : public Sensor{
	private:
		double readMeasure();
	public:
		Longitude(double *time);
		~Longitude();
};

