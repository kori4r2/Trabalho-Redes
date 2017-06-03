#include "Pressure.hpp"
#include "Altitude.hpp"
#include "Latitude.hpp"
#include "Longitude.hpp"
#include "Temperature.hpp"
#include "Speed.hpp"
#include "Weight.hpp"

#define ALTITUDE1 0
#define LATITUDE1 1
#define LONGITUDE1 2
#define ALTITUDE2 3
#define LATITUDE2 4
#define LONGITUDE2 5
#define PRESSURE 6
#define TEMPERATURE1 7
#define TEMPERATURE2 8
#define TEMPERATURE3 9
#define SPEED 10
#define WEIGHT 11

#define N_SENSORS 12

void initalizeSensors(Sensor **sensorArray, double *time){
	if(sensorArray != NULL){
		sensorArray[ALTITUDE1] = new Altitude(time);
		sensorArray[ALTITUDE2] = new Altitude(time);
		sensorArray[LATITUDE1] = new Latitude(time);
		sensorArray[LATITUDE2] = new Latitude(time);
		sensorArray[LONGITUDE1] = new Longitude(time);
		sensorArray[LONGITUDE2] = new Longitude(time);
		sensorArray[TEMPERATURE1] = new Temperature(time);
		sensorArray[TEMPERATURE2] = new Temperature(time);
		sensorArray[TEMPERATURE3] = new Temperature(time);
		sensorArray[PRESSURE] = new Pressure(time);
		sensorArray[SPEED] = new Speed(time);
		sensorArray[WEIGHT] = new Weight(time);
	}
}

void deleteSensors(Sensor **sensorArray){
	for(int i = 0; i < N_SENSORS; i++){
		delete sensorArray[i];
	}
}
