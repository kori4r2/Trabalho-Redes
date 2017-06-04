
#include "Sensor.hpp"
#include <cstdlib>

using namespace std;
class Temperature : public Sensor{
	public:
		double readMeasure() override;
		Temperature(double *time);
		~Temperature();
};
