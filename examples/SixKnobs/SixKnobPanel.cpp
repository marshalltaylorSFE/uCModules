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
		if( knob1.hasFreshData() || knob2.hasFreshData() || knob3.hasFreshData()
			|| knob4.hasFreshData() || knob5.hasFreshData() || knob6.hasFreshData() )
			{
				bool flagPrint = false;
				//Print knobs
				Serial.print("Knob: ");
				if( knob1.serviceChanged() )
				{
					Serial.print("1");
				}
				else Serial.print(" ");
				if( knob2.serviceChanged() )
				{
					flagPrint = true;
					windowDebugVar = knob2.getState();
					knob1.setWindow( windowDebugVar );
					knob2.setWindow( windowDebugVar );
					knob3.setWindow( windowDebugVar );
					knob4.setWindow( windowDebugVar );
					//knob5.setWindow( windowDebugVar );
					//knob6.setWindow( windowDebugVar );

					Serial.print("2");
				}
				else Serial.print(" ");
				if( knob3.serviceChanged() )
				{
					flagPrint = true;
					averagedDebugVar = 1 + knob3.getState() / 64;
					knob1.setSamplesAveraged( averagedDebugVar );
					knob2.setSamplesAveraged( averagedDebugVar );
					knob3.setSamplesAveraged( averagedDebugVar );
					knob4.setSamplesAveraged( averagedDebugVar );
					//knob5.setSamplesAveraged( averagedDebugVar );
					//knob6.setSamplesAveraged( averagedDebugVar );

					Serial.print("3");
				}
				else Serial.print(" ");
				if( knob4.serviceChanged() )
				{
					Serial.print("4");
				}
				else Serial.print(" ");
				if( knob5.serviceChanged() )
				{
					Serial.print("5");
				}
				else Serial.print(" ");
				if( knob6.serviceChanged() )
				{
					Serial.print("6");
				}
				else Serial.print(" ");
				
				Serial.print(" ");
				
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
				
				if(flagPrint)
				{
					Serial.print("  ");
					Serial.print( averagedDebugVar );
					Serial.print(" ave'd, window = ");
					Serial.print( windowDebugVar ); 
				}
				Serial.println();
				
			}
		break;
	default:
		nextState = PInit;
		break;
	}
	state = nextState;

}
