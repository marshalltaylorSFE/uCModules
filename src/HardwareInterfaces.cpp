#include "HardwareInterfaces.h"
#include <Arduino.h>

//Arduino in
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

//Arduino out
ArduinoDigitalOut::ArduinoDigitalOut( int inputPin )
{
	pinMode(inputPin, OUTPUT);
	localData.size = 1;
	localData.data = new uint8_t[localData.size];
	pin = inputPin;

}

void ArduinoDigitalOut::writeHardware( void )
{
	digitalWrite( pin, *localData.data );
};
