#include "LuxPanel.h"
LuxPanel myLuxPanel;

void setup()
{
	Serial.begin(115200);
}

void loop()
{
	delay(1000);
	myLuxPanel.tickStateMachine(10);
	Serial.println("End of loop");
}