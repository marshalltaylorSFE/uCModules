#ifndef PANELCOMPONENTS_H
#define PANELCOMPONENTS_H
#include <Arduino.h>
#include "timeKeeper.h"

//---Switch------------------------------------------------------
class PanelSwitch
{
public:
  PanelSwitch( void );
  void update( void );
  void init( uint8_t );
  uint8_t getState( void );
  uint8_t state;
  uint8_t invert;
  uint8_t pinNumber;
  uint8_t newData;
protected:
private:
};

//---Button------------------------------------------------------
class PanelButton
{
public:
  TimeKeeper buttonDebounceTimeKeeper;
  
  PanelButton( void );
  void update( void );
  void init( uint8_t );
  uint8_t getState( void );
  uint8_t state;
  uint8_t invert;
  uint8_t pinNumber;
  uint8_t newData;
protected:
private:
};

//---Led---------------------------------------------------------
enum ledState_t
{
  LEDOFF,
  LEDON,
  LEDFLASHING,
  LEDFLASHINGFAST
};

class PanelLed
{
public:
  PanelLed( void );
  void update( void );
  void init( uint8_t );
  ledState_t getState( void );
  void setState( ledState_t );
  ledState_t state;
  uint8_t pinNumber;
  uint8_t newData;
protected:
private:
};

//---Selector----------------------------------------------------
class PanelSelector
{
public:
  PanelSelector( void );
  void update( void );
  void init( uint8_t, uint8_t, uint8_t );  // pinNum, maxInput, minInpu,
  uint8_t getState( void );
  uint8_t state;
  uint8_t pinNumber;
  uint8_t newData;
  uint8_t thresholds[9];
protected:
private:
};

#endif
