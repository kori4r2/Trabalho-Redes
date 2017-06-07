#pragma once

#include "Sensor.hpp"
#include <cstdlib>

#define MAX_ALT 100000 // no idea 

using namespace std;
class Altitude : public Sensor{
	public:
		double readMeasure() override;
		Altitude(double *time);
		~Altitude();
};

