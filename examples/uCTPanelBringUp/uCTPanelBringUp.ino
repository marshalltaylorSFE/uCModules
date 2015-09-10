//**********************************************************************//
//  BEERWARE LICENSE
//
//  This code is free for any use provided that if you meet the author
//  in person, you buy them a beer.
//
//  This license block is BeerWare itself.
//
//  Written by:  Marshall Taylor
//  Created:  September 9, 2015
//
//**********************************************************************//
#include <Wire.h>
//**Panels and stuff**************************//

#include "Panel.h"

//Panel related variables
Panel myPanel;

//**Timers and stuff**************************//
#define MAXINTERVAL 2000 //Max TimerClass interval
#include "timerModule.h"
IntervalTimer myTimer;


//HOW TO OPERATE
//  Make TimerClass objects for each thing that needs periodic service
//  pass the interval of the period in ticks
//  Set MAXINTERVAL to the max foreseen interval of any TimerClass
//  Set MAXTIMER to overflow number in the header.  MAXTIMER + MAXINTERVAL
//    cannot exceed variable size.

TimerClass panelUpdateTimer(10);
TimerClass ledToggleTimer( 333 );
uint8_t ledToggleState = 0;
TimerClass ledToggleFastTimer( 100 );
uint8_t ledToggleFastState = 0;

TimerClass debounceTimer(5);
#include "timeKeeper.h"


//tick variable for interrupt driven timer1
uint16_t msTicks = 0;
uint8_t msTicksMutex = 1; //start locked out

//**Seven Segment Display*********************//
// Here we'll define the I2C address of our S7S. By default it
//  should be 0x71. This can be changed, though.
#include "Wire.h"
#include "s7sWrapper.h"
char tempString[10];  // Will be used with sprintf to create strings
S7sObject leftDisplay( 0x71 );
S7sObject rightDisplay( 0x30 );

// -----------------------------------------------------------------------------
void setup() 
{
	delay(4000);
	//Initialize serial:
	Serial.begin(9600);
	Serial.println("Program Started");

	// initialize IntervalTimer
	myTimer.begin(serviceMS, 1000);  // serviceMS to run every 0.001 seconds

	//Init panel.h stuff
	myPanel.init();
	myPanel.update();

	leftDisplay.clear();  // Clears display, resets cursor
	leftDisplay.setBrightness(255);  // High brightness
	sprintf(tempString, "%4d", (unsigned int)8888);
	leftDisplay.SendString(tempString);
	
	rightDisplay.clear();  // Clears display, resets cursor
	rightDisplay.setBrightness(255);  // High brightness
	sprintf(tempString, "%4d", (unsigned int)9999);
	rightDisplay.SendString(tempString);	

	//while(1);
}

void loop()
{
	//**Copy to make a new timer******************//  
	//msTimerA.update(msTicks);
	ledToggleTimer.update(msTicks);
	ledToggleFastTimer.update(msTicks);
	panelUpdateTimer.update(msTicks);
	debounceTimer.update(msTicks);
	
	//**Copy to make a new timer******************//  
	//if(msTimerA.flagStatus() == PENDING)
	//{
	//	digitalWrite( LEDPIN, digitalRead(LEDPIN) ^ 1 );
	//}
	//**Debounce timer****************************//  
	if(debounceTimer.flagStatus() == PENDING)
	{
		myPanel.timersMIncrement(5);

	}
	
	//**Update the panel LEDs and stuff***********//  
	if(panelUpdateTimer.flagStatus() == PENDING)
	{
		myPanel.update();
		sprintf(tempString, "%4d", (unsigned int)myPanel.songNum);
		leftDisplay.SendString(tempString);
		sprintf(tempString, "%4d", (unsigned int)myPanel.trackNum);
		rightDisplay.SendString(tempString);
	}

	//**Fast LED toggling of the panel class***********//  
	if(ledToggleFastTimer.flagStatus() == PENDING)
	{
		myPanel.toggleFastFlasherState();
		
	}

	//**LED toggling of the panel class***********//  
	if(ledToggleTimer.flagStatus() == PENDING)
	{
		myPanel.toggleFlasherState();
		
	}
}

void serviceMS(void)
{
	uint32_t returnVar = 0;
	if(msTicks >= ( MAXTIMER + MAXINTERVAL ))
	{
		returnVar = msTicks - MAXTIMER;

	}
	else
	{
		returnVar = msTicks + 1;
	}
	msTicks = returnVar;
	msTicksMutex = 0;  //unlock

}
