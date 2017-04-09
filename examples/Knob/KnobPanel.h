//Header
#ifndef KNOBPANEL_H_INCLUDED
#define KNOBPANEL_H_INCLUDED

#include "Panel.h"
#include "PanelComponents.h"
#include "HardwareInterfaces.h"
#include <Arduino.h>


enum PStates
{
	PInit,
	PRun,
};

class KnobPanel : public Panel
{
public:
	KnobPanel( void );
	void reset( void );
	void tickStateMachine( int msTicksDelta );
	
private:
	//Internal Panel Components
	Windowed10BitKnob myKnob;

	//State machine stuff  
	PStates state;

};



#endif