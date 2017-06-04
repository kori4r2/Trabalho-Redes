#pragma once

#include "Sensor.hpp"

class Speed : public Sensor{
	private:
		double _factor;
	public:
		double readMeasure() override;
		Speed(double *time);
		~Speed();
};
