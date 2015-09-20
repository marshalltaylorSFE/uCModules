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
#include <stdint.h>
#include "Wire.h"

#define BUTTON_PIN 7
#define LED_PIN 13

class ClassB
{
public:
	ClassB( void ){};
	void tick( void )
	{
		if(digitalRead(BUTTON_PIN) == 1)
		{
			if(buttonFlag.getFlag() == 0)
			{
				buttonFlag.setFlag();
			}
		}
		else
		{
			if(buttonFlag.getFlag() == 1)
			{
				buttonFlag.clearFlag();
			}
		}
		if(ledFlag.serviceRisingEdge())
		{
			digitalWrite( LED_PIN, 1 );
		}
		if(ledFlag.serviceFallingEdge())
		{
			digitalWrite( LED_PIN, 0 );
		}
	};
	MessagingFlag buttonFlag;
	MessagingFlag ledFlag;
};

class ClassA
{
public:
	ClassA( void )
	{
	};
	void tick( void )
	{
		myClassB.tick();
		if(myClassB.buttonFlag.serviceRisingEdge())
		{
			myClassB.ledFlag.setFlag();
		}
		if(myClassB.buttonFlag.serviceFallingEdge())
		{
			myClassB.ledFlag.clearFlag();
		}

	};
private:
	ClassB myClassB;
};

ClassA myClassA;

void setup()
{
	Serial.begin(115200);
	delay(500);
	Serial.println("\nProgram Started.");
	pinMode( BUTTON_PIN, INPUT_PULLUP );
	pinMode( LED_PIN, OUTPUT );
	
}

void loop()
{
	myClassA.tick();
	delay(500);
}