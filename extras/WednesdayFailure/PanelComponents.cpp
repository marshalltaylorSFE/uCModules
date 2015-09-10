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
	beingHeld = 0;
	bank = 0;
}

void PanelButton::init( uint8_t pinNum )
{
	pinNumber = pinNum;
	if( bank == 0 )
	{
		pinMode( pinNumber, INPUT_PULLUP );
		update();
		//force newData high in case knob starts on last value
		newData = 1;
	}
}

//This is the intended operation:
// Button is pressed. If it has been long enough since last movement:
//   update newData
//   clear timer
// If This has already been done, check for button held.  If so,
//   update newData
//   clear timer
void PanelButton::update( void )
{
	uint8_t tempState;
	if( bank == 0 )
	{
		tempState = digitalRead( pinNumber ) ^ 0x01;
	}
	else
	{
		tempState = cache;  //Take externally provided data
	}
	
	switch( state )
	{
	case 0: //Last state was 0
	case 1: //Last state was 1
		if(( state != tempState ) && (buttonDebounceTimeKeeper.mGet() > 50))
		{
			//  Serial.println(buttonDebounceTimeKeeper.mGet());
			state = tempState;
			newData = 1;
			//Start the timer
			buttonDebounceTimeKeeper.mClear();
			if( tempState == 1 )
			{
				//Ok, we are being held down
				beingHeld = 1;
			}
		}
		if(( tempState == 1 )&&( beingHeld == 1 ))
		{
			//We're being held
			if(buttonDebounceTimeKeeper.mGet() > 1000)
			{
			newData = 1;
			state = 2;//BUTTONHOLD;
			//Serial.println("Got it.");
			}
			else
			{
				//Serial.println("WAITING!!!");
			}
		}
		break;
	case 2: //In the process of holding
		if(( tempState == 0) && ( state != tempState ) && (buttonDebounceTimeKeeper.mGet() > 50))
		{
		  //  Serial.println(buttonDebounceTimeKeeper.mGet());
		  state = tempState;
		  newData = 1;
		//Start the timer
		buttonDebounceTimeKeeper.mClear();
		beingHeld = 0;
		}
		break;
	default:
		break;
	}

}

uint8_t PanelButton::getState( void )
{
  newData = 0;

  return state;
}

void PanelButton::setBank( uint8_t newBank )
{
  bank = newBank;

}

//---Led---------------------------------------------------------
PanelLed::PanelLed( void )
{
  state = LEDOFF;
  
}

void PanelLed::init( uint8_t pinNum )
{
	pinNumber = pinNum;
	if( bank == 0 )
	{
		pinMode( pinNumber, OUTPUT );
		update();
	}

}

void PanelLed::update( void )
{
	if( bank == 0 )
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
	else
	{
		cache = state;
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

void PanelLed::setBank( uint8_t newBank )
{
  bank = newBank;

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