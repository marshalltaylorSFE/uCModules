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


//Includes
#include "timerModule32.h"

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
    if( usTicksInput < lastService )  //overflow has occurred
    {
        //Adjust as if nothing ever happened
        lastService = lastService - maxTimer;
    }

    //Now process knowing an overflow has been dealt with if present
    if( usTicksInput >= (lastService + interval) )
    {
        //Timer has expired
        //Save the last service time
        lastService = lastService + interval;
        //Ready the flag
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
