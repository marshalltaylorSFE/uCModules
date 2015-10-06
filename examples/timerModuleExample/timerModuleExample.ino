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
#define MAXTIMER 60000
#define MAXINTERVAL 2000


#define LEDPIN 13
#include "timerModule.h"
#include "stdint.h"

//If 328p based, do this
#ifdef __AVR__
#include <Arduino.h>
#endif

//If Teensy 3.1 based, do this
#ifdef __MK20DX256__
IntervalTimer myTimer;
#endif

//Globals
TimerClass msTimerA( 20 );
TimerClass msTimerB( 21 );

uint16_t msTicks = 0;
uint8_t msTicksMutex = 1; //start locked out

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

int i = 0;
int intervalSeed = 20;
void loop()
{
  // main program
  
  if( msTicksMutex == 0 )  //Only touch the timers if clear to do so.
  {
    msTimerA.update(msTicks);
    msTimerB.update(msTicks);
    //Done?  Lock it back up
    msTicksMutex = 1;
  }  //The ISR should clear the mutex.
  
  
  if(msTimerA.flagStatus() == PENDING)
  {
    digitalWrite( LEDPIN, digitalRead(LEDPIN) ^ 1 );
  }
  if(msTimerB.flagStatus() == PENDING)
  {
    digitalWrite( LEDPIN, digitalRead(LEDPIN) ^ 1 );
  }
  i++;
  delay(1);
  if(i > 100)
  {
    i = 0;
    msTimerA.setInterval(intervalSeed);
    intervalSeed++;
    msTimerB.setInterval(intervalSeed);

  }

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
  msTicksMutex = 0;  //unlock
}


