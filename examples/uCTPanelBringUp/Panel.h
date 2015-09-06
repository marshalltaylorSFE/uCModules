#ifndef PANEL_H
#define PANEL_H

#include "PanelComponents.h"
#include <Arduino.h>
//#include "timeKeeper.h"

class Panel
{
public:
  Panel( void );
  void update( void );
  void init( void );
  void timersMIncrement( uint8_t );
  
  PanelSwitch hazardsSwitch;
  PanelLed hazardsOffLed;
  PanelLed hazardsActiveLed;
  PanelButton matchReady;
  PanelButton matchPause;
  PanelLed matchReadyLed;
  PanelLed matchPauseLed;
  PanelButton countStart;
  PanelButton countReset;
  PanelButton eStop;
  PanelSwitch doorsSwitch;
  PanelLed doorsUnlockLed;
  PanelLed doorsLeftAjarLed;
  PanelLed doorsRightAjarLed;

  PanelButton tapButton;
  PanelButton syncButton;
  PanelButton songUpButton;
  PanelButton songDownButton;
  PanelButton trackUpButton;
  PanelButton trackDownButton;
  PanelButton playButton;
  PanelButton stopButton;
  PanelLed recordLed;
  PanelLed playLed;
  PanelLed overdubLed;
  PanelLed tapLed;
  PanelLed syncLed;
  PanelSelector quantizeSelector;
  
protected:
private:
};

#endif // PANEL_H



