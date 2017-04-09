//**********************************************************************//
//  BEERWARE LICENSE
//
//  This code is free for any use provided that if you meet the author
//  in person, you buy them a beer.
//
//  This license block is BeerWare itself.
//
//  Written by:  Marshall Taylor
//  Created:  May 26, 2015
//
//**********************************************************************//
#ifndef CIRCULARBUFFER_H_INCLUDED
#define CIRCULARBUFFER_H_INCLUDED

#include "stdint.h"

//Classes
template <class T>
class CircularBuffer
{
public:
	CircularBuffer<T>( uint8_t inputDepth )
	{
		bufferDepth = inputDepth;
		buffer = new T[bufferDepth];
		buffer[0] = 0;
		//Create success logging here
	};
	//~CircularBuffer( void )
	T read( uint8_t inputDepth )
	{
		//Start the returnPtr as 0
		int8_t returnPtr = 0;
		//If depth is too deep, clamp it.
		if( inputDepth >= bytesUsed )
		{
			inputDepth = bytesUsed - 1;
		}
		//Ok, add the offset
		returnPtr = topPointer + inputDepth;
		//If position exceeds max-depth, roll
		if( returnPtr > bufferDepth )
		{
			returnPtr = returnPtr - (bufferDepth + 1);
		}
		//If there is no data in the buffer
		float returnVal = 0;
		if( bytesUsed == 0 )
		{
			//Woah, move that pointer back to the zero cell
			returnVal = 0;
		}
		else
		{
			returnVal = buffer[returnPtr];
		}
		//Return
		return returnVal;
	};
	void write( T inputData )
	{
		//Lift the top pointer, unless
		if( topPointer > 0 )
		{
			//ok, lift
			topPointer--;
		}
		else
		{
			//flip
			topPointer = bufferDepth;
		}
		//Write the byte
		buffer[topPointer] = inputData;
		//Increment the bytesUsed if less than max
		if( bytesUsed <= bufferDepth )
		{
			bytesUsed++;
		}
	};
	void clear( void )
	{
		topPointer = 0;
		bytesUsed = 0;
		buffer[0] = 0;
	};
	uint8_t debugVariable = 0;
private:
	uint8_t topPointer;
	uint8_t bytesUsed;
	uint8_t bufferDepth;
	T * buffer;

};

#endif
