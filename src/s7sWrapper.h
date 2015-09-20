//Basic class that operates like the sparkfun hookup I2C example
#ifndef S7SWRAPPER_H_INCLUDED
#define S7SWRAPPER_H_INCLUDED
#include <stdint.h>

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