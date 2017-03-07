#include "Panel.h"
#include "HardwareInterfaces.h"
#include "Arduino.h"

Panel::Panel( void )
{
	
}

void Panel::freshenComponents( uint16_t msTicksDelta )
{
	//call all contained objects' freshen
	ListObject * tempObject = componentList;
	while( tempObject != NULL )
	{
		tempObject->component->freshen( msTicksDelta );
		tempObject = tempObject->next;
	}
}

void Panel::add( PanelComponent * inputComp )
{
	//Add the component to the linked list
	ListObject * tempPointer = lastComponent();
	if( tempPointer == NULL )
	{
		//There is no item, make it
		componentList = new ListObject;
		//And fill it
		componentList->component = inputComp;
		//Now the top level object 'componentList' points to the first component
	}
	else
	{
		tempPointer->next = new ListObject;
		tempPointer = tempPointer->next;
		tempPointer->component = inputComp;
	}
	
}

ListObject * Panel::lastComponent( void )
{
	ListObject * tempObject = componentList;
	if( tempObject != NULL )
	{
		while( tempObject->next != NULL )
		{
			tempObject = tempObject->next;
		}
	}
	return tempObject;
}