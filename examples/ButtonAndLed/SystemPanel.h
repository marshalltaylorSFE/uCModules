//Header
#ifndef SYSTEMPANEL_H_INCLUDED
#define SYSTEMPANEL_H_INCLUDED

#include "LuxPanel.h"
#include "panelComponents.h"
#include "HardwareInterfaces.h"
#include <Arduino.h>

enum PStates
{
	PInit,
	POn,
	PFlash,
	PFastFlash
};

class SystemPanel : public Panel
{
public:
	SystemPanel( void );
	void reset( void );
	void tickStateMachine( int msTicksDelta );
	
private:
	//Internal Panel Components
	Button myButton;
	Led myLed;

	//  ..and data
	uint8_t held;
	
	//State machine stuff  
	PStates state;

};

#endif