#include <Wire.h>  // Wire.h library is required to use SX1509 lib
//#include "sx1509Panel_registers.h"
#include "sx1509Panel_library.h"  // Include the SX1509 library
#include "Panel.h"

//This is where PanelComponents are joined to form the custom panel
#define tapButtonPin 4
#define syncButtonPin 5

#define playButtonPin 2
#define stopButtonPin 3
#define recordLedPin A1
#define playLedPin A2
#define overdubLedPin A3
#define tapLedPin A8
#define syncLedPin A9
#define quantizeSelectorPin A0

#define option1ButtonPin 3
#define option2ButtonPin 2
#define option3ButtonPin 1
#define option4ButtonPin 0
#define songUpButtonPin 7
#define songDownButtonPin 6
#define trackUpButtonPin 5
#define trackDownButtonPin 4

#define option1LedPin 8
#define option2LedPin 9
#define option3LedPin 10
#define option4LedPin 11


Panel::Panel( void )
{

}

void Panel::init( void )
{
  tapButton.init(tapButtonPin);
  syncButton.init(syncButtonPin);
  playButton.init(playButtonPin);
  stopButton.init(stopButtonPin);
  recordLed.init(recordLedPin);
  playLed.init(playLedPin);
  overdubLed.init(overdubLedPin);
  tapLed.init(tapLedPin);
  syncLed.init(syncLedPin);
  quantizeSelector.init(quantizeSelectorPin, 255, 0 ); //With max and min ranges

  //set bank before init
  songUpButton.setBank(1);
  songUpButton.init(songUpButtonPin);
  songDownButton.setBank(1);
  songDownButton.init(songDownButtonPin);
  trackUpButton.setBank(1);
  trackUpButton.init(trackUpButtonPin);
  trackDownButton.setBank(1);
  trackDownButton.init(trackDownButtonPin);
  option1Button.setBank(1);
  option1Button.init(option1ButtonPin);
  option2Button.setBank(1);
  option2Button.init(option2ButtonPin);
  option3Button.setBank(1);
  option3Button.init(option3ButtonPin);
  option4Button.setBank(1);
  option4Button.init(option4ButtonPin);
  option1Led.setBank(1);
  option1Led.init(option1LedPin);
  option2Led.setBank(1);
  option2Led.init(option2LedPin);
  option3Led.setBank(1);
  option3Led.init(option3LedPin);
  option4Led.setBank(1);
  option4Led.init(option4LedPin);
  expanderA.init( 0x3E );
  Serial.println("finished INIT");
  
}

void Panel::update( void )
{
  tapButton.update();
  syncButton.update();
  playButton.update();
  stopButton.update();
  recordLed.update();
  playLed.update();
  overdubLed.update();
  tapLed.update();
  syncLed.update();
  quantizeSelector.update();

  expanderA.update(); // to read
  songUpButton.update();
  songDownButton.update();
  trackUpButton.update();
  trackDownButton.update();
  option1Button.update();
  option2Button.update();
  option3Button.update();
  option4Button.update();
  option1Led.update();
  option2Led.update();
  option3Led.update();
  option4Led.update();
  expanderA.update(); // and to set LEDs

}

void Panel::timersMIncrement( uint8_t inputValue )
{
  tapButton.buttonDebounceTimeKeeper.mIncrement(inputValue);
  syncButton.buttonDebounceTimeKeeper.mIncrement(inputValue);
  songUpButton.buttonDebounceTimeKeeper.mIncrement(inputValue);
  songDownButton.buttonDebounceTimeKeeper.mIncrement(inputValue);
  trackUpButton.buttonDebounceTimeKeeper.mIncrement(inputValue);
  trackDownButton.buttonDebounceTimeKeeper.mIncrement(inputValue);
  playButton.buttonDebounceTimeKeeper.mIncrement(inputValue);
  stopButton.buttonDebounceTimeKeeper.mIncrement(inputValue);

}


SX1509Panel::SX1509Panel( void ) //Pass address
{
	address = 0;
	nullValue = 0;	
	for( int i = 0; i < 16; i++ )
	{
		//Fill that up!
		inputCachePointers[0] = &nullValue;
		outputCachePointers[0] = &nullValue;
	}
	portDir = 0;
	portPU = 0;

	
}

void SX1509Panel::update( void )
{
	//Read Input States
	uint16_t tempData = 0;
	tempData = expanderClass.readByte( REG_DATA_A );
	tempData |= ( (uint16_t)expanderClass.readByte( REG_DATA_B ) << 8 );
	for( int i = 0; i < 16; i++ )  //Save all the input data
	{
		*inputCachePointers[i] = tempData & 0x0001;
		tempData = tempData >> 1;  

	}
	
	//Write Output States
	tempData = 0;
	uint8_t tempState;
	for( int i = 0; i < 16; i++ )  //Save all the input data
	{
		switch(*outputCachePointers[i])
		{
			case LEDON:
			tempState = 0;
			break;
			case LEDFLASHING:
			case LEDFLASHINGFAST:
		default:
			case LEDOFF:
			tempState = 1;
			break;
		}		
		tempData = tempData & ( tempState << i );

	}	
	expanderClass.writeByte( REG_DATA_A, tempData & ( uint16_t )0x00FF );
	expanderClass.writeByte( REG_DATA_B, ( tempData & ( uint16_t )0xFF00 ) >> 8 );	

}

void SX1509Panel::pinMode( uint8_t pin, uint8_t mode, PanelButton * objectIn )
{
	//Point output to object cache
	inputCachePointers[pin] = &objectIn->cache;
	
	//Work on the config words
	switch( mode )
	{
	case OUTPUT:
		//Clear bit for output
		portDir &= ~( 0x01 << pin );
		break;
	case INPUT_PULLUP:
		//Set bit for input
		portDir |= ( 0x01 << pin );
		//Enable pullup
		portPU |= ( 0x01 << pin );
		break;
	default:
	case INPUT:
		//Set bit for input
		portDir |= ( 0x01 << pin );
		//Enable pullup
		portPU &= ~( 0x01 << pin );
		break;
	}
}

void SX1509Panel::pinMode( uint8_t pin, uint8_t mode, PanelLed * objectOut )
{
	//Point output to object cache
	outputCachePointers[pin] = &objectOut->cache;
	
	//Work on the config words
	switch( mode )
	{
	case OUTPUT:
		//Clear bit for output
		portDir &= ~( 0x01 << pin );
		break;
	case INPUT_PULLUP:
		//Set bit for input
		portDir |= ( 0x01 << pin );
		//Enable pullup
		portPU |= ( 0x01 << pin );
		break;
	default:
	case INPUT:
		//Set bit for input
		portDir |= ( 0x01 << pin );
		//Enable pullup
		portPU &= ~( 0x01 << pin );
		break;
	}
}

void SX1509Panel::init( uint8_t addressIn )
{
	expanderClass.setAddress( addressIn );
	expanderClass.init();
	expanderClass.writeByte( REG_PULL_UP_A, portDir & ( uint16_t )0x00FF );
	expanderClass.writeByte( REG_PULL_UP_B, ( portDir & ( uint16_t )0xFF00 ) >> 8 );	
	expanderClass.writeByte( REG_PULL_DOWN_A, 0x00 );
	expanderClass.writeByte( REG_PULL_DOWN_B, 0x00 );
	expanderClass.writeByte( REG_PULL_UP_A, portPU & ( uint16_t )0x00FF );
	expanderClass.writeByte( REG_PULL_UP_B, ( portPU & ( uint16_t )0xFF00 ) >> 8 );	
}