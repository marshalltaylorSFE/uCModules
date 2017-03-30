#include "stdint.h"
#include "PanelComponents.h"
#include "HardwareInterfaces.h"
#include "Arduino.h"
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
void Led::setHardware( GenericHardwareDescription * input, uint8_t invertInput )
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


//---Knob--------------------------------------------------------
Simple10BitKnob::Simple10BitKnob( void )
{
	newData = 0;
	lastState = 0;
}

void Simple10BitKnob::setHardware( GenericHardwareDescription * input )
{
	hardwareInterface = input;
}

bool Simple10BitKnob::hasFreshData( void )
{
	uint8_t returnVar = 0;
	if( newData )
	{
		returnVar = 1;
	}
	return returnVar;
}

void Simple10BitKnob::freshen( uint16_t msTickDelta )
{
	//Throw away input
	//Cause the interface to get the data
	hardwareInterface->readHardware();
	//Collect the data
	KnobDataObject tempObject;
//	KnobDataObject * tempObject = new KnobDataObject;
//		Serial.println("knobFreshen");
//		Serial.print("Temp ");
//		dumpObject(tempObject);
//Serial.print((uint32_t)&(*tempObject), HEX);
//Serial.println("Created");
	hardwareInterface->getData(&tempObject);
	
	uint16_t tempState = *(uint16_t *)tempObject.data;
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
		if( state > lastState + hysteresis || histDirTemp == 1)
		{
			newData = 1;
			lastState = state;
			lastSlope = tempSlope;
		}
		if( state < lastState - hysteresis || histDirTemp == -1 )
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

uint16_t Simple10BitKnob::getState( void )
{
  newData = 0;

  return state;
}

uint8_t Simple10BitKnob::serviceChanged( void )
{
	return newData;
}