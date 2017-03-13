#ifndef LUX_PANEL_H
#define LUX_PANEL_H

#include "Panel.h"
//#include "PanelComponents.h"  //included by panel
//#include "HardwareInterfaces.h"  //included by panel
#include "LightMeterPanelComponents.h"
#include "TeensyView.h"

extern const float fStopTable[];
extern const float exposureTable[];
extern const float isoTable[];
extern uint8_t isoTableSize;

enum PState_t
{
	PInit,
	PDisplayLuxValueInit,
	PDisplayLuxValue,
	PDisplayPhotoValueInit,
	PDisplayPhotoValue,
	PSetFStopInit,
	PSetFStop,
	PSetISOInit,
	PSetISO,
	PSetExposureInit,
	PSetExposure
};

class LuxPanel : public Panel
{
public:
	LuxPanel( void );
	void tickStateMachine( int msTicksDelta );

private:
	Button upButton;
	Button downButton;
	Button encButton;
	RotoryEncoder dataWheel;
	
	PState_t state;
	uint8_t fStopSetting;
	uint8_t exposureSetting;
	uint8_t isoSetting;
	
};

class OLEDFunctions : public TeensyView
{
public:
	OLEDFunctions(uint8_t rst, uint8_t dc, uint8_t cs, uint8_t sck, uint8_t mosi) : TeensyView(rst, dc, cs, sck, mosi) {};//(uint8_t rst, uint8_t dc, uint8_t cs, uint8_t sck, uint8_t mosi)
	void drawByte(uint8_t x, uint8_t y, uint8_t c)//, uint8_t color, uint8_t mode);
	{
		uint8_t color = WHITE;
		uint8_t mode = NORM;
		for (int i =0;i<8;i++)
		{			// 8 is the LCD's page height (see datasheet for explanation)
			if (c & 0x1)
			{
				pixel(x, y+7-i, color,mode);
			}
			else
			{
				pixel(x, y+7-i, !color,mode);
			}
			c >>=1;
		}
	};
	void drawSeperator1( void ){
		line(0,8,127,8);
	};
	void drawRightArrow( void ){
		drawByte(122,0,0x10);
		drawByte(123,0,0x10);
		drawByte(124,0,0x7C);
		drawByte(125,0,0x38);
		drawByte(126,0,0x10);
	};
	void drawLeftArrow( void ){
		drawByte(0,0,0x10);
		drawByte(1,0,0x38);
		drawByte(2,0,0x7C);
		drawByte(3,0,0x10);
		drawByte(4,0,0x10);
	};
	void drawLeftBracket( uint8_t xIn, uint8_t yIn ){
		drawByte(xIn + 2,yIn + 0,0x7F);
		drawByte(xIn + 3,yIn + 0,0xFF);
		drawByte(xIn + 4,yIn + 0,0xC0);
		drawByte(xIn + 5,yIn + 0,0xC0);

		drawByte(xIn + 0,yIn + 8,0x20);
		drawByte(xIn + 1,yIn + 8,0x70);
		drawByte(xIn + 2,yIn + 8,0xFF);
		drawByte(xIn + 3,yIn + 8,0xDF);

		drawByte(xIn + 2,yIn + 16,0xF0);
		drawByte(xIn + 3,yIn + 16,0xF8);
		drawByte(xIn + 4,yIn + 16,0x18);
		drawByte(xIn + 5,yIn + 16,0x18);
	};
	void drawRightBracket( uint8_t xIn, uint8_t yIn ){
		drawByte(xIn + 0,yIn + 0,0xC0);
		drawByte(xIn + 1,yIn + 0,0xC0);
		drawByte(xIn + 2,yIn + 0,0xFF);
		drawByte(xIn + 3,yIn + 0,0x7F);

		drawByte(xIn + 2,yIn + 8,0xDF);
		drawByte(xIn + 3,yIn + 8,0xFF);
		drawByte(xIn + 4,yIn + 8,0x70);
		drawByte(xIn + 5,yIn + 8,0x20);

		drawByte(xIn + 0,yIn + 16,0x18);
		drawByte(xIn + 1,yIn + 16,0x18);
		drawByte(xIn + 2,yIn + 16,0xF8);
		drawByte(xIn + 3,yIn + 16,0xF0);
	};
	void eraseArrows( void ){
		rectFill(0,0,7,8,BLACK,NORM); //x,y,w,h,c,m
		rectFill(121,0,7,8,BLACK,NORM); //x,y,w,h,c,m
	};
	void eraseMenu( void ){
		rectFill(7,0,115,8,BLACK,NORM); //x,y,w,h,c,m
	};
	void eraseBracketArea( void ){
		rectFill(31,10,91,21,BLACK,NORM); //x,y,w,h,c,m
	};
	void eraseTypeArea( void ){
		rectFill(0,10,25,21,BLACK,NORM); //x,y,w,h,c,m
	};
	void eraseLowerArea( void ){
		rectFill(0,9,128,22,BLACK,NORM); //x,y,w,h,c,m
	};
	void eraseOuterLowerArea( void ){
		rectFill(0,9,24,30,BLACK,NORM); //x,y,w,h,c,m
	};
	void drawMenu(const char *name, bool lArrow, bool rArrow )
	{
		setCursor(10,0);
		print(name);
		eraseArrows();
		if( lArrow ) drawLeftArrow();
		if( rArrow ) drawRightArrow();
		drawSeperator1();
	};
	void drawMenu(const char *name, uint16_t isoValue, bool lArrow, bool rArrow )
	{
		drawMenu(name, lArrow, rArrow );
		setCursor(71,0);
		print("ISO");
		setCursor(88,0);
		print(":");
		setCursor(92,0);
		print(isoValue);
	};
	void drawISOScale( uint8_t isoSetting )
	{
		uint8_t x1, x2, x3;
		eraseLowerArea();
		if( isoTable[isoSetting] < 1000 )
		{
			x2 = 53;
			x3 = x2 + 28;
		}
		else
		{
			x2 = 48;
			x3 = x2 + 37;
		}
		setFontType(1); 
		setCursor(x2,15);
		print(isoTable[isoSetting], 0);
		setFontType(0);
		if( isoSetting < (isoTableSize - 1) )
		{
			//draw next higher value
			setCursor( x3, 19 );
			print( isoTable[isoSetting + 1], 0 );
		}
		if( isoSetting > 0 )
		{
			if( isoTable[isoSetting - 1] < 1000 )
			{
				x1 = x2 - 20;
			}
			else
			{
				x1 = x2 - 26;
			}
			//draw next lower value
			setCursor( x1, 19 );
			print( isoTable[isoSetting - 1], 0 );
		}
	};
	void drawExposureStyle1( float exposureTime, uint8_t xIn, uint8_t yIn )
	{	
		
		if( exposureTime < 1 )
		{
			setCursor(xIn,yIn);
			print("1");
			line(xIn + 4, yIn + 10, xIn + 9, yIn + 2, WHITE, NORM);
			setCursor(xIn + 10,yIn + 5);
			print(1 / exposureTime, 0 );
		}
		else
		{
			setCursor(xIn,yIn + 3);
			print(exposureTime,1);
		}
	};
	void drawFNumStyle1( float fNumIn, uint8_t xIn, uint8_t yIn )
	{	
		setCursor(xIn,yIn + 4);
		print("F");
		setFontType(1);
		uint16_t whole = (uint16_t)fNumIn;
		float partial = ( fNumIn - whole );
		if( partial > 0 )
		{
			//Print 1 dec
			setCursor(xIn + 8,yIn);
			print( whole );
			setCursor(xIn + 18,yIn);
			print( partial * 10, 0 );
			pixel(xIn + 15,yIn + 10, WHITE,NORM);
			pixel(xIn + 15,yIn + 11, WHITE,NORM);
			pixel(xIn + 16,yIn + 10, WHITE,NORM);
			pixel(xIn + 16,yIn + 11, WHITE,NORM);
		}
		else
		{
			//Print whole
			setCursor(xIn + 8,yIn);
			print( whole );
		}
		setFontType(0);
	};
};

#endif