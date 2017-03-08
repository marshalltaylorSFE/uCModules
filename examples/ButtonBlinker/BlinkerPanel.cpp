//********************************************//
#include "BlinkerPanel.h"
#include "panelComponents.h"
#include "HardwareInterfaces.h"
#include <Arduino.h>

BlinkerPanel::BlinkerPanel( void )
{
	myButton.setHardware(new ArduinoDigitalIn( 3 ), 1);
	add( &myButton );

	myLed.setHardware(new ArduinoDigitalOut( 2 ), 0);
	add( &myLed );
	
	state = PInit;
	held = 0;
}

void BlinkerPanel::reset( void )
{
	//Set explicit states
	//Set all LED off
	myLed.setState(LEDOFF);
	state = PInit;
	held = 0;
	
}

void BlinkerPanel::tickStateMachine( int msTicksDelta )
{
	freshenComponents( msTicksDelta );
	
	//***** PROCESS THE LOGIC *****//
	//Now do the states.
	PStates nextState = state;
	switch( state )
	{
	case PInit:
		//Can't be running, if button pressed move on
		if( myButton.serviceRisingEdge() )
		{
			nextState = POn;
			myLed.setState(LEDON);
		}
		break;
	case POn:
		//Can't be running, if button pressed move on
		if( myButton.serviceRisingEdge() )
		{
			nextState = PFlash;
			myLed.setState(LEDFLASHING);
		}
		break;
	case PFlash:
		//Can't be running, if button pressed move on
		if( myButton.serviceHoldRisingEdge() )
		{
			myLed.setState(LEDFLASHINGFAST);
			held = 1;
		}
		if( myButton.serviceFallingEdge() && (held == 1))
		{
			myButton.serviceRisingEdge();
			nextState = PInit;
			held = 0;
			myLed.setState(LEDOFF);
		}
		break;		
	default:
		nextState = PInit;
		break;
	}
	state = nextState;

}
