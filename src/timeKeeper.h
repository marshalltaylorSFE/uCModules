//**********************************************************************//
//  BEERWARE LICENSE
//
//  This code is free for any use provided that if you meet the author
//  in person, you buy them a beer.
//
//  This license block is BeerWare itself.
//
//  Written by:  Marshall Taylor
//  Created:  May 26, 2015
//
//**********************************************************************//
#ifndef TIMEKEEPER_H_INCLUDED
#define TIMEKEEPER_H_INCLUDED

#include "stdint.h"

#define WAITING 0
#define PENDING 1
#define UNSERVICED 2


#define MAXTIMER 60000

//Classes
class TimeKeeper
{
public:
    TimeKeeper( void );
    void mClear( void );
    uint16_t mGet( void );
    void mIncrement( uint8_t );

private:
    uint16_t timeElapsed;

};


#endif // FILE_H_INCLUDED

