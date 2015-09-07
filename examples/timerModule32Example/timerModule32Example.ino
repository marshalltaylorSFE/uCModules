//**********************************************************************//
//  BEERWARE LICENSE
//
//  This code is free for any use provided that if you meet the author
//  in person, you buy them a beer.
//
//  This license block is BeerWare itself.
//
//  Written by:  Marshall Taylor
//  Changelog (YYYY/MM/D):
//    2015/09/07: Created
//  
//**********************************************************************//

//HOW TO OPERATE
//  Make TimerClass objects for each thing that needs periodic service
//  pass the interval of the period in ticks
//  Set MAXINTERVAL to the max foreseen interval of any TimerClass
//  Set MAXTIMER to overflow number in the header.  MAXTIMER + MAXINTERVAL
//    cannot exceed variable size.

//NOTICE:
//  The timerModule32 only works on teensy / fast processors.  It works the same
//  but keeps track of everything in us counts.

#define LEDPIN 13
#include "timerModule32.h"
#include "stdint.h"

IntervalTimer myTimer;

//Globals
TimerClass32 usTimerA( 20000 );
TimerClass32 usTimerB( 21000 );

uint32_t usTicks = 0;
uint8_t usTicksMutex = 1; //start locked out

#define MAXINTERVAL 2000000

void setup()
{
  //Serial.begin(9600);
  pinMode(LEDPIN, OUTPUT);

  // initialize IntervalTimer
  myTimer.begin(serviceUS, 1);  // serviceMS to run every 0.000001 seconds

}

int i = 0;
int32_t intervalSeed = 20000;
void loop()
{
  // main program
  
  if( usTicksMutex == 0 )  //Only touch the timers if clear to do so.
  {
    usTimerA.update(usTicks);
    usTimerB.update(usTicks);
    //Done?  Lock it back up
    usTicksMutex = 1;
  }  //The ISR should clear the mutex.
  
  
  if(usTimerA.flagStatus() == PENDING)
  {
    digitalWrite( LEDPIN, digitalRead(LEDPIN) ^ 1 );
  }
  if(usTimerB.flagStatus() == PENDING)
  {
    digitalWrite( LEDPIN, digitalRead(LEDPIN) ^ 1 );
  }
  i++;
  delay(1);
  if(i > 100)
  {
    i = 0;
    usTimerA.setInterval(intervalSeed);
    intervalSeed = intervalSeed + 1000;
    usTimerB.setInterval(intervalSeed);

  }

}

void serviceUS(void)
{
  uint32_t returnVar = 0;
  if(usTicks >= ( MAXTIMER + MAXINTERVAL ))
  {
    returnVar = usTicks - MAXTIMER;

  }
  else
  {
    returnVar = usTicks + 1;
  }
  usTicks = returnVar;
  usTicksMutex = 0;  //unlock
}


