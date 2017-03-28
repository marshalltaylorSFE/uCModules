#include "HardwareInterfaces.h"
#include <Arduino.h>

//Note, the ought to be deconstructed if the hardware interfaces are used
//dynamically

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

//Arduino Digital out
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

//Arduino Analog in
ArduinoAnalogIn::ArduinoAnalogIn( int inputPin )
{
	pinMode(inputPin, INPUT);
	localData.size = 2;
	localData.data = new uint8_t[localData.size];
	pin = inputPin;

}

void ArduinoAnalogIn::readHardware( void )
{
	*(uint16_t *)localData.data = analogRead( pin );
};
