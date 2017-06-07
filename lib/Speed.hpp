#pragma once

#include "Sensor.hpp"
#define MAX_SPEED 1200
#define MIN_SPEED 0

class Speed : public Sensor{
	private:
		double _factor, _lastTime;
	public:
		double readMeasure() override;
		Speed(double *time);
		~Speed();
};
