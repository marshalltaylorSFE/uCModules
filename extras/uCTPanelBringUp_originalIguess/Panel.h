#ifndef PANEL_H
#define PANEL_H

#include "PanelComponents.h"
#include <Arduino.h>
//#include "timeKeeper.h"
#include "sx1509Panel_library.h"  // Include the Modified SX1509 library


class SX1509Panel
{
public:
	SX1509Panel( void ); //Pass address
	void update( void );
	void pinMode( uint8_t, uint8_t, PanelButton * );
	void pinMode( uint8_t, uint8_t, PanelLed * );
	void init( uint8_t ); //Run to apply settings
	uint8_t address;
	uint8_t * inputCachePointers[16];
	uint8_t * outputCachePointers[16];
	uint8_t nullValue;
	uint16_t portDir;
	uint16_t portPU;
	
	sx1509Class expanderClass;
};

class Panel
{
public:
  Panel( void );
  void update( void );
  void init( void );
  void timersMIncrement( uint8_t );
  
  PanelButton tapButton;
  PanelButton syncButton;
  PanelButton playButton;
  PanelButton stopButton;
  PanelLed recordLed;
  PanelLed playLed;
  PanelLed overdubLed;
  PanelLed tapLed;
  PanelLed syncLed;
  PanelSelector quantizeSelector;

  PanelButton songUpButton;
  PanelButton songDownButton;
  PanelButton trackUpButton;
  PanelButton trackDownButton;
  PanelButton option1Button;
  PanelButton option2Button;
  PanelButton option3Button;
  PanelButton option4Button;
  PanelLed option1Led;
  PanelLed option2Led;
  PanelLed option3Led;
  PanelLed option4Led;
    
  SX1509Panel expanderA;
  
protected:
private:
};

#endif // PANEL_H



