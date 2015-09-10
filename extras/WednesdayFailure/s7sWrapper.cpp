#include "Wire.h"
#include "s7sWrapper.h"

//Constructor
S7sObject::S7sObject( byte inputAddress )
{
	i2cAddress = inputAddress;
}

// This custom function works somewhat like a serial.print.
//  You can send it an array of chars (string) and it'll print
//  the first 4 characters in the array.
void S7sObject::SendString(String toSend)
{
  Wire.beginTransmission(i2cAddress);
  for (int i=0; i<4; i++)
  {
    Wire.write(toSend[i]);
  }
  Wire.endTransmission();
}

// Send the clear display command (0x76)
//  This will clear the display and reset the cursor
void S7sObject::clear()
{
  Wire.beginTransmission(i2cAddress);
  Wire.write(0x76);  // Clear display command
  Wire.endTransmission();
}

// Set the displays brightness. Should receive byte with the value
//  to set the brightness to
//  dimmest------------->brightest
//     0--------127--------255
void S7sObject::setBrightness(byte value)
{
  Wire.beginTransmission(i2cAddress);
  Wire.write(0x7A);  // Set brightness command byte
  Wire.write(value);  // brightness data byte
  Wire.endTransmission();
}

// Turn on any, none, or all of the decimals.
//  The six lowest bits in the decimals parameter sets a decimal 
//  (or colon, or apostrophe) on or off. A 1 indicates on, 0 off.
//  [MSB] (X)(X)(Apos)(Colon)(Digit 4)(Digit 3)(Digit2)(Digit1)
void S7sObject::setDecimals(byte decimals)
{
  Wire.beginTransmission(i2cAddress);
  Wire.write(0x77);
  Wire.write(decimals);
  Wire.endTransmission();
}
