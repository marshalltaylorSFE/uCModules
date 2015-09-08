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
#include "timeKeeper32.h"
#include "timerModule32.h"

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
	if( timeElapsed < MAXTIMER )
	{
		timeElapsed += var_Increment;
	}
}
