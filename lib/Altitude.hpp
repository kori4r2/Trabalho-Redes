#pragma once

#include "Sensor.hpp"
#include <cstdlib>

using namespace std;
class Altitude : public Sensor{
	public:
		double readMeasure() override;
		Altitude(double *time);
		~Altitude();
};

