// This is a template for using the uCModules panel stuff to make a program.
// The example makes some components and basically does nothing.
// To make your own,
// 
// 1. Copy all three files into a new project folder
// 2. Rename the .ino to match the folder
// 3. Rename CustomPanel.cpp and .h to something regarding your project
//     (ex: DoorbellPanel)
//     Also change #include
// 4. Modify the custom panel:
//   a. Rename guards
//   b. Change class name in both files
// 5. Change the type of myCustomPanel to your new name
//     (Give panel useful name here too)
// 
// Now the code should compile.  Go on,
// 
// 6. Add components within the new panel's class (DoorbellPanel.h)
// 7. Initialize these components inside the new panel (DoorbellPanel::DoorbellPanel())
// 8. Add custom variables to the panel and initialize inside constructor
// 9. List desired states in header file (PStates)
// 10. Expand state machine with new state logic.


#include "CustomPanel.h" //Edit and rename this

CustomPanel myCustomPanel;

uint16_t debugCounter = 0;

void setup()
{
	Serial.begin(115200);

	//Go to fresh state
	myCustomPanel.reset();
	
}

//Set LOOP_DELAY to length of time between ticks of everything (in ms)
#define LOOP_DELAY 10

void loop()
{
	//Tick the machine
	myCustomPanel.tickStateMachine(LOOP_DELAY);

	//Delay the loop
	delay(LOOP_DELAY);
	
	//Increment a counter for debug stuff
	debugCounter = debugCounter + LOOP_DELAY;
	if( debugCounter > 1000 )
	{
		//Do debug stuff
		Serial.println(myCustomPanel.getState());
		debugCounter = 0;
	}
	
}