//**********************************************************************//
//
//  MIT License: http://opensource.org/licenses/MIT
//
//  Written by:  Marshall Taylor
//  Changelog (YYYY/MM/DD):
//    2015/03/21: Created
//    2015/10/08: Initial Release
//
//**********************************************************************//


//Includes
#include "timerModule.h"
extern uint16_t maxTimer;
extern uint16_t maxInterval;
//**********************************************************************//
//
//  Periodic timer class
//
//**********************************************************************//
TimerClass::TimerClass( int intervalVar )
{
    flag = WAITING;
    lastService = 0;
    interval = intervalVar;
}

void TimerClass::update( uint16_t msTicksInput )
{
    //Check if overflow has occurred
    if( msTicksInput < lastService )  //overflow has occurred
    {
        //Adjust as if nothing ever happened
        lastService = lastService - maxTimer;
    }

    //Now process knowing an overflow has been dealt with if present
    if( msTicksInput >= (lastService + interval) )
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

int TimerClass::flagStatus()
{
    int returnVar = flag;
    flag = WAITING;
    return returnVar;

}

void TimerClass::setInterval( uint16_t msLength )
{
  interval = msLength;
  
}
