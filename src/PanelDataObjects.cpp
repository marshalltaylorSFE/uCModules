#include <Arduino.h>
#include <PanelDataObjects.h>

DataObject::DataObject( void )
{
	size = -1;
	data = NULL;
}

ButtonDataObject::ButtonDataObject( void ){
	size = 1;
	data = new uint8_t[size];
Serial.print((uint32_t)&(*this), HEX);
Serial.print(", +");
Serial.print(sizeof(data));
Serial.print("B @");
Serial.println((uint32_t)&(*data), HEX);
}

ButtonDataObject::~ButtonDataObject( void ){
Serial.print((uint32_t)&(*this), HEX);
Serial.print(", -");
Serial.print(sizeof(data));
Serial.print("B @");
Serial.println((uint32_t)&(*data), HEX);
	delete[] data;
}

LedDataObject::LedDataObject( void ){
	size = 1;
	data = new uint8_t[size];
Serial.print((uint32_t)&(*this), HEX);
Serial.print(", +");
Serial.print(sizeof(data));
Serial.print("B @");
Serial.println((uint32_t)&(*data), HEX);
}

LedDataObject::~LedDataObject( void ){
Serial.print((uint32_t)&(*this), HEX);
Serial.print(", -");
Serial.print(sizeof(data));
Serial.print("B @");
Serial.println((uint32_t)&(*data), HEX);
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


//dumpObject lists a DataObject's contents, plus the contents of it's contained data
void dumpObject( DataObject * inputObjectPtr )
{
	Serial.print("Obj Addr: 0x");
	Serial.println((uint32_t)&(*inputObjectPtr), HEX);
	uint8_t * bytePtr = (uint8_t *)inputObjectPtr;
	for( int i = 0; i < sizeof(*inputObjectPtr); i++ )
	{
		Serial.print("0x");
		Serial.print(*bytePtr,HEX);
		Serial.print(", ");
		bytePtr++;
	}
	Serial.println();
	
	Serial.print("Containted data: ");
	bytePtr = (uint8_t *)inputObjectPtr->data;
	for( int i = 0; i < inputObjectPtr->size; i++ )
	{
		Serial.print("0x");
		Serial.print(*bytePtr,HEX);
		Serial.print(", ");
		bytePtr++;
	}
	Serial.println();

}
