#pragma once

#include "Sensor.hpp"

class Weight : public Sensor{
	public:
		double readMeasure();
		Weight(double *time);
		~Weight();
};
