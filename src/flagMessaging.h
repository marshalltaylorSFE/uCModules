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

#ifndef FLAGMESSAGING_H
#define FLAGMESSAGING_H
#include <stdint.h>
/* TRY FIRST WITHOUT THIS
//---Outward Messaging-------------------------------------------
class OutwardMessagingFlag
{
public:
	OutwardMessagingFlag( void ); //Constructor
	uint8_t serviceRisingEdge( void );
	uint8_t serviceFallingEdge( void );
	uint8_t getFlag( void );
private:
	void setFlag( void );
	void clearFlag( void );
	
	uint8_t flag;
	uint8_t risingEdgeFlag;
	uint8_t fallingEdgeFlag;
	
};

//---Inward Messaging--------------------------------------------
class InwardMessagingFlag
{
public:
	InwardMessagingFlag( void ); //Constructor
	void setFlag( void );
	void clearFlag( void );
private:
	uint8_t serviceRisingEdge( void );
	uint8_t serviceFallingEdge( void );
	
	uint8_t flag;
	uint8_t risingEdgeFlag;
	uint8_t fallingEdgeFlag;
};*/

//---Generic Messaging-------------------------------------------
class MessagingFlag
{
public:
	MessagingFlag( void ); //Constructor
	void setFlag( void );
	void clearFlag( void );
	uint8_t getFlag( void );
	uint8_t serviceRisingEdge( void );
	uint8_t serviceFallingEdge( void );
	
private:
	uint8_t flag;
	uint8_t risingEdgeFlag;
	uint8_t fallingEdgeFlag;
};
#endif
