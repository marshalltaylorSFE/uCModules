//**********************************************************************//
//
//  MIT License: http://opensource.org/licenses/MIT
//
//  Written by:  Marshall Taylor
//  Changelog (YYYY/MM/DD):
//    2015/10/08: Beta Release
//    2016/05/08: Fixed button falling edge detection and added LED invertion
//
//**********************************************************************//
#include "s7sWrapper.h"
#ifndef PANELCOMPONENTS_H
#define PANELCOMPONENTS_H

#include <Arduino.h>
#include "timeKeeper.h"
#include "flagMessaging.h"

//---Switch------------------------------------------------------
class PanelSwitch
{
public:
  PanelSwitch( void );
  void update( void );
  void init( uint8_t );
  MessagingFlag state;
  uint8_t invert;
  uint8_t pinNumber;
	TimeKeeper SwitchDebounceTimeKeeper;
protected:
private:
};

//---Knob--------------------------------------------------------
class PanelKnob8Bit
{
public:
  PanelKnob8Bit( void );
  void update( void );
  void init( uint8_t );
  uint8_t getState( void );
  uint8_t state;
  uint8_t pinNumber;
  uint8_t newData;
protected:
private:
};

//---Register----------------------------------------------------
class PanelRegister
{
public:
  PanelRegister( void );
  void update( void );
  void init( uint8_t, uint8_t, uint8_t );
  uint8_t getState( void );
  uint8_t state;
  uint8_t pinMap[8];
  uint8_t length;
  uint8_t newData;
protected:
private:
  PanelSwitch switchBit0;
  PanelSwitch switchBit1;
  PanelSwitch switchBit2;
};

//---Button------------------------------------------------------
enum buttonState_t
{
  BUTTONOFF = 0,
  BUTTONON = 1,
  BUTTONHOLD = 2,

};

class PanelButton
{
public:
	TimeKeeper buttonDebounceTimeKeeper;
	
	PanelButton( void );
	void update( void );
	void init( uint8_t );
	void init( uint8_t, uint8_t );
	buttonState_t getState( void );
	uint8_t serviceRisingEdge( void );
	uint8_t serviceFallingEdge( void );
	uint8_t serviceHoldRisingEdge( void );
	uint8_t serviceHoldFallingEdge( void );
	void setBank( uint8_t );
	buttonState_t state;
	uint8_t invert;
	uint8_t pinNumber;
	uint8_t cache;
protected:
private:
	uint8_t beingHeld;
	uint8_t bank;
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

class PanelLed
{
public:
  PanelLed( void );
  void update( void );
  void init( uint8_t );
  void init( uint8_t, uint8_t );
  void init( uint8_t, uint8_t, volatile uint8_t * volatile , volatile uint8_t * volatile );
  ledState_t getState( void );
  void setState( ledState_t );
  ledState_t state;
  uint8_t pinNumber;
  uint8_t cache;
  void setBank( uint8_t );
  void outputInvert( uint8_t );
protected:
private:
  uint8_t bank;
  volatile uint8_t * volatile flasherState;
  volatile uint8_t * volatile fastFlasherState;
  uint8_t invert;
};

//---Selector----------------------------------------------------
class PanelSelector
{
public:
  PanelSelector( void );
  ~PanelSelector( void );
  void update( void );
  void init( uint8_t, uint8_t, uint8_t );  //calls init(,,,);
  void init( uint8_t, uint8_t, uint8_t, uint8_t);//pinNum, maxInput, minInput, number of points
  uint8_t getState( void );
  
  uint8_t serviceChanged( void );
protected:
private:
  uint8_t state;
  uint8_t pinNumber;
  uint8_t * thresholds;
	uint8_t changedFlag;
	uint8_t points;
};

//---Seven Segment Display---------------------------------------
enum sSDisplayState_t
{
  SSON = 0,
  SSOFF = 1,
  SSFLASHING = 2,
  SSLASHINGFAST = 3,
  SSPEEKTHROUGH = 4
};

class sSDisplay : public S7sObject
{
public:
	sSDisplay( void );
	void update( void );
	void init( uint8_t );
	void init( uint8_t, volatile uint8_t * volatile , volatile uint8_t * volatile );
	sSDisplayState_t getState( void );
	void setState( sSDisplayState_t );
	void setData( String );
	void peekThrough( String, uint16_t ); // 'data' type, time in ms to persist
	TimeKeeper peekThroughTimeKeeper;
protected:
private:
	uint8_t peekThroughFlag;
	uint8_t peekingFlag;
	uint16_t peekThroughTime;
	sSDisplayState_t state;
	uint8_t updateDisplayFlag;
	uint8_t lastFlasherState;
	char data[5];
	char peekThroughData[5];
	volatile uint8_t * volatile flasherState;
	volatile uint8_t * volatile fastFlasherState;
	

};

#endif
