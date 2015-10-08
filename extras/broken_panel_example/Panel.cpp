#include "Panel.h"


//This is where PanelComponents are joined to form the custom panel
#define tapButtonPin 4
#define syncButtonPin 5

#define playButtonPin 2
#define stopButtonPin 3
#define recordLedPin A1
#define playLedPin A2
#define overdubLedPin A3
#define tapLedPin A8
#define syncLedPin A9
#define quantizeSelectorPin A0

#define option1ButtonPin 3
#define option2ButtonPin 2
#define option3ButtonPin 0
#define option4ButtonPin 1
#define songUpButtonPin 7
#define songDownButtonPin 6
#define trackUpButtonPin 5
#define trackDownButtonPin 4

#define option1LedPin 8
#define option2LedPin 9
#define option3LedPin 10
#define option4LedPin 11

#define MAXSONG 24
#define MAXTRACK 16

Panel::Panel( void )
{
	songNum = 1;
	trackNum = 1;
}

void Panel::init( void )
{
	tapButton.init(tapButtonPin);
	syncButton.init(syncButtonPin);
	playButton.init(playButtonPin);
	stopButton.init(stopButtonPin);
	recordLed.init(recordLedPin, 0, &flasherState, &fastFlasherState);
	playLed.init(playLedPin, 0, &flasherState, &fastFlasherState);
	overdubLed.init(overdubLedPin, 0, &flasherState, &fastFlasherState);
	tapLed.init(tapLedPin, 0, &flasherState, &fastFlasherState);
	syncLed.init(syncLedPin, 0, &flasherState, &fastFlasherState);
	quantizeSelector.init(quantizeSelectorPin, 255, 0 ); //With max and min ranges

	songUpButton.init(songUpButtonPin, 1);
	songDownButton.init(songDownButtonPin, 1);
	trackUpButton.init(trackUpButtonPin, 1);
	trackDownButton.init(trackDownButtonPin, 1);
	option1Button.init(option1ButtonPin, 1);
	option2Button.init(option2ButtonPin, 1);
	option3Button.init(option3ButtonPin, 1);
	option4Button.init(option4ButtonPin, 1);
	option1Led.init(option1LedPin, 1, &flasherState, &fastFlasherState);
	option2Led.init(option2LedPin, 1, &flasherState, &fastFlasherState);
	option3Led.init(option3LedPin, 1, &flasherState, &fastFlasherState);
	option4Led.init(option4LedPin, 1, &flasherState, &fastFlasherState);
	
	//SX1509 stuff
	expanderA.pinMode( option1LedPin, OUTPUT, &option1Led );
	expanderA.pinMode( option2LedPin, OUTPUT, &option2Led );
	expanderA.pinMode( option3LedPin, OUTPUT, &option3Led );
	expanderA.pinMode( option4LedPin, OUTPUT, &option4Led );
	expanderA.pinMode( option1ButtonPin, INPUT_PULLUP, &option1Button );
	expanderA.pinMode( option2ButtonPin, INPUT_PULLUP, &option2Button );
	expanderA.pinMode( option3ButtonPin, INPUT_PULLUP, &option3Button );
	expanderA.pinMode( option4ButtonPin, INPUT_PULLUP, &option4Button );
	expanderA.pinMode( songUpButtonPin, INPUT_PULLUP, &songUpButton );
	expanderA.pinMode( songDownButtonPin, INPUT_PULLUP, &songDownButton );
	expanderA.pinMode( trackUpButtonPin, INPUT_PULLUP, &trackUpButton );
	expanderA.pinMode( trackDownButtonPin, INPUT_PULLUP, &trackDownButton );
	expanderA.init( 0x3E );
	

}

void Panel::update( void )
{
	tapButton.update();
	syncButton.update();
	playButton.update();
	stopButton.update();
	recordLed.update();
	playLed.update();
	overdubLed.update();
	tapLed.update();
	syncLed.update();
	option1Led.update();
	option2Led.update();
	option3Led.update();
	option4Led.update();

	quantizeSelector.update();

	songUpButton.update();
	songDownButton.update();
	trackUpButton.update();
	trackDownButton.update();
	option1Button.update();
	option2Button.update();
	option3Button.update();
	option4Button.update();
	
	if( songUpButton.newData == 1 )
	{
		switch( songUpButton.getState() )
		{
		case BUTTONON:
			songNum++;
			if( songNum > MAXSONG )
			{
				songNum -= MAXSONG;
			}
			break;
		case BUTTONOFF:
		default:
			break;
		}
	}
	if( songDownButton.newData == 1 )
	{
		switch(songDownButton.getState())
		{
		case BUTTONON:
			songNum--;
			if( songNum < 1 )
			{
				songNum += MAXSONG;
			}
			break;
		case BUTTONOFF:
		default:
			break;
		}
	}
	if( trackUpButton.newData == 1 )
	{
		switch( trackUpButton.getState() )
		{
		case BUTTONON:
			trackNum++;
			if( trackNum > MAXTRACK )
			{
				trackNum -= MAXTRACK;
			}
			break;
		case BUTTONOFF:
		default:
			break;
		}
	}
	if( trackDownButton.newData == 1 )
	{
		switch(trackDownButton.getState())
		{
		case BUTTONON:
			trackNum--;
			if( trackNum < 1 )
			{
				trackNum += MAXTRACK;
			}
			break;
		case BUTTONOFF:
		default:
			break;
		}
	}
	//OPTION SECTION
	if( option2Button.newData == 1 )
	{
		switch(option2Button.getState())
		{
		case BUTTONON:
			if( option2Led.getState() == LEDON )
			{
				option2Led.setState( LEDOFF );
			}
			else
			{
				option2Led.setState( LEDON );
			}
			break;
		case BUTTONOFF:
		default:
			break;
		}
	}
	if( option3Button.newData == 1 )
	{
		switch(option3Button.getState())
		{
		case BUTTONON:
			if( option3Led.getState() == LEDON )
			{
				option3Led.setState( LEDOFF );
			}
			else
			{
				option3Led.setState( LEDON );
			}
			break;
		case BUTTONOFF:
		default:
			break;
		}
	}
	if( option1Button.newData == 1 )
	{
		switch(option1Button.getState())
		{
		case BUTTONON:
			if( option1Led.getState() == LEDON )
			{
				option1Led.setState( LEDOFF );
			}
			else
			{
				option1Led.setState( LEDON );
			}
			break;
		case BUTTONOFF:
		default:
			break;
		}
	}
	if( option4Button.newData == 1 )
	{
		switch(option4Button.getState())
		{
		case BUTTONON:
			if( option4Led.getState() == LEDON )
			{
				option4Led.setState( LEDOFF );
			}
			else
			{
				option4Led.setState( LEDON );
			}
			break;
		case BUTTONOFF:
		default:
			break;
		}
	}

	//POST OPTIONS
	if( playButton.newData == 1 )
	{
		switch(playButton.getState())
		{
		case BUTTONON:
			recordLed.setState( LEDON );
			playLed.setState( LEDON );
			overdubLed.setState( LEDON );
			tapLed.setState( LEDON );
			syncLed.setState( LEDON );
			option1Led.setState( LEDON );
			option2Led.setState( LEDON );
			option3Led.setState( LEDON );
			option4Led.setState( LEDON );
			break;
		case BUTTONHOLD:
			recordLed.setState( LEDFLASHING );
			playLed.setState( LEDFLASHING );
			overdubLed.setState( LEDFLASHING );
			tapLed.setState( LEDFLASHING );
			syncLed.setState( LEDFLASHING );
			option1Led.setState( LEDFLASHING );
			option2Led.setState( LEDFLASHING );
			option3Led.setState( LEDFLASHING );
			option4Led.setState( LEDFLASHING );
			break;
		case BUTTONOFF:
		default:

			break;
		}
	}
	
	if( stopButton.newData == 1 )
	{
		switch(stopButton.getState())
		{
		case BUTTONON:
			recordLed.setState( LEDFLASHINGFAST );
			playLed.setState( LEDFLASHINGFAST );
			overdubLed.setState( LEDFLASHINGFAST );
			tapLed.setState( LEDFLASHINGFAST );
			syncLed.setState( LEDFLASHINGFAST );
			option1Led.setState( LEDFLASHINGFAST );
			option2Led.setState( LEDFLASHINGFAST );
			option3Led.setState( LEDFLASHINGFAST );
			option4Led.setState( LEDFLASHINGFAST );
			break;
		case BUTTONHOLD:
			recordLed.setState( LEDOFF );
			playLed.setState( LEDOFF );
			overdubLed.setState( LEDOFF );
			tapLed.setState( LEDOFF );
			syncLed.setState( LEDOFF );
			option1Led.setState( LEDOFF );
			option2Led.setState( LEDOFF );
			option3Led.setState( LEDOFF );
			option4Led.setState( LEDOFF );
			break;
		case BUTTONOFF:
		default:

			break;
		}
	}
	

	switch(tapButton.getState())
	{
	case BUTTONON:
		break;
	case BUTTONHOLD:
		songNum = quantizeSelector.getState();
		break;
	case BUTTONOFF:
	default:
		break;
	}

	switch(syncButton.getState())
	{
	case BUTTONON:
		break;
	case BUTTONHOLD:
		trackNum = quantizeSelector.getState();
		break;
	case BUTTONOFF:
	default:
		break;
	}


	//if( .newData == 1 )
	//{
	//	switch(.getState())
	//	{
	//	case :
	//		break;
	//	default:
	//		break;
	//	}
	//}
	expanderA.update();
	Serial.println("Tick");
}

void Panel::timersMIncrement( uint8_t inputValue )
{
	tapButton.buttonDebounceTimeKeeper.mIncrement(inputValue);
	syncButton.buttonDebounceTimeKeeper.mIncrement(inputValue);
	songUpButton.buttonDebounceTimeKeeper.mIncrement(inputValue);
	songDownButton.buttonDebounceTimeKeeper.mIncrement(inputValue);
	trackUpButton.buttonDebounceTimeKeeper.mIncrement(inputValue);
	trackDownButton.buttonDebounceTimeKeeper.mIncrement(inputValue);
	playButton.buttonDebounceTimeKeeper.mIncrement(inputValue);
	stopButton.buttonDebounceTimeKeeper.mIncrement(inputValue);
	option1Button.buttonDebounceTimeKeeper.mIncrement(inputValue);
	option2Button.buttonDebounceTimeKeeper.mIncrement(inputValue);
	option3Button.buttonDebounceTimeKeeper.mIncrement(inputValue);
	option4Button.buttonDebounceTimeKeeper.mIncrement(inputValue);
}

void Panel::toggleFlasherState( void )
{
	flasherState ^= 0x01;
}

void Panel::toggleFastFlasherState( void )
{
	fastFlasherState ^= 0x01;
}
