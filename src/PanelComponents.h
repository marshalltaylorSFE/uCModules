#ifndef PANEL_COMPONENTS_H
#define PANEL_COMPONENTS_H

#include "HardwareInterfaces.h"

class PanelComponent
{
public:
	PanelComponent( void ){};
	virtual bool hasFreshData( void ){
		return 0;
	}
	virtual void freshen( uint16_t msTickDelta );
protected:
};

class Button : public PanelComponent
{
public:
	void setHardware( GenericHardwareDescription * input );
	bool hasFreshData( void );
	void freshen( uint16_t msTickDelta );
	uint8_t getState( void );
private:
	GenericHardwareDescription * hardwareInterface;
};
#endif