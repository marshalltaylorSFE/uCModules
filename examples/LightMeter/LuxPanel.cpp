#include "LuxPanel.h"
#include "panelComponents.h"
#include "LightMeterPanelComponents.h"
#include "HardwareInterfaces.h"
#include "LightMeterHardwareInterfaces.h"
#include <Arduino.h>
#include "TeensyView.h"
#include <SparkFunTSL2561.h>

extern TeensyView oled;

// Create an SFE_TSL2561 object, here called "light":
extern SFE_TSL2561 light;
extern boolean gain;     // Gain setting, 0 = X1, 1 = X16;
extern unsigned int ms;  // Integration ("shutter") time in milliseconds

const float fStopTable[15] = {1, 1.4, 2, 2.8, 4, 5.6, 8, 11, 16, 22, 32, 45, 64, 90, 128};
const float exposureTable[15] = {2, 4, 8, 15, 30, 60, 125, 250, 500, 1000, 2000, 4000,};


LuxPanel::LuxPanel( void )
{
	upButton.setHardware(new ArduinoDigitalIn( 22 ), 1);
	add( &upButton );
	
	downButton.setHardware(new ArduinoDigitalIn( 23 ), 1);
	add( &downButton );
	
	encButton.setHardware(new ArduinoDigitalIn( 4 ), 1);
	add( &encButton );

	dataWheel.setHardware(new TeensyEncoderIn( 8, 9 ) );
	add( &dataWheel );
	
	fStopSetting = 5;
	exposureSetting = 5;
	state = PInit;
	
}

void LuxPanel::tickStateMachine( int msTicksDelta )
{
	freshenComponents( msTicksDelta );
	
	bool drawDisplay = false;
	unsigned int data0, data1;
	
	//***** PROCESS THE LOGIC *****//
	//Now do the states.
	PState_t nextState = state;
	switch( state )
	{
	case PInit:
		//Can't be running, if button pressed move on
		oled.clear(PAGE);
		drawDisplay = true;
		nextState = PDisplayLuxValueInit;
		break;
	case PDisplayLuxValueInit:
		drawDisplay = true;
		oled.clear(PAGE);
		oled.setCursor(0,0);
		oled.print("Lux");
		Serial.print("Lux");
		nextState = PDisplayLuxValue;
		break;
	case PDisplayLuxValue:
		if( upButton.serviceRisingEdge() )
		{
			nextState = PDisplayPhotoValueInit;
		}
		downButton.serviceRisingEdge();
		
		if (light.getData(data0,data1))
		{
			drawDisplay = true;
			oled.setCursor(0,8);
			// getData() returned true, communication was successful
			oled.print(" data0: ");
			oled.print(data0);
			oled.setCursor(0,16);
			oled.print(" data1: ");
			oled.print(data1);
			double lux;    // Resulting lux value
			boolean good;  // True if neither sensor is saturated
			// Perform lux calculation:
			good = light.getLux(gain,ms,data0,data1,lux);
			// Print out the results:
			oled.setCursor(0,24);
			oled.print(" lux: ");
			oled.print(lux);
			oled.setCursor(20,0);
			if (good) oled.print("(good)");	else oled.print("      ");
		}
		else
		{
			drawDisplay = true;
			oled.setCursor(0,8);
			oled.print(" ERROR ");
		}


		break;
	case PDisplayPhotoValueInit:
		drawDisplay = true;
		oled.clear(PAGE);
		oled.setCursor(0,0);
		oled.print("Photo");
		Serial.print("Photo");
		nextState = PDisplayPhotoValue;
		break;
	case PDisplayPhotoValue:
		if( downButton.serviceRisingEdge() )
		{
			nextState = PDisplayLuxValueInit;
		}
		if( upButton.serviceRisingEdge() )
		{
			nextState = PSetFStopInit;
		}
		break;
	case PSetFStopInit:
		dataWheel.clearDiff();
		drawDisplay = true;
		oled.clear(PAGE);
		oled.setCursor(0,0);
		oled.print("F-Stop");
		Serial.print("F-Stop");
		//Draw last value
		oled.setCursor(0,8);
		oled.print(fStopTable[fStopSetting], 2);
		nextState = PSetFStop;
		break;
	case PSetFStop:
		if( dataWheel.serviceChanged() )
		{
			drawDisplay = true;
			//Erase part
			oled.setCursor(0,8);
			oled.print("      ");
        
			if( dataWheel.getDiff() > 0 )
			{
				if( fStopSetting < 14 )
				{
					fStopSetting++;
				}
				dataWheel.removeDiff(7);
				
			}
			if( dataWheel.getDiff() < 0 )
			{
				if( fStopSetting > 0 )
				{
					fStopSetting--;
				}
				dataWheel.removeDiff(7);
				
			}
			//Draw new value
			oled.setCursor(0,8);
			oled.print(fStopTable[fStopSetting], 2);
        
		}
		if( downButton.serviceRisingEdge() )
		{
			nextState = PDisplayPhotoValueInit;
		}
		if( upButton.serviceRisingEdge() )
		{
			nextState = PSetExposureInit;
		}
		break;
	case PSetExposureInit:
		dataWheel.clearDiff();
		drawDisplay = true;
		oled.clear(PAGE);
		oled.setCursor(0,0);
		oled.print("Exp.");
		Serial.print("Exp.");
		//Draw last value
		oled.setCursor(0,8);
		oled.print("1/");
		oled.print((int)exposureTable[exposureSetting]);
		nextState = PSetExposure;
		break;
	case PSetExposure:
		if( dataWheel.serviceChanged() )
		{
			drawDisplay = true;
			//Erase part
			oled.setCursor(0,8);
			oled.print("      ");
        
			if( dataWheel.getDiff() > 0 )
			{
				if( exposureSetting < 11 )
				{
					exposureSetting++;
				}
				dataWheel.removeDiff(7);
				
			}
			if( dataWheel.getDiff() < 0 )
			{
				if( exposureSetting > 0 )
				{
					exposureSetting--;
				}
				dataWheel.removeDiff(7);
				
			}
			//Draw new value
			oled.setCursor(0,8);
			oled.print("1/");
			oled.print((int)exposureTable[exposureSetting]);
        
		}
		if( downButton.serviceRisingEdge() )
		{
			nextState = PSetFStopInit;
		}
		upButton.serviceRisingEdge();
		break;
	default:
		nextState = PInit;
		break;
	}
	state = nextState;
	
	if( drawDisplay )oled.display();
	
	
	
	
	
	
	
//	//Do logic here
//	//Serial.println(upButton.getState());
//	//Serial.println(downButton.getState());
//	if(upButton.serviceRisingEdge()) Serial.println("RisingEdge");
//	if(upButton.serviceFallingEdge()) Serial.println("FallingEdge");
//	if(upButton.serviceHoldRisingEdge()) Serial.println("HoldRisingEdge");
//	if(upButton.serviceHoldFallingEdge()) Serial.println("HoldFallingEdge");
//	
//	if(dataWheel.serviceChanged())
//	{
//		Serial.println( dataWheel.getDiff() );
//		dataWheel.removeDiff(1);
//		
//	}
}