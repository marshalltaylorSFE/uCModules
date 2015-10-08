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
#ifndef TIMEKEEPER32_H_INCLUDED
#define TIMEKEEPER32_H_INCLUDED

#include "stdint.h"

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

