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
const byte s7sAddress = 0x71;
char tempString[10];  // Will be used with sprintf to create strings

// -----------------------------------------------------------------------------
void setup() 
{
  //Initialize serial:
  Serial.begin(9600);

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
  // Clear the display, and then turn on all segments and decimals
  clearDisplayI2C();  // Clears display, resets cursor
  setBrightnessI2C(255);  // High brightness
    // Magical sprintf creates a string for us to send to the s7s.
  //  The %4d option creates a 4-digit integer.
  sprintf(tempString, "%4d", (unsigned int)8888);
  // This will output the tempString to the S7S
  s7sSendStringI2C(tempString);

  
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
				// This will output the tempString to the S7S
				s7sSendStringI2C(tempString);
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

// This custom function works somewhat like a serial.print.
//  You can send it an array of chars (string) and it'll print
//  the first 4 characters in the array.
void s7sSendStringI2C(String toSend)
{
  Wire.beginTransmission(s7sAddress);
  for (int i=0; i<4; i++)
  {
    Wire.write(toSend[i]);
  }
  Wire.endTransmission();
}

// Send the clear display command (0x76)
//  This will clear the display and reset the cursor
void clearDisplayI2C()
{
  Wire.beginTransmission(s7sAddress);
  Wire.write(0x76);  // Clear display command
  Wire.endTransmission();
}

// Set the displays brightness. Should receive byte with the value
//  to set the brightness to
//  dimmest------------->brightest
//     0--------127--------255
void setBrightnessI2C(byte value)
{
  Wire.beginTransmission(s7sAddress);
  Wire.write(0x7A);  // Set brightness command byte
  Wire.write(value);  // brightness data byte
  Wire.endTransmission();
}

// Turn on any, none, or all of the decimals.
//  The six lowest bits in the decimals parameter sets a decimal 
//  (or colon, or apostrophe) on or off. A 1 indicates on, 0 off.
//  [MSB] (X)(X)(Apos)(Colon)(Digit 4)(Digit 3)(Digit2)(Digit1)
void setDecimalsI2C(byte decimals)
{
  Wire.beginTransmission(s7sAddress);
  Wire.write(0x77);
  Wire.write(decimals);
  Wire.endTransmission();
}

  



