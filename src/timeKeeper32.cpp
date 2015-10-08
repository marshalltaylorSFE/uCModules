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


//Includes
#include "timeKeeper32.h"
#include "timerModule32.h"

#define MAXTIME 60000000

//**********************************************************************//
//
//  Time Keeper
//    Keeps time in number of microseconds
//
//**********************************************************************//
TimeKeeper32::TimeKeeper32( void )
{
    timeElapsed = 0;

}

void TimeKeeper32::uClear( void )
{
    timeElapsed = 0;

}

uint32_t TimeKeeper32::uGet( void )
{
    return timeElapsed;
}

void TimeKeeper32::uIncrement( uint32_t var_Increment )
{
    //clamp it.
	if( timeElapsed < MAXTIME )
	{
		timeElapsed += var_Increment;
	}
}
