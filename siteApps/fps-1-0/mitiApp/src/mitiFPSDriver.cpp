//Wrapping class for system calls of mitigation FPS device driver.
#include <stdlib.h>
#include "mitiFPSDriver.h"

#define NUM_REGS_MAX 350

namespace miti_fps
{
//==============================================================================
//----===@ mitiFPSDriver
// Parameters  :
// Description :
//==============================================================================
  mitiFPSDriver::mitiFPSDriver(const int mode, const int devNum)
  {
  }

//==============================================================================
//----===@ ~mitiFPSDriver
// Parameters  :
// Description :
//==============================================================================
  mitiFPSDriver::~mitiFPSDriver()
  {
  }

  mitiFPSDriver* mitiFPSDriver::GetInstance()
  {
    return this;
  }


} //name space end




