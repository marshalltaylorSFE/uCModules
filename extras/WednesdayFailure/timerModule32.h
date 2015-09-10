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
#ifndef TIMERMODULE32_H_INCLUDED
#define TIMERMODULE32_H_INCLUDED

#include "stdint.h"

#define WAITING 0
#define PENDING 1
#define UNSERVICED 2


#define MAXTIMER 60000000  //60 seconds

//Classes
class TimerClass32
{
    int8_t flag;
    uint32_t lastService;
    int32_t interval;
public:
    TimerClass32( uint32_t );
    void update( uint32_t usTicksInput );
    int8_t flagStatus( void );
    void setInterval( uint32_t usLength );
    
};

#endif // FILE_H_INCLUDED

