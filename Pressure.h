#ifndef Pressure_h
#define Pressure_h
#include "Arduino.h"
#include "AD5760.h"

class Pressure
{
	public:
		Pressure (int MISO, int MOSI, int SCK, int LDAC, int SS);
		void setVoltage();
		void setPressure(float pressure);
		double getPressure();
}

#endif