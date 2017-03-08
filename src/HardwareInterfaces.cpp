#include "HardwareInterfaces.h"
#include <Arduino.h>

ArduinoDigitalIn::ArduinoDigitalIn( int inputPin )
{
	pinMode(inputPin, INPUT_PULLUP);
	localData.size = 1;
	localData.data = new uint8_t[localData.size];
	pin = inputPin;

}

void ArduinoDigitalIn::readHardware( void )
{
	*localData.data = digitalRead( pin );
};

