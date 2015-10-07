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

//HOW TO OPERATE
//  Make TimerClass objects for each thing that needs periodic service
//  pass the interval of the period in ticks
//  Set MAXINTERVAL to the max foreseen interval of the loop
//    Calculate by summing the execution time of all timerClasses used
//    plus the time of the loop itself.  In other words, if all timers are
//    called in a particular loop, the msTicks will be incrementing for
//    extra time before the loop has a chance to update the timerClasses
//    so let msTicks increment rather than to reset ( or we lose track of
//    time ).
//  Set MAXTIMER to overflow number in the header.  MAXTIMER + MAXINTERVAL
//    cannot exceed variable size.

//Globals
uint16_t MAXTIMER = 60000;
uint16_t MAXINTERVAL = 2000;

#define LEDPIN 13
#include "timerModule.h"
#include "stdint.h"

//Not used by this sketch but dependant on one 
#include "Wire.h"

//If 328p based, do this
#ifdef __AVR__
#include <Arduino.h>
#endif

//If Teensy 3.1 based, do this
#ifdef __MK20DX256__
IntervalTimer myTimer;
#endif

//Globals
TimerClass msTimerA( 200 ); //milliseconds between calls
TimerClass msTimerB( 210 );
TimerClass msTimerC( 220 );
//Note on TimerClass-
//Change with msTimerA.setInterval( <the new interval> );


uint16_t msTicks = 0;

// The lock works like this:
//
//  When the interrupt fires, the lock is removed.  Now
// the main free-wheeling loop can update the change to
// the timerModules.  Once complete, the lock is replaced
// so that it can't update more than once per firing
// of the interrupt
uint8_t msTicksLocked = 1; //start locked out

void setup()
{
	//Serial.begin(9600);
	pinMode(LEDPIN, OUTPUT);


	//If 328p based, do this
#ifdef __AVR__
	// initialize Timer1
	cli();          // disable global interrupts
	TCCR1A = 0;     // set entire TCCR1A register to 0
	TCCR1B = 0;     // same for TCCR1B

	// set compare match register to desired timer count:
	OCR1A = 16000;

	// turn on CTC mode:
	TCCR1B |= (1 << WGM12);

	// Set CS10 and CS12 bits for 1 prescaler:
	TCCR1B |= (1 << CS10);


	// enable timer compare interrupt:
	TIMSK1 |= (1 << OCIE1A);

	// enable global interrupts:
	sei();
#endif

	//If Teensy 3.1 based, do this
#ifdef __MK20DX256__
	// initialize IntervalTimer
	myTimer.begin(serviceMS, 1000);  // serviceMS to run every 0.001 seconds
#endif


}

void loop()
{
	//Update the timers, but only once per interrupt
	if( msTicksLocked == 0 )
	{
		msTimerA.update(msTicks);
		msTimerB.update(msTicks);
		msTimerC.update(msTicks);
		//Done?  Lock it back up
		msTicksLocked = 1;
	}  //The ISR will unlock.


	if(msTimerA.flagStatus() == PENDING)
	{
		digitalWrite( LEDPIN, digitalRead( LEDPIN ) ^ 0x01 );
	}
	if(msTimerB.flagStatus() == PENDING)
	{
		digitalWrite( LEDPIN, digitalRead( LEDPIN ) ^ 0x01 );
	}
	if(msTimerC.flagStatus() == PENDING)
	{
		digitalWrite( LEDPIN, digitalRead( LEDPIN ) ^ 0x01 );
	}
	delay(1);


}

//If 328p based, do this
#ifdef __AVR__
ISR(TIMER1_COMPA_vect)
#else
#endif
//If Teensy 3.1 based, do this
#ifdef __MK20DX256__
void serviceMS(void)
#else
#endif
{
	uint32_t returnVar = 0;
	if( msTicks >= ( MAXTIMER + MAXINTERVAL ) )
	{
		returnVar = msTicks - MAXTIMER;

	}
	else
	{
		returnVar = msTicks + 1;
	}
	msTicks = returnVar;
	msTicksLocked = 0;  //unlock
}


