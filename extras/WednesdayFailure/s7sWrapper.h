//Basic class that operates like the sparkfun hookup I2C example

class S7sObject
{
public:
	S7sObject( byte );
	void SendString( String );
	void clear( void );
	void setBrightness( byte );
	void setDecimals( byte );
private:
	byte i2cAddress;
};
