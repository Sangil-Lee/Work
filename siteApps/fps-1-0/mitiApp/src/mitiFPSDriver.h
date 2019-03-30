#ifndef   _TIMING_REGISTER_MAP_H
#define   _TIMING_REGISTER_MAP_H

#include <cstdio>
#include <string>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/ioctl.h>

//#include "mitiFPSData.h"

using namespace std;

namespace miti_fps
{
  class mitiFPSDriver 
  {
    public:

      mitiFPSDriver(const int mode, const int devNum);
      ~mitiFPSDriver();

	mitiFPSDriver* GetInstance();
    private:
  };

};
#endif
