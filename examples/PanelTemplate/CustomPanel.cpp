//********************************************//
#include "CustomPanel.h"
#include "panelComponents.h"
#include "HardwareInterfaces.h"
#include <Arduino.h>

CustomPanel::CustomPanel( void )
{
	myButton.setHardware(new ArduinoDigitalIn( 3 ), 1);
	add( &myButton );

	myLed.setHardware(new ArduinoDigitalOut( 2 ), 0);
	add( &myLed );
	
	state = PInit;
}

void CustomPanel::reset( void )
{
	//Set explicit states
	//Set all LED off
	myLed.setState(LEDOFF);
	state = PInit;
	
}

void CustomPanel::tickStateMachine( int msTicksDelta )
{
	freshenComponents( msTicksDelta );
	
	//***** PROCESS THE LOGIC *****//
	//Now do the states.
	PStates nextState = state;
	switch( state )
	{
	case PInit:
		nextState = PRun;
		break;
	case PRun:
		break;		
	default:
		nextState = PInit;
		break;
	}
	state = nextState;

}
