#ifndef   _ACQ_FPS_REGISTER_MAP_H
#define   _ACQ_FPS_REGISTER_MAP_H

#include <cstdio>
#include <string>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/ioctl.h>

#include "acqFPSData.h"

using namespace std;

namespace acq_fps
{
  class acqFPSDriver 
  {
    public:

      acqFPSDriver(const int mode, const int devNum);
      ~acqFPSDriver();

	acqFPSDriver* GetInstance();
    private:
  };

};
#endif
