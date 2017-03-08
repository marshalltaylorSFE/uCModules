#ifndef PANEL_COMPONENTS_H
#define PANEL_COMPONENTS_H

#include "stdint.h"
#include "HardwareInterfaces.h"
#include "timeKeeper.h"

class PanelComponent
{
public:
	PanelComponent( void ){};
	virtual bool hasFreshData( void ){
		return 0;
	}
	virtual void freshen( uint16_t msTickDelta );
protected:
	GenericHardwareDescription * hardwareInterface;
};

//---Button------------------------------------------------------
enum buttonState_t
{
	BUTTONOFF = 0,
	BUTTONON = 1,
	BUTTONHOLD = 2,
};

class Button : public PanelComponent
{
public:
	void setHardware( GenericHardwareDescription * input, uint8_t invertInput );
	bool hasFreshData( void );
	void freshen( uint16_t msTickDelta );
	buttonState_t getState( void );
	uint8_t serviceRisingEdge( void );
	uint8_t serviceFallingEdge( void );
	uint8_t serviceHoldRisingEdge( void );
	uint8_t serviceHoldFallingEdge( void );

private:
	buttonState_t state;
	TimeKeeper buttonDebounceTimeKeeper;
	uint8_t invert;
	uint8_t beingHeld;
	uint8_t risingEdgeFlag;
	uint8_t fallingEdgeFlag;
	uint8_t holdRisingEdgeFlag;
	uint8_t holdFallingEdgeFlag;
};
#endif