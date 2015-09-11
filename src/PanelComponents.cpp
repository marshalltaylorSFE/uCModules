#include "PanelComponents.h"
//#include "timeKeeper.h"
#include "Arduino.h"

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

void PanelButton::init( uint8_t pinInput )
{
	init( pinInput, 0 );
}

void PanelButton::init( uint8_t pinInput, uint8_t bankInput )
{
	bank = bankInput;
	pinNumber = pinInput;
	if( bank == 0 )
	{
		pinMode( pinNumber, INPUT_PULLUP );
		update();
		//force newData high in case knob starts on last value
		newData = 1;
	}
	else
	{
		//Do bank related initialization here
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
		tempState = cache ^ 0x01;  //Take externally provided data
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
	bank = 0;
}

void PanelLed::init( uint8_t pinInput )
{
	init( pinInput, 0 );
}

void PanelLed::init( uint8_t pinInput, uint8_t bankInput )
{
	bank = bankInput;
	pinNumber = pinInput;
	if( bank == 0 )
	{
		pinMode( pinNumber, OUTPUT );
		update();
	}
	else
	{
		//Do bank related initialization here
	}

}

void PanelLed::init( uint8_t pinInput, uint8_t bankInput, volatile uint8_t * volatile flasherAddress, volatile uint8_t * volatile fastFlasherAddress )
{
	init( pinInput, bankInput ); //Do regular init, plus:
	
	flasherState = flasherAddress;
	fastFlasherState = fastFlasherAddress;
	
}

void PanelLed::update( void )
{
	uint8_t outputValue = 0;
	switch(state)
	{
	case LEDON:
		outputValue = 0;
		break;
	case LEDFLASHING:
		outputValue = *flasherState;
		break;
	case LEDFLASHINGFAST:
		outputValue = *fastFlasherState;
		break;
	default:
	case LEDOFF:
		outputValue = 1;
		break;
	} 

	//Serial.print("A:");
	//Serial.print(*flasherState);
	if( bank == 0 )
	{
		digitalWrite( pinNumber, outputValue );
		//Serial.print("B:");
		//Serial.print(*flasherState);
	}
	if( bank != 0 )
	{
		//Serial.print("C:");
		//Serial.print(*flasherState);
		cache = outputValue & 0x0001;
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

void PanelLed::setBank( uint8_t inputBank )
{
	bank = inputBank;

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