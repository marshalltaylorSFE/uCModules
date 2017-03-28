#include <Arduino.h>
#include <PanelDataObjects.h>

DataObject::DataObject( void )
{
	size = -1;
}

ButtonDataObject::ButtonDataObject( void ){
	size = 1;
	data = new uint8_t[size];
}

ButtonDataObject::~ButtonDataObject( void ){
	delete[] data;
}

LedDataObject::LedDataObject( void ){
	size = 1;
	data = new uint8_t[size];
}

LedDataObject::~LedDataObject( void ){
	delete[] data;
}

KnobDataObject::KnobDataObject( void )
{
	size = 2;
	data = new uint8_t[size];
Serial.print((uint32_t)&(*this), HEX);
Serial.print(", +");
Serial.print(sizeof(data));
Serial.print("B @");
Serial.println((uint32_t)&(*data), HEX);
}

KnobDataObject::~KnobDataObject( void )
{
Serial.print((uint32_t)&(*this), HEX);
Serial.print(", -");
Serial.print(sizeof(data));
Serial.print("B @");
Serial.println((uint32_t)&(*data), HEX);
	delete[] data;
}
