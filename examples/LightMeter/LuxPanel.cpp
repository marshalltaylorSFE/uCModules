#include "LuxPanel.h"
#include "panelComponents.h"
#include "HardwareInterfaces.h"

LuxPanel::LuxPanel( void )
{
	upButton.setHardware(new ArduinoDigitalIn( 22 ));
	add( &upButton );
	
	downButton.setHardware(new ArduinoDigitalIn( 23 ));
	add( &downButton );
	
	encButton.setHardware(new ArduinoDigitalIn( 4 ));
	add( &encButton );

	//add( dataWheel, PCHTeensyRotoryEncoder, TeensyRotoryEncoder hwParams(2, 3) );
	
}

void LuxPanel::tickStateMachine( int msTicksDelta )
{
	freshenComponents( msTicksDelta );
	//Do logic here
	Serial.println(upButton.getState());
	Serial.println(downButton.getState());
}