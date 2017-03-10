#ifndef LUX_PANEL_H
#define LUX_PANEL_H

#include "Panel.h"
//#include "PanelComponents.h"  //included by panel
//#include "HardwareInterfaces.h"  //included by panel
#include "LightMeterPanelComponents.h"

enum PState_t
{
	PInit,
	PDisplayLuxValueInit,
	PDisplayLuxValue,
	PDisplayPhotoValueInit,
	PDisplayPhotoValue,
	PSetFStopInit,
	PSetFStop,
	PSetExposureInit,
	PSetExposure
};

class LuxPanel : public Panel
{
public:
	LuxPanel( void );
	void tickStateMachine( int msTicksDelta );

private:
	Button upButton;
	Button downButton;
	Button encButton;
	RotoryEncoder dataWheel;
	
	PState_t state;
	uint8_t fStopSetting;
	uint8_t exposureSetting;
	
};

#endif