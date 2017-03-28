#include "Panel.h"
#include "HardwareInterfaces.h"
#include "Arduino.h"

extern int freeRam();
extern void barfOutStackPointer();

Panel::Panel( void )
{
	
}

void Panel::freshenComponents( uint16_t msTicksDelta )
{
Serial.println(" A");
Serial.println(freeRam());
barfOutStackPointer();
	ListObject * tempObject = componentList;
	if( tempObject == NULL ) return;
	//Call the static update of PanelComponent
	//tempObject->PanelComponent->freshenStatic( msTicksDelta );
	PanelComponent::freshenStatic( msTicksDelta );
	//call all contained objects' freshen
Serial.println(" B");
	while( tempObject != NULL )
	{
Serial.println(" C");
		tempObject->component->freshen( msTicksDelta );
Serial.println(" D");
		tempObject = tempObject->next;
Serial.println(" E");
	}
Serial.println(" F");
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