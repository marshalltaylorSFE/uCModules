#include "stdint.h"
#include "PanelComponents.h"
#include "HardwareInterfaces.h"
#include "Arduino.h"

//---Button------------------------------------------------------
void Button::setHardware( GenericHardwareDescription * input, uint8_t invertInput )
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
	*tempObject.data ^= invert;  //Do the inversion here
	
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
