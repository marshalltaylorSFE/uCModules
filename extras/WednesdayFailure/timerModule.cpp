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

//Includes
#include "timerModule.h"

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
        lastService = lastService - MAXTIMER;
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
