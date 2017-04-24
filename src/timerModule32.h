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
#ifndef TIMERMODULE32_H_INCLUDED
#define TIMERMODULE32_H_INCLUDED

#include "stdint.h"

#define WAITING 0
#define PENDING 1
#define UNSERVICED 2

//Classes
class TimerClass32
{
    int8_t flag;
    int32_t lastService;
    int32_t interval;
public:
    TimerClass32( uint32_t );
    void update( uint32_t usTicksInput );
    int8_t flagStatus( void );
    void setInterval( uint32_t usLength );
    
};

#endif // FILE_H_INCLUDED

