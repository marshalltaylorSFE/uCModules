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

//Includes
#include "timerModule32.h"

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
        lastService = lastService - MAXTIMER;
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
