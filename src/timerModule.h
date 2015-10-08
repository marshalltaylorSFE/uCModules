//**********************************************************************//
//
//  MIT License: http://opensource.org/licenses/MIT
//
//  Written by:  Marshall Taylor
//  Changelog (YYYY/MM/DD):
//    2015/05/21: Created
//    2015/10/08: Initial Release
//
//**********************************************************************//
#ifndef TIMERMODULE_H_INCLUDED
#define TIMERMODULE_H_INCLUDED

#include "stdint.h"

#define WAITING 0
#define PENDING 1
#define UNSERVICED 2

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

