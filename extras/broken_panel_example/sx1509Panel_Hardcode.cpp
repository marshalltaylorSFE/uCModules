#include <Wire.h>
#include "Arduino.h"
#include "sx1509Panel_Hardcode.h"
#include "panelComponents.h"

sx1509HardCode::sx1509HardCode( void )
{
	deviceAddress = 0;
	nullValue = 0;	
	for( int i = 0; i < 16; i++ )
	{
		//Fill that up!
		inputCachePointers[i] = &nullValue;
		outputCachePointers[i] = &nullValue;
	}
	portDir = 0;
	portPU = 0;
	
}

void sx1509HardCode::init( uint8_t addressInput )
{
	
	setAddress( addressInput );
	
	// Begin I2C
	Wire.begin();
	
	writeByte( REG_PULL_DOWN_A, 0x00 );
	writeByte( REG_PULL_DOWN_B, 0x00 );
	writeByte( REG_PULL_UP_A, portDir & ( uint16_t )0x00FF );
	writeByte( REG_PULL_UP_B, ( portDir & ( uint16_t )0xFF00 ) >> 8 );	
	writeByte( REG_DIR_A, portPU & ( uint16_t )0x00FF );
	writeByte( REG_DIR_B, ( portPU & ( uint16_t )0xFF00 ) >> 8 );	
	
}


// readByte(uint8_t registerAddress)
//	This function reads a single uint8_t located at the registerAddress register.
//	- deviceAddress should already be set by the constructor.
//	- Return value is the uint8_t read from registerAddress
//		- Currently returns 0 if communication has timed out
uint8_t sx1509HardCode::readByte(uint8_t registerAddress)
{
	uint8_t readValue;
	unsigned int timeout = RECEIVE_TIMEOUT_VALUE;
	Wire.beginTransmission(deviceAddress);
	Wire.write(registerAddress);
	Wire.endTransmission();
	Wire.requestFrom(deviceAddress, (uint8_t) 1);
	while ((Wire.available() < 1) && (timeout != 0))
	timeout--;
	
	if (timeout == 0)
	return 0;
	readValue = Wire.read();
	return readValue;
}

// readWord(uint8_t registerAddress)
//	This function will read a two-uint8_t word beginning at registerAddress
//	- A 16-bit unsigned int will be returned.
//		- The msb of the return value will contain the value read from registerAddress
//		- The lsb of the return value will contain the value read from registerAddress + 1
unsigned int sx1509HardCode::readWord(uint8_t registerAddress)
{
	unsigned int readValue;
	unsigned int msb, lsb;
	unsigned int timeout = RECEIVE_TIMEOUT_VALUE * 2;

	Wire.beginTransmission(deviceAddress);
	Wire.write(registerAddress);
	Wire.endTransmission();
	Wire.requestFrom(deviceAddress, (uint8_t) 2);

	while ((Wire.available() < 2) && (timeout != 0))
	timeout--;
	
	if (timeout == 0)
	return 0;
	
	msb = (Wire.read() & 0x00FF) << 8;
	lsb = (Wire.read() & 0x00FF);
	readValue = msb | lsb;

	return readValue;
}

// readBytes(uint8_t firstRegisterAddress, uint8_t * destination, uint8_t length)
//	This function reads a series of uint8_ts incrementing from a given address
//	- firstRegsiterAddress is the first address to be read
//	- destination is an array of uint8_ts where the read values will be stored into
//	- length is the number of uint8_ts to be read
//	- No return value.
void sx1509HardCode::readBytes(uint8_t firstRegisterAddress, uint8_t * destination, uint8_t length)
{
	uint8_t readValue;

	Wire.beginTransmission(deviceAddress);
	Wire.write(firstRegisterAddress);
	Wire.endTransmission();
	Wire.requestFrom(deviceAddress, length);
	
	while (Wire.available() < length)
	;
	
	for (int i=0; i<length; i++)
	{
		destination[i] = Wire.read();
	}
}

// writeByte(uint8_t registerAddress, uint8_t writeValue)
//	This function writes a single uint8_t to a single register on the SX509.
//	- writeValue is written to registerAddress
//	- deviceAddres should already be set from the constructor
//	- No return value.
void sx1509HardCode::writeByte(uint8_t registerAddress, uint8_t writeValue)
{
	Wire.beginTransmission(deviceAddress);
	Wire.write(registerAddress);
	Wire.write(writeValue);
	Wire.endTransmission();
}

// writeWord(uint8_t registerAddress, ungisnged int writeValue)
//	This function writes a two-uint8_t word to registerAddress and registerAddress + 1
//	- the upper uint8_t of writeValue is written to registerAddress
//		- the lower uint8_t of writeValue is written to registerAddress + 1
//	- No return value.
void sx1509HardCode::writeWord(uint8_t registerAddress, unsigned int writeValue)
{
	uint8_t msb, lsb;
	msb = ((writeValue & 0xFF00) >> 8);
	lsb = (writeValue & 0x00FF);
	Wire.beginTransmission(deviceAddress);
	Wire.write(registerAddress);
	Wire.write(msb);
	Wire.write(lsb);
	Wire.endTransmission();	
}

// writeBytes(uint8_t firstRegisterAddress, uint8_t * writeArray, uint8_t length)
//	This function writes an array of uint8_ts, beggining at a specific adddress
//	- firstRegisterAddress is the initial register to be written.
//		- All writes following will be at incremental register addresses.
//	- writeArray should be an array of uint8_t values to be written.
//	- length should be the number of uint8_ts to be written.
//	- no return value.
void sx1509HardCode::writeBytes(uint8_t firstRegisterAddress, uint8_t * writeArray, uint8_t length)
{
	Wire.beginTransmission(deviceAddress);
	Wire.write(firstRegisterAddress);
	for (int i=0; i<length; i++)
	{
		Wire.write(writeArray[i]);
	}
	Wire.endTransmission();
}

void sx1509HardCode::setAddress( uint8_t inputAddress )
{
	deviceAddress = inputAddress;
	
}

void sx1509HardCode::update( void )
{
	//uint8_t * tempPointer = inputCachePointers[0];
	//Read Input States
	uint16_t tempData = 0;
	Serial.println("Did we get this far?");
	tempData = readByte( REG_DATA_A );
	tempData |= ( (uint16_t)readByte( REG_DATA_B ) << 8 );
	for( int i = 0; i < 16; i++ )  //Save all the input data
	{
		//*tempPointer = (uint8_t)(tempData & 0x0001);
		*inputCachePointers[i] = (tempData & 0x0001);
		tempData = tempData >> 1;  
		//tempPointer++;

	}
	
	//Write Output States
	tempData = 0;
	uint16_t outputValue;
	for( int i = 0; i < 16; i++ )  //get the output
	{
		switch(*outputCachePointers[i])
		{
		case 0:
			outputValue = 0;
			break;
		default:
		case 1:
			outputValue = 1;
			break;
		}		
		tempData = tempData | ( outputValue << i );
		Serial.println(*outputCachePointers[i]);

	}	
	writeByte( REG_DATA_A, tempData & ( uint16_t )0x00FF );
	writeByte( REG_DATA_B, ( tempData & ( uint16_t )0xFF00 ) >> 8 );

}

void sx1509HardCode::pinMode( uint8_t pin, uint8_t mode, PanelButton * objectIn )
{
	//Point output to object cache
	inputCachePointers[pin] = &objectIn->cache;
	
	//Work on the config words
	switch( mode )
	{
	case OUTPUT:
		//Clear bit for output
		portDir &= ~( 0x0001 << pin );
		break;
	case INPUT_PULLUP:
		//Set bit for input
		portDir |= ( 0x0001 << pin );
		//Enable pullup
		portPU |= ( 0x0001 << pin );
		break;
	default:
	case INPUT:
		//Set bit for input
		portDir |= ( 0x0001 << pin );
		//Disable pullup
		portPU &= ~( 0x0001 << pin );
		break;
	}
	Serial.println( portDir, HEX );
}

void sx1509HardCode::pinMode( uint8_t pin, uint8_t mode, PanelLed * objectOut )
{
	//Point output to object cache
	outputCachePointers[pin] = &objectOut->cache;
	
	//Work on the config words
	switch( mode )
	{
	case OUTPUT:
		//Clear bit for output
		portDir &= ~( 0x0001 << pin );
		break;
	case INPUT_PULLUP:
		//Set bit for input
		portDir |= ( 0x0001 << pin );
		//Enable pullup
		portPU |= ( 0x0001 << pin );
		break;
	default:
	case INPUT:
		//Set bit for input
		portDir |= ( 0x0001 << pin );
		//Enable pullup
		portPU &= ~( 0x0001 << pin );
		break;
	}
	Serial.println( portDir, HEX );
}
