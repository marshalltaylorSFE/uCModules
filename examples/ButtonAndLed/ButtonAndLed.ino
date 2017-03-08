#include "Arduino.h"
#include "timerModule32.h"
//Globals
uint32_t MAXTIMER = 60000000;
uint32_t MAXINTERVAL = 2000000;

uint8_t ledToggleState = 0;
uint8_t ledToggleFastState = 0;

//**Copy to make a new timer******************//  
//        TimerClass32 usTimerA( 20000 ); //20 ms
//
//Note on TimerClass-
//Change with usTimerA.setInterval( <the new interval> );
TimerClass32 panelUpdateTimer(10000);
TimerClass32 debounceTimer(5000);
TimerClass32 ledToggleTimer( 333000 );
TimerClass32 ledToggleFastTimer( 100000 );

TimerClass32 debugTimer(5000000);

//**Panel State Machine***********************//
#include "SystemPanel.h"
SystemPanel mySimplePanel;

uint32_t usTicks = 0;

//  The lock works like this:
//
//    When the interrupt fires, the lock is removed.  Now
//    the main free-wheeling loop can update the change to
//    the timerModules.  Once complete, the lock is replaced
//    so that it can't update more than once per firing
//    of the interrupt

uint8_t usTicksLocked = 1; //start locked out

void setup()
{
	//Serial.begin(9600);


	//If 328p based, do this
#ifdef __AVR__
	// initialize Timer1
	cli();          // disable global interrupts
	TCCR1A = 0;     // set entire TCCR1A register to 0
	TCCR1B = 0;     // same for TCCR1B

	// set compare match register to desired timer count:
	OCR1A = 16000;

	// turn on CTC mode:
	TCCR1B |= (1 << WGM12);

	// Set CS10 and CS12 bits for 1 prescaler:
	TCCR1B |= (1 << CS10);


	// enable timer compare interrupt:
	TIMSK1 |= (1 << OCIE1A);

	// enable global interrupts:
	sei();
#endif

	Serial.begin(115200);

  	//Init panel.h stuff
	mySimplePanel.init();
	
	
	//Go to fresh state
	mySimplePanel.reset();
	
	//Update the panel
	mySimplePanel.update();
  
}

void loop()
{
	//Update the timers, but only once per interrupt
	if( usTicksLocked == 0 )
	{
		//**Copy to make a new timer******************//  
		//msTimerA.update(usTicks);

		//Panel timers
		ledToggleTimer.update(usTicks);
		ledToggleFastTimer.update(usTicks);
		panelUpdateTimer.update(usTicks);
		debounceTimer.update(usTicks);

		debugTimer.update(usTicks);
		
		//Done?  Lock it back up
		usTicksLocked = 1;
	}  //The ISR will unlock.

	//**Copy to make a new timer******************//  
	//if(usTimerA.flagStatus() == PENDING)
	//{
	//	//User code
	//}
	
	//**Debounce timer****************************//  
	if(debounceTimer.flagStatus() == PENDING)
	{
		mySimplePanel.timersMIncrement(5);
	
	}
		
	//**Process the panel and state machine***********//  
	if(panelUpdateTimer.flagStatus() == PENDING)
	{
		//Provide inputs

		//Tick the machine
		mySimplePanel.processMachine();
		
		//Deal with outputs 
		//  Form: if( myFlagName.resetTapHeadFlag.serviceRisingEdge() )...
		
	}
	//**Fast LED toggling of the panel class***********//  
	if(ledToggleFastTimer.flagStatus() == PENDING)
	{
		//mySimplePanel.toggleFastFlasherState();
		
	}

	//**LED toggling of the panel class***********//  
	if(ledToggleTimer.flagStatus() == PENDING)
	{
		//mySimplePanel.toggleFlasherState();
		
	}
	//**Debug timer*******************************//  
	if(debugTimer.flagStatus() == PENDING)
	{
		//uint32_t tempRef = analogRead(REFPIN);
		Serial.println("**************************Debug Service**************************");
		//Serial.print("Current state: ");
		//Serial.println(mySimplePanel.getState());
	
	
	}
}

//If 328p based, do this
#ifdef __AVR__
ISR(TIMER1_COMPA_vect)
#else
#endif
//If Teensy 3.1 based, do this
#ifdef __MK20DX256__
void serviceMS(void)
#else
#endif
{
	uint32_t returnVar = 0;
	if( usTicks >= ( MAXTIMER + MAXINTERVAL ) )
	{
		returnVar = usTicks - MAXTIMER;

	}
	else
	{
		returnVar = usTicks + 1000;
	}
	usTicks = returnVar;
	usTicksLocked = 0;  //unlock
}

