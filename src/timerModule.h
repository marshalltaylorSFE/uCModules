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
#ifndef TIMERMODULE_H_INCLUDED
#define TIMERMODULE_H_INCLUDED

#include "stdint.h"

#define WAITING 0
#define PENDING 1
#define UNSERVICED 2


#define MAXTIMER 60000

//Classes
class TimerClass
{
    int flag;
    uint16_t lastService;
    int16_t interval;
public:
    TimerClass( int );
    void update( uint16_t msTicksInput );
    int flagStatus( void );
    void setInterval( uint16_t msLength );
    
};

#endif // FILE_H_INCLUDED

