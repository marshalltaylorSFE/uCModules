#ifndef PANEL_COMPONENTS_H
#define PANEL_COMPONENTS_H

#include "stdint.h"
#include "HardwareInterfaces.h"
#include "timeKeeper.h"
#include "CircularBuffer.h"

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
	static uint16_t flasherCounter;
	static uint16_t fastFlasherCounter;
	static uint16_t flasherCounterMax;
	static uint16_t fastFlasherCounterMax;
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
	void setHardware( GenericHardwareDescription * input, bool invertInput );
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
	bool invert;
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
	void setHardware( GenericHardwareDescription * input, bool invertInput );
	void freshen( uint16_t msTickDelta );
	
	ledState_t getState( void );
	void setState( ledState_t );
	void toggle( void );
	
private:
	bool invert;
	ledState_t state;

};

//---Knob--------------------------------------------------------
class Knob : public PanelComponent
{
public:
	Knob( void );
	void setHardware( GenericHardwareDescription * input );
	bool hasFreshData( void );
	void freshen( uint16_t msTickDelta );
	uint16_t getState( void );
	uint8_t serviceChanged( void );
private:
	uint16_t state;
	uint16_t lastState;
	uint8_t newData;

};

//---Simple10BitKnob---------------------------------------------
class Simple10BitKnob : public PanelComponent
{
public:
	Simple10BitKnob( void );
	void setHardware( GenericHardwareDescription * input );
	bool hasFreshData( void );
	void freshen( uint16_t msTickDelta );
	uint16_t getState( void );
	uint8_t serviceChanged( void );
private:
	uint16_t state;
	uint16_t lastState;
	const uint8_t hysteresis = 2;
	int8_t lastSlope = 1;
	uint8_t newData;

};

//---Hysteresis10BitKnob---------------------------------------------
class Hysteresis10BitKnob : public PanelComponent
{
public:
	Hysteresis10BitKnob( void );
	void setHardware( GenericHardwareDescription * input );
	bool hasFreshData( void );
	void freshen( uint16_t msTickDelta );
	uint16_t getState( void );
	uint8_t serviceChanged( void );
	uint8_t getAsUInt8( void );
	int16_t getAsInt16( void );
	uint16_t getAsUInt16( void );
	float getAsFloat( void );
	void setLowerKnobVal( uint16_t input );
	void setUpperKnobVal( uint16_t input );
	void setLowerFloatVal( float input );
	void setUpperFloatVal( float input );
	void setLowerIntVal( int16_t input );
	void setUpperIntVal( int16_t input );
	void setLowerUIntVal( uint16_t input );
	void setUpperUIntVal( uint16_t input );
	void setHysteresis( uint8_t input );
	void setSamplesAveraged( uint8_t input );
private:
	uint8_t hysteresis = 20;
	int8_t lastSlope = 1;
	uint8_t newData;
	uint8_t samplesAveraged = 7;
	CircularBuffer values;
	CircularBuffer averages;
	uint16_t lowerKnobVal = 0;
	uint16_t upperKnobVal = 1023;
	float lowerFloatVal = -1.0;
	float upperFloatVal = 1.0;
	int16_t lowerIntVal = -32768;
	int16_t upperIntVal = 32767;
	uint16_t lowerUIntVal = 0;
	uint16_t upperUIntVal = 65535;
	int32_t currentValue = 0;
	uint8_t hystState = 0; //can be 0 for lower or 1 for output (think positive feedback)
};

//---Windowed10BitKnob---------------------------------------------
class Windowed10BitKnob : public PanelComponent
{
public:
	Windowed10BitKnob( void );
	void setHardware( GenericHardwareDescription * input );
	bool hasFreshData( void );
	void freshen( uint16_t msTickDelta );
	uint16_t getState( void );
	uint8_t serviceChanged( void );
	uint8_t getAsUInt8( void );
	int16_t getAsInt16( void );
	uint16_t getAsUInt16( void );
	float getAsFloat( void );
	void setLowerKnobVal( uint16_t input );
	void setUpperKnobVal( uint16_t input );
	void setLowerFloatVal( float input );
	void setUpperFloatVal( float input );
	void setLowerIntVal( int16_t input );
	void setUpperIntVal( int16_t input );
	void setLowerUIntVal( uint16_t input );
	void setUpperUIntVal( uint16_t input );
	void setWindow( uint8_t input );
	void setSamplesAveraged( uint8_t input );
private:
	void moveWindowUpper( uint16_t input );
	void moveWindowLower( uint16_t input );
	int16_t windowUpper = 60;
	int16_t windowLower = 40;
	uint8_t window = 50;
	int8_t lastSlope = 1;
	uint8_t newData;
	uint8_t samplesAveraged = 6;
	CircularBuffer values;
	CircularBuffer averages;
	uint16_t lowerKnobVal = 0;
	uint16_t upperKnobVal = 1023;
	float lowerFloatVal = -1.0;
	float upperFloatVal = 1.0;
	int16_t lowerIntVal = -32768;
	int16_t upperIntVal = 32767;
	uint16_t lowerUIntVal = 0;
	uint16_t upperUIntVal = 65535;
	int32_t currentValue = 0;
};
#endif