//**********************************************************************//
//
//  MIT License: http://opensource.org/licenses/MIT
//
//  Written by:  Marshall Taylor
//  Changelog (YYYY/MM/DD):
//    2015/10/08: Beta Release
//    2016/05/08: Fixed button falling edge detection and added LED invertion
//
//**********************************************************************//
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
}

void PanelSwitch::update( void )
{
	uint8_t tempState = digitalRead( pinNumber ) ^ 0x01;
	tempState ^= invert;
	if(( state.getFlag() != tempState ) && ( SwitchDebounceTimeKeeper.mGet() > 200 ))
	{
		if( tempState == 1 )
		{
			state.setFlag();
		}
		else
		{
			state.clearFlag();
		}
		SwitchDebounceTimeKeeper.mClear();
	}
}

//---Knob--------------------------------------------------------
PanelKnob8Bit::PanelKnob8Bit( void )
{

}

void PanelKnob8Bit::init( uint8_t pinNum )
{
  pinNumber = pinNum;
  pinMode( pinNumber, INPUT );
  update();
  //force newData high in case knob starts on last value
  newData = 1;
}

void PanelKnob8Bit::update( void )
{
  uint8_t tempState = analogRead( pinNumber ) >> 2;
  if( tempState < 2 )
  {
    tempState = 1;
  }
  if( ( state > (tempState + 2) ) || ( state < (tempState - 2) ))
  {
    state = tempState;
    newData = 1;
  }
}

uint8_t PanelKnob8Bit::getState( void )
{
  newData = 0;

  return state;
}

//---Register----------------------------------------------------
PanelRegister::PanelRegister( void )
{

}

void PanelRegister::init( uint8_t pinNumBit2, uint8_t pinNumBit1, uint8_t pinNumBit0 )
{
  length = 3;
  pinMap[2] = pinNumBit2;
  pinMap[1] = pinNumBit1;
  pinMap[0] = pinNumBit0;

  pinMode( pinNumBit2, INPUT_PULLUP );
  pinMode( pinNumBit1, INPUT_PULLUP );
  pinMode( pinNumBit0, INPUT_PULLUP );

  update();
  //force newData high in case knob starts on last value
  newData = 1;
}

void PanelRegister::update( void )
{
  uint8_t tempState;
  tempState = digitalRead( pinMap[2] ) << 2;
  tempState |= digitalRead( pinMap[1] ) << 1;
  tempState |= digitalRead( pinMap[0] );

  //Invert
  tempState ^= 0x07;

  if( state != tempState )
  {
    state = tempState;
    newData = 1;
  }
}

uint8_t PanelRegister::getState( void )
{
  newData = 0;

  return state;
}

//---Button------------------------------------------------------
PanelButton::PanelButton( void )
{
	beingHeld = 0;
	bank = 0;
	risingEdgeFlag = 0;
	fallingEdgeFlag = 0;
	holdRisingEdgeFlag = 0;
	holdFallingEdgeFlag = 0; 
	
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
	uint8_t freshData;
	if( bank == 0 )
	{
		freshData = digitalRead( pinNumber ) ^ 0x01;
	}
	else
	{
		freshData = cache ^ 0x01;  //Take externally provided data
	}
	
	buttonState_t nextState = state;
	switch( state )
	{
	case BUTTONOFF: //Last state was BUTTONOFF
		if(( freshData == 1 ) && ( buttonDebounceTimeKeeper.mGet() > 50 ))
		{
			//Start the timer
			buttonDebounceTimeKeeper.mClear();
			nextState = BUTTONON;
			risingEdgeFlag = 1;
		}
		else
		{
			nextState = BUTTONOFF;
		}
		break;
	case BUTTONON: //Last state was BUTTONON
		if( freshData == 1 )
		{
			if( buttonDebounceTimeKeeper.mGet() > 1000 )
			{
				nextState = BUTTONHOLD;
				holdRisingEdgeFlag = 1;
			}
		}
		//No break;, do buttonhold's state too
	case BUTTONHOLD: //In the process of holding
		if( freshData == 0 )
		{
			buttonDebounceTimeKeeper.mClear();
			nextState = BUTTONOFF;
			fallingEdgeFlag = 1;
		}
		break;
	default:
		break;
	}
	
	state = nextState;

}

buttonState_t PanelButton::getState( void )
{
	return state;
}

uint8_t PanelButton::serviceRisingEdge( void )
{
	uint8_t returnVar = 0;
	if( risingEdgeFlag == 1 )
	{
		risingEdgeFlag = 0;
		returnVar = 1;
	}
	
	return returnVar;
}

uint8_t PanelButton::serviceFallingEdge( void )
{
	uint8_t returnVar = 0;
	if( fallingEdgeFlag == 1 )
	{
		fallingEdgeFlag = 0;
		returnVar = 1;
	}
	
	return returnVar;
}

uint8_t PanelButton::serviceHoldRisingEdge( void )
{
	uint8_t returnVar = 0;
	if( holdRisingEdgeFlag == 1 )
	{
		holdRisingEdgeFlag = 0;
		returnVar = 1;
	}
	
	return returnVar;
}

uint8_t PanelButton::serviceHoldFallingEdge( void )
{
	uint8_t returnVar = 0;
	if( holdFallingEdgeFlag == 1 )
	{
		holdFallingEdgeFlag = 0;
		returnVar = 1;
	}
	
	return returnVar;
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
	invert = 0;
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
	//Serial.println((uint32_t)flasherState, HEX);
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
		digitalWrite( pinNumber, ( outputValue ^ invert ) );
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


void PanelLed::outputInvert( uint8_t invertVar )
{
	invert = invertVar;
	
}

//---Selector----------------------------------------------------
PanelSelector::PanelSelector( void )
{
	changedFlag = 0;
	points = 10;
}

PanelSelector::~PanelSelector( void )
{
	delete[] thresholds;
}
// 8 bit resolution on the ADC should be fine.  Right shift on analogRead
void PanelSelector::init( uint8_t pinNum, uint8_t maxInput, uint8_t minInput, uint8_t pointsInput )
{
	if( pointsInput < 2 )
	{
		points = 2;
	}
	else
	{
		points = pointsInput - 1; //( by (n-1) not n )
	}
	thresholds = new uint8_t[points];

	pinNumber = pinNum;
	pinMode( pinNumber, INPUT );
	//Set up the ranges
	uint8_t stepHeight = ( maxInput - minInput ) / points;
	thresholds[0] = minInput + ( stepHeight / 2 );
	int i;
	for( i = 1; i < points; i++ )
	{
		thresholds[i] = thresholds[i - 1] + stepHeight;
	
	}
	update();
	//force changedFlag high in case knob starts on last value
	//changedFlag = 1;
}

void PanelSelector::init( uint8_t pinNum, uint8_t maxInput, uint8_t minInput )
{
	init( pinNum, maxInput, minInput, 10 );
}

void PanelSelector::update( void )
{
	uint8_t freshData = (analogRead( pinNumber )) >> 2;  //Now 8 bits
	uint8_t tempState = 0;
	//Seek the position
	int i;
	for( i = 0; i < points; i++ )
	{
		if( freshData > thresholds[i] )
		{
			tempState = i + 1; //It's this or higher
		}
	}
	//Check if new
	if( state != tempState )
	{
		state = tempState;
		changedFlag = 1;
	}
}

uint8_t PanelSelector::getState( void )
{
	return state;
}

uint8_t PanelSelector::serviceChanged( void )
{
	uint8_t returnVar = 0;
	if( changedFlag == 1 )
	{
		changedFlag = 0;
		returnVar = 1;
	}
	
	return returnVar;
}

//---Seven Segment Display---------------------------------------
sSDisplay::sSDisplay( void )
{
	state = SSOFF;
	updateDisplayFlag = 0;
	lastFlasherState = 0;
	peekThroughFlag = 0;
	peekingFlag = 0;
	peekThroughTime = 1000;
}

void sSDisplay::init( uint8_t addressInput )
{
	i2cAddress = addressInput;
	clear();  // Clears display, resets cursor
	setBrightness(255);  // High brightness
	//sprintf(tempString, "%4d", (unsigned int)8888);
	SendString("    ");
	peekThroughFlag = 0;
	peekingFlag = 0;

}

void sSDisplay::init( uint8_t addressInput, volatile uint8_t * volatile flasherAddress, volatile uint8_t * volatile fastFlasherAddress )
{
	flasherState = flasherAddress;
	fastFlasherState = fastFlasherAddress;
	
	init( addressInput ); //Do regular init, plus
}


void sSDisplay::update( void )
{
	uint8_t outputValue = 0;



	//Serial.println( state, HEX );
	switch(state)
	{
	case SSOFF:
		outputValue = 0;
		break;
	case SSFLASHING:
		outputValue = *flasherState;
		if( outputValue != lastFlasherState )
		{
			updateDisplayFlag = 1;
		}
		lastFlasherState = outputValue;
		break;
	case SSLASHINGFAST:
		outputValue = *fastFlasherState;
		if( outputValue != lastFlasherState )
		{
			updateDisplayFlag = 1;
		}
		lastFlasherState = outputValue;
		break;
	default:
	case SSON:
		outputValue = 1;
		break;
	}
	
	if( peekThroughFlag == 1 )
	{
		peekThroughFlag = 0;
		peekingFlag = 1;
		peekThroughTimeKeeper.mClear();
		Serial.print("CLEARED!");
	}
	if( peekingFlag == 1)
	{
		if( peekThroughTimeKeeper.mGet() > peekThroughTime )
		{
			peekingFlag = 0;
			updateDisplayFlag = 1;
		}
		else
		{
			updateDisplayFlag = 1;
			outputValue = 2;
			//Serial.print(peekThroughTimeKeeper.mGet());
		}
		Serial.print("state: ");
		Serial.println(state);
	}
	if( updateDisplayFlag == 1 )
	{

		if( outputValue == 0 )
		{
			clear();
		}
		if( outputValue == 1 )
		{
			SendString(data);
		}
		if( outputValue == 2 )
		{
			SendString(peekThroughData);
		}
		
		updateDisplayFlag = 0;
		//Serial.print("Thing");
		//Serial.println(outputValue);
	}
	

}

sSDisplayState_t sSDisplay::getState( void )
{
	return state;
}

void sSDisplay::setState( sSDisplayState_t inputValue )
{
	state = inputValue;

}

void sSDisplay::setData( String toSet )
{
	uint8_t differences = 0;
	for( int i = 0; i < 4; i++ )
	{
		if( toSet[i] != data[i] )
		{
			differences++;
		}
	}
	if( differences != 0 )
	{
		for( int i = 0; i < 4; i++ )
		{	
			data[i] = toSet[i];
		}
		data[4] = '\0';
		updateDisplayFlag = 1;
	}

	
}

void sSDisplay::peekThrough( String toSet, uint16_t timeFor )
{
	for( int i = 0; i < 4; i++ )
	{	
		peekThroughData[i] = toSet[i];
	}
	peekThroughData[4] = '\0';
	updateDisplayFlag = 1;
	peekThroughFlag = 1;
	peekingFlag = 0;
	peekThroughTime = timeFor;

	
}

