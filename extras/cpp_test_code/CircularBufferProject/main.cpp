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

//Includes
#include "file.h"
#include "stdio.h"
#include "CircularBuffer.h"

//Globals
BasicClass myClass;

int main( void )
{
    myClass.myCircularBuffer.write(252);
    myClass.myCircularBuffer.write(2928);
    printf("Passing 31, adding 20\n");
    printf("Result is: %d\n", myClass.basicMethod(31));

    for(int i = 0; i < 30; i++)
    {
        printf("Data at %d: %d\n", i, myClass.myCircularBuffer.read(i));
    }

}
