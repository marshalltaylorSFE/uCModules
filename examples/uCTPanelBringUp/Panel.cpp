#include "Panel.h"

//This is where PanelComponents are joined to form the custom panel
#define tapButtonPin 4
#define syncButtonPin 5
#define songUpButtonPin 6
#define songDownButtonPin 7
#define trackUpButtonPin 8
#define trackDownButtonPin 9
#define playButtonPin 2
#define stopButtonPin 3
#define recordLedPin A1
#define playLedPin A2
#define overdubLedPin A3
#define tapLedPin A8
#define syncLedPin A9
#define quantizeSelectorPin A0

Panel::Panel( void )
{

}

void Panel::init( void )
{
  tapButton.init(tapButtonPin);
  syncButton.init(syncButtonPin);
  songUpButton.init(songUpButtonPin);
  songDownButton.init(songDownButtonPin);
  trackUpButton.init(trackUpButtonPin);
  trackDownButton.init(trackDownButtonPin);
  playButton.init(playButtonPin);
  stopButton.init(stopButtonPin);
  recordLed.init(recordLedPin);
  playLed.init(playLedPin);
  overdubLed.init(overdubLedPin);
  tapLed.init(tapLedPin);
  syncLed.init(syncLedPin);
  quantizeSelector.init(quantizeSelectorPin, 255, 0 ); //With max and min ranges
  
}

void Panel::update( void )
{
  tapButton.update();
  syncButton.update();
  songUpButton.update();
  songDownButton.update();
  trackUpButton.update();
  trackDownButton.update();
  playButton.update();
  stopButton.update();
  recordLed.update();
  playLed.update();
  overdubLed.update();
  tapLed.update();
  syncLed.update();
  quantizeSelector.update();
}

void Panel::timersMIncrement( uint8_t inputValue )
{
  tapButton.buttonDebounceTimeKeeper.mIncrement(inputValue);
  syncButton.buttonDebounceTimeKeeper.mIncrement(inputValue);
  songUpButton.buttonDebounceTimeKeeper.mIncrement(inputValue);
  songDownButton.buttonDebounceTimeKeeper.mIncrement(inputValue);
  trackUpButton.buttonDebounceTimeKeeper.mIncrement(inputValue);
  trackDownButton.buttonDebounceTimeKeeper.mIncrement(inputValue);
  playButton.buttonDebounceTimeKeeper.mIncrement(inputValue);
  stopButton.buttonDebounceTimeKeeper.mIncrement(inputValue);

}
