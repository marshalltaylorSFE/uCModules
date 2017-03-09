#ifndef HARDWARE_INTERFACES_H
#define HARDWARE_INTERFACES_H

#include <Arduino.h>

//DataObject is the standard data object as defined by the abstract
// hardware inteface GenericHardwareDescription
class DataObject
{
public:
	DataObject( void ){
		size = -1;
	};
	uint8_t * data;
	int size;
};
//Specific types are used to create local instances that construct
//with the correct size.
//
//A few panel components might want the same fingerprint of data.
//
//pass references-of to getData and setData
class ButtonDataObject : public DataObject
{
public:
	ButtonDataObject( void ){
		size = 1;
		data = new uint8_t[size];
	};
	~ButtonDataObject( void ){
		delete[] data;
	}
};	

class LedDataObject : public DataObject
{
public:
	LedDataObject( void ){
		size = 1;
		data = new uint8_t[size];
	};
	~LedDataObject( void ){
		delete[] data;
	}
};

class KnobDataObject : public DataObject
{
public:
	KnobDataObject( void ){
		size = 2;
		data = new uint8_t[size];
	};
	~KnobDataObject( void ){
		delete[] data;
	}
};

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
			memcpy( inputObject, &localData, sizeof(localData) );
		}
	};
	void setData( DataObject * inputObject ){
		if( sizeof(*inputObject) == sizeof(localData) )
		{
			memcpy( &localData, inputObject, sizeof(inputObject) );
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