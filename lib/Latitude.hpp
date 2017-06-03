#pragma once

#include "Sensor.hpp"
#include <cstdlib>

using namespace std;
class Latitude : public Sensor{
	private:
		double readMeasure();
	public:
		Latitude(double *time);
		~Latitude();
};

