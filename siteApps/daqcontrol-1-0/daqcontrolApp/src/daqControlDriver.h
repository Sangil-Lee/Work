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

#include "daqControlData.h"

using namespace std;

namespace daqControl
{
  class daqControlDriver
  {
    public:

      daqControlDriver(const int mode, const int devNum);
      ~daqControlDriver();

      daqControlDriver* getInstance();

//============================================================================================================================
    public:

    private:
  };
};
#endif
