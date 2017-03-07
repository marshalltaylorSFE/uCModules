#ifndef LUX_PANEL_H
#define LUX_PANEL_H

#include "Panel.h"
//#include "PanelComponents.h"
//#include "HardwareInterfaces.h"

class LuxPanel : public Panel
{
public:
	LuxPanel( void );
	void tickStateMachine( int msTicksDelta );

private:	
	Button upButton;
	Button downButton;
	Button encButton;
	//RotoryEncoder dataWheel;
};

#endif