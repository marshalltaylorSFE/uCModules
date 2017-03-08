//**SystemPanel*******************************//
#include "SystemPanel.h"
#include "panelComponents.h"
#include "HardwareInterfaces.h"
#include <Arduino.h>

SystemPanel::SystemPanel( void )
{
	upButton.setHardware(new ArduinoDigitalIn( 22 ), 1);
	add( &myButton );
	
	state = PInit;
	held = 0;
}

void SystemPanel::reset( void )
{
	//Set explicit states
	//Set all LED off
	myLed.setState(LEDOFF);
	state = PInit;
	held = 0;
	
}

void SystemPanel::tickStateMachine( int msTicksDelta )
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
