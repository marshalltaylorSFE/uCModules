//Header
#ifndef CUSTOMPANEL_H_INCLUDED
#define CUSTOMPANEL_H_INCLUDED

#include "Panel.h"
#include "PanelComponents.h"
#include "HardwareInterfaces.h"
#include <Arduino.h>

enum PStates
{
	PInit,
	PRun
};

class CustomPanel : public Panel
{
public:
	CustomPanel( void );
	void reset( void );
	void tickStateMachine( int msTicksDelta );
	uint8_t getState( void ){
		return (uint8_t)state;
	}
	
private:
	//Internal Panel Components
	Button myButton;
	Led myLed;
	Simple10BitKnob myKnob;

	//State machine stuff  
	PStates state;

};

#endif