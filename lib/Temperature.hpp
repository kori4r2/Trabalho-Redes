
#include "Sensor.hpp"
#include <cstdlib>

using namespace std;
class Temperature : public Sensor{
	private:
	
		double readMeasure();
	public:
		Temperature(double *time);

		~Temperature();

};
