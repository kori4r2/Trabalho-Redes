#pragma once

#include "Sensor.hpp"
#include <cstdlib>

using namespace std;
class Latitude : public Sensor{
	public:
		double readMeasure() override;
		Latitude(double *time);
		~Latitude();
};

