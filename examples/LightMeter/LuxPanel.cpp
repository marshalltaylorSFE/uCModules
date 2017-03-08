#include "LuxPanel.h"
#include "panelComponents.h"
#include "HardwareInterfaces.h"
#include <Arduino.h>

LuxPanel::LuxPanel( void )
{
	upButton.setHardware(new ArduinoDigitalIn( 22 ), 1);
	add( &upButton );
	
	downButton.setHardware(new ArduinoDigitalIn( 23 ), 1);
	add( &downButton );
	
	encButton.setHardware(new ArduinoDigitalIn( 4 ), 1);
	add( &encButton );

	//add( dataWheel, PCHTeensyRotoryEncoder, TeensyRotoryEncoder hwParams(2, 3) );
	
}

void LuxPanel::tickStateMachine( int msTicksDelta )
{
	freshenComponents( msTicksDelta );
	//Do logic here
	//Serial.println(upButton.getState());
	//Serial.println(downButton.getState());
	if(upButton.serviceRisingEdge()) Serial.println("RisingEdge");
	if(upButton.serviceFallingEdge()) Serial.println("FallingEdge");
	if(upButton.serviceHoldRisingEdge()) Serial.println("HoldRisingEdge");
	if(upButton.serviceHoldFallingEdge()) Serial.println("HoldFallingEdge");
}