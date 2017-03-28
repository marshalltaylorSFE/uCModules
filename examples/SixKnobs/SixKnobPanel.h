//Header
#ifndef SIXKNOBPANEL_H_INCLUDED
#define SIXKNOBPANEL_H_INCLUDED

#include "Panel.h"
#include "PanelComponents.h"
#include "HardwareInterfaces.h"
#include <Arduino.h>

enum PStates
{
	PInit,
	PRun
};

class SixKnobPanel : public Panel
{
public:
	SixKnobPanel( void );
	void reset( void );
	void tickStateMachine( int msTicksDelta );
	uint8_t getState( void ){
		return (uint8_t)state;
	}
	
private:
	//Internal Panel Components
	Simple10BitKnob knob1;
	Simple10BitKnob knob2;
	Simple10BitKnob knob3;
	Simple10BitKnob knob4;
	Simple10BitKnob knob5;
	Simple10BitKnob knob6;

	//State machine stuff  
	PStates state;

};

#endif