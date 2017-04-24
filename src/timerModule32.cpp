//**********************************************************************//
//
//  MIT License: http://opensource.org/licenses/MIT
//
//  Written by:  Marshall Taylor
//  Changelog (YYYY/MM/DD):
//    2015/09/07: Created
//    2015/10/08: Initial Release
//
//**********************************************************************//

//Define TM32_DEBUG to get serial debug info regarding time and state
//#define TM32_DEBUG


//Includes
#include "timerModule32.h"
#ifdef TM32_DEBUG
#include <Arduino.h>
#endif
extern uint32_t maxTimer;
extern uint32_t maxInterval;

//**********************************************************************//
//
//  Periodic timer class
//
//**********************************************************************//
TimerClass32::TimerClass32( uint32_t intervalVar )
{
    flag = WAITING;
    lastService = 0;
    interval = intervalVar;
}

void TimerClass32::update( uint32_t usTicksInput )
{
    //Check if overflow has occurred
    if( (int32_t) usTicksInput < lastService )  //overflow has occurred
    {
        //Adjust as if nothing ever happened
        lastService = lastService - maxTimer;
#ifdef TM32_DEBUG
		Serial.println("OVF:");
		Serial.println(usTicksInput);
		Serial.println(lastService);
#endif
    }

    //seek until present
    while( usTicksInput >= (lastService + interval) )
    {
        //Timer has expired
        //Save the last service time
        lastService = lastService + interval;
        //Ready the flag
#ifdef TM32_DEBUG
		if( flag == PENDING ) Serial.println("SV_DPD");
#endif		
        if( flag == WAITING )
        {
            flag = PENDING;

        }
        else
        {
            flag = UNSERVICED;

        }
    }

}

int8_t TimerClass32::flagStatus()
{
    int8_t returnVar = flag;
    flag = WAITING;
    return returnVar;

}

void TimerClass32::setInterval( uint32_t usLength )
{
  interval = usLength;
  
}
