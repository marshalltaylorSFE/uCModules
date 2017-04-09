//**********************************************************************//
//  BEERWARE LICENSE
//
//  This code is free for any use provided that if you meet the author
//  in person, you buy them a beer.
//
//  This license block is BeerWare itself.
//
//  Written by:  Marshall Taylor
//  Created:  March 21, 2015
//
//**********************************************************************//
#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED
#include <CircularBuffer.h>

//Classes
class BasicClass
{
    int variable;
public:
    BasicClass( void );
    int basicMethod( int input );

    CircularBuffer<uint8_t> myCircularBuffer;
};

#endif // FILE_H_INCLUDED
