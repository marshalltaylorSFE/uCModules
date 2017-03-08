//Header
#ifndef SYSTEMPANEL_H_INCLUDED
#define SYSTEMPANEL_H_INCLUDED

#include "Panel.h"
#include "PanelComponents.h"
#include "HardwareInterfaces.h"
#include <Arduino.h>

enum PStates
{
	PInit,
	POn,
	PFlash,
	PFastFlash
};

class BlinkerPanel : public Panel
{
public:
	BlinkerPanel( void );
	void reset( void );
	void tickStateMachine( int msTicksDelta );
	uint8_t getState( void ){
		return (uint8_t)state;
	}
	
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