//********************************************//
#include "SixKnobPanel.h"
#include "panelComponents.h"
#include "HardwareInterfaces.h"
#include <Arduino.h>

SixKnobPanel::SixKnobPanel( void )
{
	knob1.setHardware(new ArduinoAnalogIn( A0 ));
	add( &knob1 );
	knob2.setHardware(new ArduinoAnalogIn( A1 ));
	add( &knob2 );
	knob3.setHardware(new ArduinoAnalogIn( A2 ));
	add( &knob3 );
	knob4.setHardware(new ArduinoAnalogIn( A3 ));
	add( &knob4 );
	knob5.setHardware(new ArduinoAnalogIn( A4 ));
	add( &knob5 );
	knob6.setHardware(new ArduinoAnalogIn( A5 ));
	add( &knob6 );
	
	state = PInit;
}

void SixKnobPanel::reset( void )
{
	//Set explicit states
	state = PInit;
	
}

void SixKnobPanel::tickStateMachine( int msTicksDelta )
{
	freshenComponents( msTicksDelta );
	//***** PROCESS THE LOGIC *****//
	//Now do the states.
	PStates nextState = state;
	switch( state )
	{
	case PInit:
		nextState = PRun;
		Serial.println("Init state");
		break;
	case PRun:
		//Serial.println(knob1.serviceChanged());
		if( knob1.serviceChanged() || knob2.serviceChanged() || knob3.serviceChanged()
			|| knob4.serviceChanged() || knob5.serviceChanged() || knob6.serviceChanged() )
			{
				//Print stuff
				Serial.print( knob1.getState() );
				Serial.print(" ");
				Serial.print( knob2.getState() );
				Serial.print(" ");
				Serial.print( knob3.getState() );
				Serial.print(" ");
				Serial.print( knob4.getState() );
				Serial.print(" ");
				Serial.print( knob5.getState() );
				Serial.print(" ");
				Serial.print( knob6.getState() );
				Serial.println();
			}
		break;		
	default:
		nextState = PInit;
		break;
	}
	state = nextState;

}
