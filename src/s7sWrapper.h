#ifndef S7SWRAPPER_H_INCLUDED
#define S7SWRAPPER_H_INCLUDED

//Basic class that operates like the sparkfun hookup I2C example
#include <stdint.h>
#include <Arduino.h>

class S7sObject
{
public:
	S7sObject( void );
	void SendString( String );
	void clear( void );
	void setBrightness( uint8_t );
	void setDecimals( uint8_t );
	uint8_t i2cAddress;
};

#endif
