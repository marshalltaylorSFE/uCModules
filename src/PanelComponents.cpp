#include "PanelComponents.h"
//#include "timeKeeper.h"
#include "Arduino.h"
extern uint8_t ledToggleState;
extern uint8_t ledToggleFastState;

//---Switch------------------------------------------------------
PanelSwitch::PanelSwitch( void )
{

}

void PanelSwitch::init( uint8_t pinNum )
{
  pinNumber = pinNum;
  pinMode( pinNumber, INPUT_PULLUP );
  update();
  //force newData high in case knob starts on last value
  newData = 1;
}

void PanelSwitch::update( void )
{
  uint8_t tempState = digitalRead( pinNumber ) ^ 0x01;
  if( state != tempState )
  {
    state = tempState;
    newData = 1;
  }
}

uint8_t PanelSwitch::getState( void )
{
  newData = 0;

  return state;
}

//---Button------------------------------------------------------
PanelButton::PanelButton( void )
{

}

void PanelButton::init( uint8_t pinNum )
{
  pinNumber = pinNum;
  pinMode( pinNumber, INPUT_PULLUP );
  update();
  //force newData high in case knob starts on last value
  newData = 1;
}

void PanelButton::update( void )
{
  uint8_t tempState = digitalRead( pinNumber ) ^ 0x01;
  if(( state != tempState ) && (buttonDebounceTimeKeeper.mGet() > 50))
  {
	//  Serial.println(buttonDebounceTimeKeeper.mGet());
    state = tempState;
    newData = 1;
	//Start the timer
	buttonDebounceTimeKeeper.mClear();
  }
}

uint8_t PanelButton::getState( void )
{
  newData = 0;

  return state;
}
//---Led---------------------------------------------------------
PanelLed::PanelLed( void )
{
  state = LEDOFF;
  
}

void PanelLed::init( uint8_t pinNum )
{
  pinNumber = pinNum;
  pinMode( pinNumber, OUTPUT );
  update();

}

void PanelLed::update( void )
{
	switch(state)
	{
		case LEDON:
		digitalWrite( pinNumber, 0 );
		break;
		case LEDFLASHING:
		digitalWrite( pinNumber, ledToggleState);
		break;
		case LEDFLASHINGFAST:
		digitalWrite( pinNumber, ledToggleFastState);
		break;
		default:
		case LEDOFF:
		digitalWrite( pinNumber, 1 );
		break;
	}

}

ledState_t PanelLed::getState( void )
{
  return state;
  
}

void PanelLed::setState( ledState_t inputValue )
{
  state = inputValue;
  
}

//---Selector----------------------------------------------------
PanelSelector::PanelSelector( void )
{

}

// 8 bit resolution on the ADC should be fine.  Right shift on analogRead
void PanelSelector::init( uint8_t pinNum, uint8_t maxInput, uint8_t minInput )
{
  pinNumber = pinNum;
  pinMode( pinNumber, INPUT );
  //Set up the ranges
  uint8_t stepHeight = ( maxInput - minInput ) / 9;
  thresholds[0] = minInput + ( stepHeight / 2 );
  int i;
  for( i = 1; i < 9; i++ )
  {
	thresholds[i] = thresholds[i - 1] + stepHeight;
  }
  update();
  //force newData high in case knob starts on last value
  newData = 1;
}

void PanelSelector::update( void )
{
  uint8_t analogReadRaw = (analogRead( pinNumber )) >> 2;  //Now 8 bits
  uint8_t tempState = 0;
  //Seek the position
  int i;
  for( i = 0; i < 9; i++ )
  {
	if( analogReadRaw > thresholds[i] )
	{
		tempState = i + 1; //It's this or higher
	}
  }
  //Check if new
  if( state != tempState )
  {
    state = tempState;
    newData = 1;
  }
}

uint8_t PanelSelector::getState( void )
{
  newData = 0;

  return state;
}