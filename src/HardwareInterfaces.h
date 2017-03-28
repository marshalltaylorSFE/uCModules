#ifndef HARDWARE_INTERFACES_H
#define HARDWARE_INTERFACES_H

#include <Arduino.h>
#include <PanelDataObjects.h>

//The abstract class GenericHardwareDescription must be inheireted
// by the interface to whatever external driver is being used.  Data must be present
// or get-able by the readHardware and writeHardware definitions.
class GenericHardwareDescription
{
public:
	virtual void readHardware( void ){};
	virtual void writeHardware( void ){};
	void getData( DataObject * inputObject ){
		if( sizeof(*inputObject) == sizeof(localData) )
		{
			//memcpy( inputObject, &localData, sizeof(localData) );
			memcpy( inputObject->data, &localData.data, localData.size );
			//inputObject->size = localData.size;
		}
	};
	void setData( DataObject * inputObject ){
		if( sizeof(*inputObject) == sizeof(localData) )
		{
			//memcpy( &localData, inputObject, sizeof(*inputObject) );
			memcpy( &localData.data, inputObject->data, inputObject->size );
			//localData.size = inputObject->size;
		}
	};
//protected:
	GenericHardwareDescription( void ){};
	//Data vended upstream
	DataObject localData;
};

//This is the most basic hardware type, the Arduino digital input
class ArduinoDigitalIn : public GenericHardwareDescription
{
public:
	ArduinoDigitalIn( int inputPin );
	void readHardware( void );
protected:
	int pin;
};

//Arduino digital out
class ArduinoDigitalOut : public GenericHardwareDescription
{
public:
	ArduinoDigitalOut( int inputPin );
	void writeHardware( void );
protected:
	int pin;
};

class ArduinoAnalogIn : public GenericHardwareDescription
{
public:
	ArduinoAnalogIn( int inputPin );
	void readHardware( void );
protected:
	int pin;
};

#endif