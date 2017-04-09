#include "KnobPanel.h"
#include "panelComponents.h"
#include "HardwareInterfaces.h"
#include <Arduino.h>

KnobPanel::KnobPanel( void )
{
	myKnob.setHardware(new ArduinoAnalogIn( A0 ));
	add( &myKnob );
	
	state = PInit;
}

void KnobPanel::reset( void )
{
	state = PInit;
	
}

void KnobPanel::tickStateMachine( int msTicksDelta )
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
		if( myKnob.serviceChanged() )
		{
			Serial.print("knob: ");
			Serial.println(myKnob.getState());
		}
		break;
	default:
		nextState = PInit;
		break;
	}
	state = nextState;

}
