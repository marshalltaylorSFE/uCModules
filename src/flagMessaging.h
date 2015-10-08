//**********************************************************************//
//
//  MIT License: http://opensource.org/licenses/MIT
//
//  Written by:  Marshall Taylor
//  Changelog (YYYY/MM/DD):
//    2015/09/20: Created
//    2015/10/08: Initial Release
//
//**********************************************************************//


#ifndef FLAGMESSAGING_H
#define FLAGMESSAGING_H
#include <stdint.h>

//---Generic Messaging-------------------------------------------
class MessagingFlag
{
public:
	MessagingFlag( void ); //Constructor
	void setFlag( void );
	void clearFlag( void );
	uint8_t getFlag( void );
	uint8_t serviceRisingEdge( void );
	uint8_t serviceFallingEdge( void );
	
private:
	uint8_t flag;
	uint8_t risingEdgeFlag;
	uint8_t fallingEdgeFlag;
};
#endif
