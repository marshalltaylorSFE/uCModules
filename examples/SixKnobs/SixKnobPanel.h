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
	PRun,

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
	Windowed10BitKnob knob1;
	Windowed10BitKnob knob2;
	Windowed10BitKnob knob3;
	Windowed10BitKnob knob4;
	Simple10BitKnob knob5;
	Simple8BitKnob knob6;

	//State machine stuff  
	PStates state;
	
	uint16_t windowDebugVar;
	uint16_t averagedDebugVar;

};

#endif