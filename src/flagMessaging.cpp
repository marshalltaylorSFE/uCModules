//**********************************************************************//
//  BEERWARE LICENSE
//
//  This code is free for any use provided that if you meet the author
//  in person, you buy them a beer.
//
//  This license block is BeerWare itself.
//
//  Written by:  Marshall Taylor
//  Created:  September 20, 2015
//
//**********************************************************************//

#include "flagMessaging.h"
/*
//---Outward Messaging-------------------------------------------
OutwardMessagingFlag::OutwardMessagingFlag( void )
{
	flag = 0;
	risingEdgeFlag = 0;
	fallingEdgeFlag = 0;
	
}

uint8_t OutwardMessagingFlag::serviceRisingEdge( void )
{
	uint8_t returnVar = 0;
	if( risingEdgeFlag == 1 )
	{
		risingEdgeFlag = 0;
		returnVar = 1;
	}
	
	return returnVar;
}
uint8_t OutwardMessagingFlag::serviceFallingEdge( void )
{
	uint8_t returnVar = 0;
	if( fallingEdgeFlag == 1 )
	{
		fallingEdgeFlag = 0;
		returnVar = 1;
	}
	
	return returnVar;
}
uint8_t OutwardMessagingFlag::getFlag( void )
{
	return flag;
}

void OutwardMessagingFlag::setFlag( void )
{
	if(flag == 1)
	{
		//Flag already set.  What to do!
		risingEdgeFlag = 1;
	}
	else
	{
		flag = 1;
		risingEdgeFlag = 1;
	}
}
void OutwardMessagingFlag::clearFlag( void )
{
	if(flag == 0)
	{
		//Flag already set.  What to do!
		fallingEdgeFlag = 1;
	}
	else
	{
		flag = 0;
		fallingEdgeFlag = 1;
	}
}

//---Inward Messaging--------------------------------------------
InwardMessagingFlag::InwardMessagingFlag( void )
{
	flag = 0;
	risingEdgeFlag = 0;
	fallingEdgeFlag = 0;
	
}
void InwardMessagingFlag::setFlag( void )
{
	if(flag == 1)
	{
		//Flag already set.  What to do!
		risingEdgeFlag = 1;
	}
	else
	{
		flag = 1;
		risingEdgeFlag = 1;
	}
}
void InwardMessagingFlag::clearFlag( void )
{
	if(flag == 0)
	{
		//Flag already set.  What to do!
		fallingEdgeFlag = 1;
	}
	else
	{
		flag = 0;
		fallingEdgeFlag = 1;
	}
}
uint8_t InwardMessagingFlag::serviceRisingEdge( void )
{
	uint8_t returnVar = 0;
	if( risingEdgeFlag == 1 )
	{
		risingEdgeFlag = 0;
		returnVar = 1;
	}
	
	return returnVar;
}
uint8_t InwardMessagingFlag::serviceFallingEdge( void )
{
	uint8_t returnVar = 0;
	if( fallingEdgeFlag == 1 )
	{
		fallingEdgeFlag = 0;
		returnVar = 1;
	}
	
	return returnVar;
}
*/

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