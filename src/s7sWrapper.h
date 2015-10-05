//Basic class that operates like the sparkfun hookup I2C example

class S7sObject
{
public:
	S7sObject( uint8_t );
	void SendString( String );
	void clear( void );
	void setBrightness( uint8_t );
	void setDecimals( uint8_t );
private:
	uint8_t i2cAddress;
};
