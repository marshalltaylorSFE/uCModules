#ifndef PANELDATAOBJECTS_H
#define PANELDATAOBJECTS_H

#include <stdint.h>

//DataObject is the standard data object as defined by the abstract
// hardware inteface GenericHardwareDescription
class DataObject
{
public:
	DataObject( void );
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
	ButtonDataObject( void );
	~ButtonDataObject( void );
};	

class LedDataObject : public DataObject
{
public:
	LedDataObject( void );
	~LedDataObject( void );
};

class KnobDataObject : public DataObject
{
public:
	KnobDataObject( void );
	~KnobDataObject( void );
};


#endif

void dumpObject( DataObject * inputObjectPtr );