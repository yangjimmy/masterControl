#include "Pressure.h"
#include "Arduino.h"
#include "AD5760.h"
#include "MAXREFDES11.h"

Pressure::Pressure(int MISO, int MOSI, int SCK, int LDAC, int SS_dac, int SS_adc)
{
	AD5760 dac = AD5760(MISO, MOSI, SCK, LDAC, SS_dac);
	MAXREFDES11 adc = MAXREFDES11(MISO, MOSI, SCK, SS_adc);
}

void Pressure::setPressure(float pressure) {
  float voltage;
  // error check
  if (pressure > 35) {
    Serial.println("Error, resetting to 35");
    pressure = 35;
  } else if (pressure < 0) {
    Serial.println("Error, resetting to 0");
    pressure = 0;
  } else {
    // no error
  }
  uint16_t data = (pressure + 0.3)/35*65535;
  data = data & 0xffff;
  uint8_t first = 0b00010000 | (data >> 12);
  uint16_t second = (data << 4) & (0xffff);
  
  dac.sendSequence(first, (uint8_t)((second >> 8) & 0xff), (uint8_t)((second) & 0xf0))
}

double Pressure::getPressure() {
	double voltage = adc.readVoltage();
	return voltage/10*35;
}