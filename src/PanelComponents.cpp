#include "stdint.h"
#include "PanelComponents.h"
#include "HardwareInterfaces.h"
#include "Arduino.h"
#include "CircularBuffer.h"

extern void barfOutStackPointer();

uint16_t PanelComponent::flasherCounter = 0;
uint16_t PanelComponent::fastFlasherCounter = 0;
uint16_t PanelComponent::flasherCounterMax = 333;
uint16_t PanelComponent::fastFlasherCounterMax = 100;
uint8_t PanelComponent::flasherState = 1;
uint8_t PanelComponent::fastFlasherState = 1;

void PanelComponent::freshenStatic( uint16_t msTickDelta ){
	flasherCounter += msTickDelta;
	if( flasherCounter > flasherCounterMax )
	{
		flasherCounter -= flasherCounterMax;
		flasherState ^= 0x01;
	}
	fastFlasherCounter += msTickDelta;
	if( fastFlasherCounter > fastFlasherCounterMax )
	{
		fastFlasherCounter -= fastFlasherCounterMax;
		fastFlasherState ^= 0x01;
	}
};

//---Button------------------------------------------------------
void Button::setHardware( GenericHardwareDescription * input, bool invertInput )
{
	hardwareInterface = input;
	invert = invertInput;
	
	freshen( 1 );
	
	state = BUTTONOFF;

	risingEdgeFlag = 0;
	fallingEdgeFlag = 0;
	holdRisingEdgeFlag = 0;
	holdFallingEdgeFlag = 0;
	beingHeld = 0;

}

bool Button::hasFreshData( void )
{
	uint8_t returnVar = 0;
	if( risingEdgeFlag || fallingEdgeFlag || holdRisingEdgeFlag || holdFallingEdgeFlag )
	{
		returnVar = 1;
	}
	return returnVar;
}

void Button::freshen( uint16_t msTickDelta )
{
	//Cause the interface to get the data
	hardwareInterface->readHardware();
	
	//Collect the data
	ButtonDataObject tempObject;
	hardwareInterface->getData(&tempObject);
	//if( tempObject.size <= 0 )
	//{
	//	return;
	//}
	//else
	//{
	//	return *tempObject.data;
	//}
	*tempObject.data ^= (uint8_t)invert;  //Do the inversion here
	
	//Update any time related things
	buttonDebounceTimeKeeper.mIncrement( msTickDelta );
	
	//Process state machine
	buttonState_t nextState = state;
	switch( state )
	{
	case BUTTONOFF: //Last state was BUTTONOFF
		if(( *tempObject.data == 1 ) && ( buttonDebounceTimeKeeper.mGet() > 50 ))
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
		if( *tempObject.data == 1 )
		{
			if( buttonDebounceTimeKeeper.mGet() > 1000 )
			{
				nextState = BUTTONHOLD;
				holdRisingEdgeFlag = 1;
			}
		}
		//No break;, do buttonhold's state too
	case BUTTONHOLD: //In the process of holding
		if( *tempObject.data == 0 )
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

buttonState_t Button::getState( void )
{
	return state;
}

uint8_t Button::serviceRisingEdge( void )
{
	uint8_t returnVar = 0;
	if( risingEdgeFlag == 1 )
	{
		risingEdgeFlag = 0;
		returnVar = 1;
	}
	
	return returnVar;
}

uint8_t Button::serviceFallingEdge( void )
{
	uint8_t returnVar = 0;
	if( fallingEdgeFlag == 1 )
	{
		fallingEdgeFlag = 0;
		returnVar = 1;
	}
	
	return returnVar;
}

uint8_t Button::serviceHoldRisingEdge( void )
{
	uint8_t returnVar = 0;
	if( holdRisingEdgeFlag == 1 )
	{
		holdRisingEdgeFlag = 0;
		returnVar = 1;
	}
	
	return returnVar;
}

uint8_t Button::serviceHoldFallingEdge( void )
{
	uint8_t returnVar = 0;
	if( holdFallingEdgeFlag == 1 )
	{
		holdFallingEdgeFlag = 0;
		returnVar = 1;
	}
	
	return returnVar;
}

//---Led---------------------------------------------------------
void Led::setHardware( GenericHardwareDescription * input, bool invertInput )
{
	//flasherState = flasherAddress;
	//fastFlasherState = fastFlasherAddress;
	//flasherState = 1;
	//fastFlasherState = 1;
	hardwareInterface = input;
	invert = invertInput;
}

void Led::freshen( uint16_t msTickDelta )
{
	uint8_t outputValue = 0;
	switch(state)
	{
	case LEDON:
		outputValue = 1;
		break;
	case LEDFLASHING:
		outputValue = flasherState;
		break;
	case LEDFLASHINGFAST:
		outputValue = fastFlasherState;
		break;
	default:
	case LEDOFF:
		outputValue = 0;
		break;
	}
	
	//Give the output
	LedDataObject outputData;
	*outputData.data = outputValue ^ invert;
	hardwareInterface->setData(&outputData);
	//Cause the interface to send the data
	hardwareInterface->writeHardware();
	
}

ledState_t Led::getState( void )
{
	return state;

}

void Led::setState( ledState_t inputValue )
{
	state = inputValue;

}

void Led::toggle( void )
{
	switch(state)
	{
	case LEDON:
		state = LEDOFF;
		break;
	case LEDOFF:
		state = LEDON;
		break;
	default:
		break;
	}
}

//---KnobParentClass--------------------------------------------------------
KnobParentClass::KnobParentClass( void )
{
	newData = 0;
}

void KnobParentClass::setHardware( GenericHardwareDescription * input )
{
	hardwareInterface = input;
}

bool KnobParentClass::hasFreshData( void )
{
	if( newData )
	{
		return 1;
	}
	else
	{
		return 0;
	}
	
}

bool KnobParentClass::serviceChanged( void )
{
	if( newData )
	{
		newData = false;
		return true;
	}
	else
	{
		return false;
	}

}


//---Simple8BitKnob---------------------------------------------------------
void Simple8BitKnob::freshen( uint16_t msTickDelta )
{
	//Throw away input
	//Cause the interface to get the data
	hardwareInterface->readHardware();
	//Collect the data
	KnobDataObject tempObject;
	hardwareInterface->getData(&tempObject);
	
	state = *(uint16_t *)tempObject.data >> 2;
	if( state != lastState )
	{
		newData = 1;
		lastState = state;
	}

}

uint8_t Simple8BitKnob::getState( void )
{
   return state;
}

//---Simple10BitKnob--------------------------------------------------------
void Simple10BitKnob::freshen( uint16_t msTickDelta )
{
	//Throw away input
	hardwareInterface->readHardware();
	//Collect the data
	KnobDataObject tempObject;
//	KnobDataObject * tempObject = new KnobDataObject;
//		Serial.println("knobFreshen");
//		Serial.print("Temp ");
//		dumpObject(&tempObject);
//Serial.print((uint32_t)&(*tempObject), HEX);
//Serial.println("Created");
	hardwareInterface->getData(&tempObject);
	
	int16_t tempState = *(uint16_t *)tempObject.data;
	int8_t tempSlope = 0;
	state = tempState;
	int8_t histDirTemp = 0;
	if( state > lastState )
	{
		tempSlope = 1;
		if( lastSlope == 1 ) histDirTemp = 1;
	}
	else if( state < lastState )
	{
		tempSlope = -1;
		if( lastSlope == -1 ) histDirTemp = -1;
	}
	if( tempSlope != 0 )
	{
		if( state > (uint32_t)lastState + hysteresis || histDirTemp == 1)
		{
			newData = 1;
			lastState = state;
			lastSlope = tempSlope;
		}
		if( state < (uint32_t)lastState - hysteresis || histDirTemp == -1 )
		{
			newData = 1;
			lastState = state;
			lastSlope = tempSlope;
		}

	}
//Serial.print("Temp ");
//dumpObject(tempObject);
//Serial.print((uint32_t)&(*tempObject), HEX);
//Serial.println("Destroyed");
//	delete tempObject;

}

void Simple10BitKnob::setHysteresis( uint8_t input )
{
	hysteresis = input;
}

uint16_t Simple10BitKnob::getState( void )
{
  return state;
}

//---Complex10BitKnob--------------------------------------------
void Complex10BitKnob::freshen( uint16_t msTickDelta )
{
	//Throw away input
	//Cause the interface to get the data
	hardwareInterface->readHardware();
	//Collect the data
	KnobDataObject tempObject;
	hardwareInterface->getData(&tempObject);
	
//		Serial.println("knobFreshen");
//		Serial.print("Temp ");
//		dumpObject(&tempObject);

	uint16_t tempState = *(uint16_t *)tempObject.data;
	//Read the knob value into the buffer
	//Serial.println(tempState);

	currentValue = tempState;
}

uint16_t Complex10BitKnob::getState( void )
{
  return (uint16_t)currentValue;
}

uint8_t Complex10BitKnob::getAsUInt8( void )
{
	newData = 0;
	return currentValue >> 2;
}
int16_t Complex10BitKnob::getAsInt16( void )
{
	uint32_t tempValue = currentValue;
	if( tempValue < lowerKnobVal ) tempValue = lowerKnobVal;
	if( tempValue > upperKnobVal ) tempValue = upperKnobVal;
	// now find scalar from 0 to 1
	float inputPercent = ((float)(tempValue - lowerKnobVal))/(float)(upperKnobVal - lowerKnobVal);
	float outputRange = upperIntVal - lowerIntVal;
	newData = 0;
	return lowerIntVal + (inputPercent * outputRange);
}
uint16_t Complex10BitKnob::getAsUInt16( void )
{
	uint32_t tempValue = currentValue;
	if( tempValue < lowerKnobVal ) tempValue = lowerKnobVal;
	if( tempValue > upperKnobVal ) tempValue = upperKnobVal;
	// now find scalar from 0 to 1
	float inputPercent = ((float)(tempValue - lowerKnobVal))/(float)(upperKnobVal - lowerKnobVal);
	float outputRange = upperUIntVal - lowerUIntVal;
	newData = 0;
	return lowerUIntVal + (inputPercent * outputRange);
}
float Complex10BitKnob::getAsFloat( void )
{
	uint32_t tempValue = currentValue;
	if( tempValue < lowerKnobVal ) tempValue = lowerKnobVal;
	if( tempValue > upperKnobVal ) tempValue = upperKnobVal;
	// now find scalar from 0 to 1
	float inputPercent = ((float)(tempValue - lowerKnobVal))/(float)(upperKnobVal - lowerKnobVal);
	float outputRange = upperFloatVal - lowerFloatVal;
	newData = 0;
	return lowerFloatVal + (inputPercent * outputRange);
}

void Complex10BitKnob::setLowerKnobVal( uint16_t input )
{
	if( input < 0 )input = 0;
	if( input > 1023 )input = 1023;
	lowerKnobVal = input;
}

void Complex10BitKnob::setUpperKnobVal( uint16_t input )
{
	if( input < 0 )input = 0;
	if( input > 1023 )input = 1023;
	upperKnobVal = input;
}

void Complex10BitKnob::setLowerFloatVal( float input )
{
	lowerFloatVal = input;
}

void Complex10BitKnob::setUpperFloatVal( float input )
{
	upperFloatVal = input;
}

void Complex10BitKnob::setLowerIntVal( int16_t input )
{
	lowerIntVal = input;
}

void Complex10BitKnob::setUpperIntVal( int16_t input )
{
	upperIntVal = input;
}

void Complex10BitKnob::setLowerUIntVal( uint16_t input )
{
	lowerUIntVal = input;
}

void Complex10BitKnob::setUpperUIntVal( uint16_t input )
{
	upperUIntVal = input;
}

//---Hysteresis10BitKnob--------------------------------------------------------
Hysteresis10BitKnob::Hysteresis10BitKnob( void ) : values(16), averages(16)
{
}
void Hysteresis10BitKnob::freshen( uint16_t msTickDelta )
{
	//Throw away input
	//Cause the interface to get the data
	hardwareInterface->readHardware();
	//Collect the data
	KnobDataObject tempObject;
	hardwareInterface->getData(&tempObject);
	
//		Serial.println("knobFreshen");
//		Serial.print("Temp ");
//		dumpObject(&tempObject);

	uint16_t tempState = *(uint16_t *)tempObject.data;
	//Read the knob value into the buffer
	//Serial.println(tempState);
	values.write(tempState);
	int32_t accumulator = 0;
	for( int i = 0; i < samplesAveraged; i++ )
	{
		accumulator += values.read(i);
	}
	averages.write(accumulator/samplesAveraged);
	if(hystState == 0)
	{
		if( averages.read(0) > currentValue )
		{
			//Allow current to go up only
			currentValue = averages.read(0);
			newData = 1;
		}
		if( averages.read(0) < currentValue - hysteresis )
		{
			//toggle behavior
			hystState = 1;
			currentValue = averages.read(0);
			newData = 1;			
		}
	}
	else
	{
		if( averages.read(0) < currentValue )
		{
			//Allow current to go up only
			currentValue = averages.read(0);
			newData = 1;
		}
		if( averages.read(0) > currentValue + hysteresis )
		{
			//toggle behavior
			hystState = 0;
			currentValue = averages.read(0);
			newData = 1;			
		}
	}
	

}

void Hysteresis10BitKnob::setHysteresis( uint8_t input )
{
	hysteresis = input;
}

void Hysteresis10BitKnob::setSamplesAveraged( uint8_t input )
{
	samplesAveraged = input;
}

//---Windowed10BitKnob--------------------------------------------------------
Windowed10BitKnob::Windowed10BitKnob( void ) : values(16), averages(16)
{
}

void Windowed10BitKnob::freshen( uint16_t msTickDelta )
{
	//Throw away input
	//Cause the interface to get the data
	hardwareInterface->readHardware();
	//Collect the data
	KnobDataObject tempObject;
	hardwareInterface->getData(&tempObject);
	
//		Serial.println("knobFreshen");
//		Serial.print("Temp ");
//		dumpObject(&tempObject);

	uint16_t tempState = *(uint16_t *)tempObject.data;
	//Read the knob value into the buffer
	//Serial.println(tempState);
	values.write(tempState);
	int32_t accumulator = 0;
	for( int i = 0; i < samplesAveraged; i++ )
	{
		accumulator += values.read(i);
	}
	averages.write(accumulator/samplesAveraged);
	if( averages.read(0) > windowUpper )
	{
		moveWindowUpper( averages.read(0) );
		newData = 1;
	}
	if( averages.read(0) < windowLower )
	{
		moveWindowLower( averages.read(0) );
		newData = 1;
	}
	uint32_t temp = windowLower + (window >> 1);
	//scale it
	currentValue = ((temp - (window >> 1)) << 10 ) / (1024 - window );
}

void Windowed10BitKnob::setSamplesAveraged( uint8_t input )
{
	samplesAveraged = input;
}

void Windowed10BitKnob::setWindow( int16_t input )
{
	window = input;
}

void Windowed10BitKnob::moveWindowUpper( uint16_t input )
{
	windowUpper = input;
	windowLower = input - window;
}

void Windowed10BitKnob::moveWindowLower( uint16_t input )
{
	windowLower = input;
	windowUpper = input + window;
	
}
