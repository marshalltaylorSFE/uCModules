#ifndef PANEL_H
#define PANEL_H

#include "PanelComponents.h"
#include "HardwareInterfaces.h"
//ListObject contains a reference to a PanelComponent, and the next one.
class ListObject
{
public:
	ListObject(){
		component = NULL;
		next = NULL;
	}
	PanelComponent * component;
	ListObject * next;
};

//Panel should never always be inherited by a operationally named class
// that has panelComponents as members and configures them.
//
//	{ ...
//		private:	
//		Button upButton;
//	}
//
//
// ctor says:
//		pinMode(22, INPUT_PULLUP);
//		upButton.setHardware(new ArduinoDigitalIn( 22 ));
//		add( &upButton );
//
// Only then can updateComponents be called.
//
// The subclass should define operation in something like the following:
//
//	void tickStateMachine( int msTicksDelta );
//
class Panel
{
public:
	Panel();
	void add( PanelComponent * inputComp );
protected:
	void freshenComponents( uint16_t msTicksDelta );
	//This is a simple linked list for the panel components.
	//Used to update objects.
	ListObject * componentList;
	ListObject * lastComponent( void );
	
};
#endif