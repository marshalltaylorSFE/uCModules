#ifndef PANEL_COMPONENTS_H
#define PANEL_COMPONENTS_H

#include "stdint.h"
#include "HardwareInterfaces.h"
#include "timeKeeper.h"

class PanelComponent
{
public:
	PanelComponent( void ){
	};
	virtual bool hasFreshData( void ){
		return 0;
	}
	virtual void freshen( uint16_t msTickDelta );
protected:
	GenericHardwareDescription * hardwareInterface;
public:	
	static void freshenStatic( uint16_t msTickDelta );
protected:
	static uint8_t flasherCounter;
	static uint8_t fastFlasherCounter;
	static uint8_t flasherCounterMax;
	static uint8_t fastFlasherCounterMax;
	static uint8_t flasherState;
	static uint8_t fastFlasherState;
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

//---Led---------------------------------------------------------
enum ledState_t
{
	LEDON = 0,
	LEDOFF = 1,
	LEDFLASHING = 2,
	LEDFLASHINGFAST = 3
};

class Led : public PanelComponent
{
public:
	Led( void ) : state(LEDOFF){};
	void setHardware( GenericHardwareDescription * input, uint8_t invertInput );
	void freshen( uint16_t msTickDelta );
	
	ledState_t getState( void );
	void setState( ledState_t );
	void toggle( void );
	
private:
	uint8_t invert;
	ledState_t state;

};
#endif