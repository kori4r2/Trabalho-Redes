#pragma once

#include "Sensor.hpp"
#include <cstdlib>

using namespace std;
class Altitude : public Sensor{
	private:
		double readMeasure();
	public:
		Altitude(double *time);
		~Altitude();
};

