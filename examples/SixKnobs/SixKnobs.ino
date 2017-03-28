#include "SixKnobPanel.h"

SixKnobPanel myCustomPanel;

uint16_t debugCounter = 0;

void setup()
{
	Serial.begin(115200);
	barfOutStackPointer();
	//Go to fresh state
	myCustomPanel.reset();
	
}

//Set LOOP_DELAY to length of time between ticks of everything (in ms)
#define LOOP_DELAY 10

void loop()
{
	//Tick the machine
	myCustomPanel.tickStateMachine(LOOP_DELAY);

	//Delay the loop
	delay(LOOP_DELAY);
	
	//Increment a counter for debug stuff
	debugCounter = debugCounter + LOOP_DELAY;
	if( debugCounter > 1000 )
	{
		//Do debug stuff
		Serial.print("freeRam: ");
		Serial.println(freeRam());
		Serial.print("State number: ");
		Serial.println(myCustomPanel.getState());
		debugCounter = 0;
	}
	
}

void barfOutStackPointer()
{
	//uint8_t * tempVariable = SP;
	//Serial.print("Blghh: 0x");
	//Serial.println((uint16_t)tempVariable, HEX);
}


int freeRam () {
//  extern int __heap_start, *__brkval;
//  int v;
//  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}