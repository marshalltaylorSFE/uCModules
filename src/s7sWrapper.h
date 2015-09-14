//Basic class that operates like the sparkfun hookup I2C example
#ifndef S7SWRAPPER_H_INCLUDED
#define S7SWRAPPER_H_INCLUDED

class S7sObject
{
public:
	S7sObject( void );
	void SendString( String );
	void clear( void );
	void setBrightness( byte );
	void setDecimals( byte );
	byte i2cAddress;
};

#endif