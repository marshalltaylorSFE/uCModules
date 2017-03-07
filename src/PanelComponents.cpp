#include "stdint.h"
#include "PanelComponents.h"
#include "HardwareInterfaces.h"
#include "Arduino.h"

void Button::setHardware( GenericHardwareDescription * input )
{
	hardwareInterface = input;
}

bool Button::hasFreshData( void )
{
	return 1;
}

void Button::freshen( uint16_t msTickDelta )
{
	hardwareInterface->readHardware();
	
}

uint8_t Button::getState( void )
{
	ButtonDataObject tempObject;
	hardwareInterface->getData(&tempObject);
	if( tempObject.size <= 0 )
	{
		return 0;
	}
	else
	{
		return *tempObject.data;
	}
}