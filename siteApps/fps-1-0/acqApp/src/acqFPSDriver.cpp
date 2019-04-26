//Wrapping class for system calls of acquisition FPS device driver.
#include <stdlib.h>
#include "acqFPSDriver.h"

#define NUM_REGS_MAX 350

namespace acq_fps
{
//==============================================================================
//----===@ acqFPSDriver
// Parameters  :
// Description :
//==============================================================================
  acqFPSDriver::acqFPSDriver(const int mode, const int devNum)
  {
  }

//==============================================================================
//----===@ ~acqFPSDriver
// Parameters  :
// Description :
//==============================================================================
  acqFPSDriver::~acqFPSDriver()
  {
  }

  acqFPSDriver* acqFPSDriver::GetInstance()
  {
    return this;
  }


} //name space end




