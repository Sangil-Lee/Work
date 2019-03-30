//Wrapping class for system calls of timing device driver.
#include <stdlib.h>
#include <termios.h>
#include "daqControlDriver.h"


#define NUM_REGS_MAX 350
//#define D_TestMode_AXICLOCK

namespace daqControl{

//----===@ daqControl
// Parameters  :
// Description :
//==============================================================================
daqControlDriver::daqControlDriver(const int mode, const int devNum)
{
}

//==============================================================================
//----===@ ~daqControlDriver
// Parameters  :
// Description :
//==============================================================================
daqControlDriver::~daqControlDriver()
{
	prnM2("~daqControlDriver\n");
}

//==============================================================================
//----===@ ~daqControlDriver
// Parameters  :
// Description :
//==============================================================================
daqControlDriver* daqControlDriver::getInstance()
{
	return this;
}


} //name space end




