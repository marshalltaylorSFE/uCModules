#include "LuxPanel.h"
#include "panelComponents.h"
#include "LightMeterPanelComponents.h"
#include "HardwareInterfaces.h"
#include "LightMeterHardwareInterfaces.h"
#include <Arduino.h>
#include "TeensyView.h"
#include <SparkFunTSL2561.h>

extern OLEDFunctions oled;

// Create an SFE_TSL2561 object, here called "light":
extern SFE_TSL2561 light;
extern boolean gain;     // Gain setting, 0 = X1, 1 = X16;
extern unsigned int ms;  // Integration ("shutter") time in milliseconds

const float fStopTable[24] = {1.4, 1.8, 2.0, 2.2, 2.5, 2.8, 3.2, 3.5, 4, 4.5, 5, 5.6, 6.3, 7.1, 8, 9, 10, 11, 13, 14, 16, 18, 20, 22};
const float exposureTable[12] = {2, 4, 8, 15, 30, 60, 125, 250, 500, 1000, 2000, 4000};
const float isoTable[7] = {100, 200, 400, 800, 1600, 3200, 6400};
uint8_t isoTableSize = 7;

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
	
	fStopSetting = 7;
	exposureSetting = 5;
	isoSetting = 2;
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
		//menu part
		oled.drawMenu("Lux", false, true );
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
			//oled.setCursor(0,8);
			// getData() returned true, communication was successful
			//oled.print(" data0: ");
			//oled.print(data0);
			//oled.setCursor(0,16);
			//oled.print(" data1: ");
			//oled.print(data1);
			double lux;    // Resulting lux value
			boolean good;  // True if neither sensor is saturated
			// Perform lux calculation:
			good = light.getLux(gain,ms,data0,data1,lux);
			// Print out the results:
			oled.setCursor(0,11);
			oled.print(" lux: ");
			oled.print(lux);
			oled.setCursor(90,11);
			if (good) oled.print("(good)");	else oled.print("      ");
		}
		else
		{
			drawDisplay = true;
			oled.setCursor(0,11);
			oled.print("ERROR");
		}


		break;
	case PDisplayPhotoValueInit:
		drawDisplay = true;
		oled.clear(PAGE);
		oled.drawMenu("Photo", isoTable[isoSetting], true, true );

		oled.drawBrackets();
		nextState = PDisplayPhotoValue;
		break;
	case PDisplayPhotoValue:
		if( downButton.serviceRisingEdge() )
		{
			nextState = PDisplayLuxValueInit;
		}
		else if( upButton.serviceRisingEdge() )
		{
			nextState = PSetFStopInit;
		}
		else
		{
			if (light.getData(data0,data1))
			{
				//get the lux
				double lux;    // Resulting lux value
				boolean good = light.getLux(gain,ms,data0,data1,lux);  // True if neither sensor is saturated
				// Print out the results:
				//oled.setCursor(0,11);
				//oled.print(" lux: ");
				//oled.print(lux);
				//oled.setCursor(90,11);
				//if (good) oled.print("(good)");	else oled.print("      ");
				float T = pow(fStopTable[fStopSetting],2)*64/(lux*(1/exposureTable[exposureSetting]));
				oled.setCursor(33,13);
				oled.print("         ");
				oled.setCursor(33,13);
				oled.print(T,2);
				drawDisplay = true;
			}
		}
		break;
	case PSetFStopInit:
		dataWheel.clearDiff();
		drawDisplay = true;
		oled.clear(PAGE);
		//menu part
		oled.drawMenu("FStop", isoTable[isoSetting], true, true );

		//Draw last value
		oled.setCursor(0,11);
		oled.print(fStopTable[fStopSetting], 2);
		nextState = PSetFStop;
		break;
	case PSetFStop:
		if( dataWheel.serviceChanged() )
		{
			drawDisplay = true;
			//Erase part
			oled.setCursor(0,11);
			oled.print("      ");
        
			if( dataWheel.getDiff() > 0 )
			{
				if( fStopSetting < 23 )
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
			oled.setCursor(0,11);
			oled.print(fStopTable[fStopSetting], 2);
        
		}
		if( downButton.serviceRisingEdge() )
		{
			nextState = PDisplayPhotoValueInit;
		}
		if( upButton.serviceRisingEdge() )
		{
			nextState = PSetISOInit;
		}
		break;
	case PSetISOInit:
		dataWheel.clearDiff();
		drawDisplay = true;
		oled.clear(PAGE);
		//menu part
		oled.drawMenu("ISO", true, true );

		//Draw last value
		oled.drawISOScale( isoSetting );
		
		nextState = PSetISO;
		break;
	case PSetISO:
		if( dataWheel.serviceChanged() )
		{
			drawDisplay = true;
			//Erase part
			oled.setCursor(0,11);
			oled.print("      ");
        
			if( dataWheel.getDiff() > 0 )
			{
				if( isoSetting < 6 )
				{
					isoSetting++;
				}
				dataWheel.removeDiff(8);
				
			}
			if( dataWheel.getDiff() < 0 )
			{
				if( isoSetting > 0 )
				{
					isoSetting--;
				}
				dataWheel.removeDiff(8);
				
			}
			//Draw new value
			oled.drawISOScale( isoSetting );
		}
		if( downButton.serviceRisingEdge() )
		{
			nextState = PSetFStopInit;
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
		//menu part
		oled.drawMenu("Exp", isoTable[isoSetting], true, false );
		
		//Draw last value
		oled.setCursor(0,11);
		oled.print("1/");
		oled.print((int)exposureTable[exposureSetting]);
		nextState = PSetExposure;
		break;
	case PSetExposure:
		if( dataWheel.serviceChanged() )
		{
			drawDisplay = true;
			//Erase part
			oled.setCursor(0,11);
			oled.print("      ");
        
			if( dataWheel.getDiff() > 0 )
			{
				if( exposureSetting < 11 )
				{
					exposureSetting++;
				}
				dataWheel.removeDiff(5);
				
			}
			if( dataWheel.getDiff() < 0 )
			{
				if( exposureSetting > 0 )
				{
					exposureSetting--;
				}
				dataWheel.removeDiff(5);
				
			}
			//Draw new value
			oled.setCursor(0,11);
			oled.print("1/");
			oled.print((int)exposureTable[exposureSetting]);
        
		}
		if( downButton.serviceRisingEdge() )
		{
			nextState = PSetISOInit;
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