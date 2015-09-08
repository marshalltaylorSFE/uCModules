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
#ifndef TIMEKEEPER32_H_INCLUDED
#define TIMEKEEPER32_H_INCLUDED

#include "stdint.h"

// #define WAITING 0
// #define PENDING 1
// #define UNSERVICED 2


//#define MAXTIMER 60000000

//Classes
class TimeKeeper32
{
public:
    TimeKeeper32( void );
    void uClear( void );
    uint32_t uGet( void );
    void uIncrement( uint32_t );

private:
    uint32_t timeElapsed;

};


#endif // FILE_H_INCLUDED

