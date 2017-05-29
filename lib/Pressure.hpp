#pragma once

#include "Sensor.hpp"
#include <cstdlib>

using namespace std;
class Pressure : public Sensor{
	private:
		double _contactArea;
		double _force;
		double readMeasure() override;
	public:
		Pressure();
		Pressure(double contactArea);
		~Pressure();
};
