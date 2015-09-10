//**********************************************************************//
//  BEERWARE LICENSE
//
//  This code is free for any use provided that if you meet the author
//  in person, you buy them a beer.
//
//  This license block is BeerWare itself.
//
//  Written by:  Marshall Taylor
//  Created:  March 21, 2015
//
//**********************************************************************//

//**Panels and stuff**************************//

#include "Panel.h"
#include "PanelComponents.h"


//Panel related variables
Panel myPanel;

//**Timers and stuff**************************//
#include "timerModule.h"

//Globals
IntervalTimer myTimer;

#define MAXINTERVAL 2000 //Max TimerClass interval

//HOW TO OPERATE
//  Make TimerClass objects for each thing that needs periodic service
//  pass the interval of the period in ticks
//  Set MAXINTERVAL to the max foreseen interval of any TimerClass
//  Set MAXTIMER to overflow number in the header.  MAXTIMER + MAXINTERVAL
//    cannot exceed variable size.

TimerClass debugTimerClass( 333 );
TimerClass panelUpdateTimer(10);
uint8_t debugLedStates = 1;

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
  
  //Init panel.h stuff
  myPanel.init();
  
  // initialize IntervalTimer
  myTimer.begin(serviceMS, 1000);  // serviceMS to run every 0.001 seconds
  
  //Debug setting of random states and stuff
  
  //Set all LED off
  myPanel.recordLed.setState(LEDOFF);
  myPanel.playLed.setState(LEDOFF);
  myPanel.overdubLed.setState(LEDOFF);
  myPanel.tapLed.setState(LEDOFF);
  myPanel.syncLed.setState(LEDOFF);

  myPanel.update();
  
  //while(1);
  
  //debugTimeKeeper.mClear();
  
  Wire.begin();  // Initialize hardware I2C pins
  // Clear the displays
  leftDisplay.clear();  // Clears display, resets cursor
  leftDisplay.setBrightness(255);  // High brightness
  sprintf(tempString, "%4d", (unsigned int)8888);
  leftDisplay.SendString(tempString);

  rightDisplay.clear();  // Clears display, resets cursor
  rightDisplay.setBrightness(255);  // High brightness
  sprintf(tempString, "%4d", (unsigned int)9999);
  rightDisplay.SendString(tempString);

  
}

void loop()
{
//**Copy to make a new timer******************//  
//    msTimerA.update(msTicks);
	 debugTimerClass.update(msTicks);
	 ledToggleTimer.update(msTicks);
	 ledToggleFastTimer.update(msTicks);
	 panelUpdateTimer.update(msTicks);
	 debounceTimer.update(msTicks);
	 
//**Copy to make a new timer******************//  
//  if(msTimerA.flagStatus() == PENDING)
//  {
//    digitalWrite( LEDPIN, digitalRead(LEDPIN) ^ 1 );
//  }
//**Debounce timer****************************//  
 if(debounceTimer.flagStatus() == PENDING)
 {
   myPanel.timersMIncrement(5);
   
 }
	
	//**Update the panel LEDs and stuff***********//  
	if(panelUpdateTimer.flagStatus() == PENDING)
	{
		myPanel.update();
		//Check for new data  ( does myPanel.memberName.newData == 1? )
		uint8_t tempStatus = 0;
		tempStatus |= myPanel.tapButton.newData;
		tempStatus |= myPanel.syncButton.newData;
		tempStatus |= myPanel.songUpButton.newData;
		tempStatus |= myPanel.songDownButton.newData;
		tempStatus |= myPanel.trackUpButton.newData;
		tempStatus |= myPanel.trackDownButton.newData;
		tempStatus |= myPanel.playButton.newData;
		tempStatus |= myPanel.stopButton.newData;
		tempStatus |= myPanel.quantizeSelector.newData;
		tempStatus |= myPanel.option1Button.newData;
		tempStatus |= myPanel.option2Button.newData;
		tempStatus |= myPanel.option3Button.newData;
		tempStatus |= myPanel.option4Button.newData;
		
		// If new, do something fabulous
		
		if( tempStatus )
		{
			if( myPanel.tapButton.newData == 1 )
			{
				if( myPanel.tapButton.getState() == 1 )
				{
					Serial.println("Tap");
					myPanel.tapLed.setState(LEDFLASHING);
				}
				else
				{
					myPanel.tapLed.setState(LEDOFF);
				}
			}
			if( myPanel.syncButton.newData == 1 )
			{
				if( myPanel.syncButton.getState() == 1 )
				{
					Serial.println("Sync");
					myPanel.syncLed.setState(LEDFLASHING);
				}
				else
				{
					myPanel.syncLed.setState(LEDOFF);
				}
			}
			if( myPanel.songUpButton.newData == 1 )
			{
				if( myPanel.songUpButton.getState() == 1 )
				{
					Serial.println("songUpButton");
					myPanel.overdubLed.setState(LEDFLASHINGFAST);
				}
			}
			if( myPanel.songDownButton.newData == 1 )
			{
				if( myPanel.songDownButton.getState() == 1 )
				{
					Serial.println("songDownButton");
					myPanel.tapLed.setState(LEDFLASHINGFAST);
					myPanel.syncLed.setState(LEDFLASHINGFAST);
				}
			}
			if( myPanel.trackUpButton.newData == 1 )
			{
				if( myPanel.trackUpButton.getState() == 1 )
				{
					Serial.println("trackUpButton");
					myPanel.recordLed.setState(LEDFLASHING);
				}
			}
			if( myPanel.trackDownButton.newData == 1 )
			{
				if( myPanel.trackDownButton.getState() == 1 )
				{
					Serial.println("trackDownButton");
					myPanel.playLed.setState(LEDFLASHING);
				}
			}
			if( myPanel.playButton.newData == 1 )
			{
				if( myPanel.playButton.getState() == 1 )
				{
					Serial.println("Play");
					//int i;
					//for( i = 0; i < 9; i++ )
					//{
					//	Serial.println(myPanel.quantizeSelector.thresholds[i]);
					//}
					
					//Set all LED on
					myPanel.recordLed.setState(LEDON);
					myPanel.playLed.setState(LEDON);
					myPanel.overdubLed.setState(LEDON);
					myPanel.tapLed.setState(LEDON);
					myPanel.syncLed.setState(LEDON);
				}
			}
			if( myPanel.stopButton.newData == 1 )
			{
				if( myPanel.stopButton.getState() == 1 )
				{
					Serial.println("Stop");
					
					//Serial.print("Analog Read: ");
					//Serial.println( (analogRead( A0 )) >> 2 );
					
					//Set all LED off
					myPanel.recordLed.setState(LEDOFF);
					myPanel.playLed.setState(LEDOFF);
					myPanel.overdubLed.setState(LEDOFF);
					myPanel.tapLed.setState(LEDOFF);
					myPanel.syncLed.setState(LEDOFF);
				}
			}
			if( myPanel.quantizeSelector.newData == 1 )
			{
				Serial.print("Quantitize: ");
				Serial.println( myPanel.quantizeSelector.getState() + 1 );
				
				sprintf(tempString, "%4d", (unsigned int)( myPanel.quantizeSelector.getState() + 1 ));
				leftDisplay.SendString(tempString);
				
				sprintf(tempString, "%4d", (unsigned int)( 9000 - (1000 * myPanel.quantizeSelector.getState()) ));
				rightDisplay.SendString(tempString);
			}
			if( myPanel.option1Button.newData == 1 )
			{
				if( myPanel.option1Button.getState() == BUTTONON )
				{
					myPanel.option1Led.setState( LEDON );
				}
				else
				{
					myPanel.option1Led.setState( LEDOFF );
				}
			}
			if( myPanel.option2Button.newData == 1 )
			{
				if( myPanel.option2Button.getState() == BUTTONON )
				{
					myPanel.option2Led.setState( LEDON );
				}
				else
				{
					myPanel.option2Led.setState( LEDOFF );
				}
			}
			if( myPanel.option3Button.newData == 1 )
			{
				if( myPanel.option3Button.getState() == BUTTONON )
				{
					myPanel.option3Led.setState( LEDON );
				}
				else
				{
					myPanel.option3Led.setState( LEDOFF );
				}
			}
			if( myPanel.option4Button.newData == 1 )
			{
				if( myPanel.option4Button.getState() == BUTTONON )
				{
					myPanel.option4Led.setState( LEDON );
				}
				else
				{
					myPanel.option4Led.setState( LEDOFF );
				}
			}
		}
	}


	if(debugTimerClass.flagStatus() == PENDING)
	{
	// if( debugLedStates == 0 )
	// {
		// debugLedStates = 1;
		// //Set all LED off
		// myPanel.hazardsOffLed.setState(LEDOFF);
		// myPanel.hazardsActiveLed.setState(LEDOFF);
		// //myPanel.matchReadyLed.setState(LEDOFF);
		// myPanel.matchPauseLed.setState(LEDOFF);
		// myPanel.doorsUnlockLed.setState(LEDOFF);
		// myPanel.doorsLeftAjarLed.setState(LEDOFF);
		// myPanel.doorsRightAjarLed.setState(LEDOFF);
		// //myPanel.update();	   
	// }
	// else
	// {
		// debugLedStates = 0;
		// //Set all LED on
		// myPanel.hazardsOffLed.setState(LEDON);
		// myPanel.hazardsActiveLed.setState(LEDON);
		// //myPanel.matchReadyLed.setState(LEDON);
		// myPanel.matchPauseLed.setState(LEDON);
		// myPanel.doorsUnlockLed.setState(LEDON);
		// myPanel.doorsLeftAjarLed.setState(LEDON);
		// myPanel.doorsRightAjarLed.setState(LEDON);
		// //myPanel.update();   
	// }
	}
	
	//**LED toggling of the panel class***********//  
	if(ledToggleFastTimer.flagStatus() == PENDING)
	{
	ledToggleFastState = ledToggleFastState ^ 0x01;
	
	}
	if(ledToggleTimer.flagStatus() == PENDING)
	{
	ledToggleState = ledToggleState ^ 0x01;
	
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
