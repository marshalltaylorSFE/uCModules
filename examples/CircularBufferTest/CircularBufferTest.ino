#include "CircularBuffer.h"

CircularBuffer<int> myBuffer(10);
int fakeo = 0;

int count = 0;

void setup()
{
	Serial.begin(115200);
	Serial.println("Program Started");
}

void loop()
{
	Serial.print("Adding number: ");
	Serial.println(count);
	myBuffer.write(count);
	
	fakeo = 0;
	Serial.println("Printing complete list:");
	for(int i = 0; i < 10; i++)
	{
		Serial.print("i = ");
		Serial.print(i);
		Serial.print(", value = ");
		Serial.println(myBuffer.read(i));
	}
	
	count++;
	Serial.println();
	delay(2500);
}