//**********************************************************************//
//
//  MIT License: http://opensource.org/licenses/MIT
//
//  Written by:  Marshall Taylor
//  Changelog (YYYY/MM/DD):
//    2015/09/20: Created
//    2015/10/08: Initial Release
//
//**********************************************************************//
#include "flagMessaging.h"

//---Generic Messaging-------------------------------------------
MessagingFlag::MessagingFlag( void )
{
	flag = 0;
	risingEdgeFlag = 0;
	fallingEdgeFlag = 0;
	
}

uint8_t MessagingFlag::serviceRisingEdge( void )
{
	uint8_t returnVar = 0;
	if( risingEdgeFlag == 1 )
	{
		risingEdgeFlag = 0;
		returnVar = 1;
	}
	
	return returnVar;
}
uint8_t MessagingFlag::serviceFallingEdge( void )
{
	uint8_t returnVar = 0;
	if( fallingEdgeFlag == 1 )
	{
		fallingEdgeFlag = 0;
		returnVar = 1;
	}
	
	return returnVar;
}
uint8_t MessagingFlag::getFlag( void )
{
	return flag;
}

void MessagingFlag::setFlag( void )
{
	if(flag == 1)
	{
		//Flag already set.  What to do!
		//risingEdgeFlag = 1;
	}
	else
	{
		flag = 1;
		risingEdgeFlag = 1;
	}
}
void MessagingFlag::clearFlag( void )
{
	if(flag == 0)
	{
		//Flag already set.  What to do!
		//fallingEdgeFlag = 1;
	}
	else
	{
		flag = 0;
		fallingEdgeFlag = 1;
	}
}