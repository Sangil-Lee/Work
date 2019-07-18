#ifndef   _LABVIEW_FPGA_DRIVER_H
#define   _LABVIEW_FPGA_DRIVER_H

#include <cstdio>
#include <string>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/ioctl.h>

#include "NiFpga.h"
#include "NiFpga_FPGA_1.h"

using namespace std;

namespace lvfpga
{
  class lvfpgaDriver 
  {
    public:
	  lvfpgaDriver(const char* bitfile, const char* signature, const char* resource);

      ~lvfpgaDriver();

      lvfpgaDriver* GetInstance();
//============================================================================================================================
    public:

	//Read Functions
	NiFpga_Status ReadBool(uint32_t indicator, NiFpga_Bool* value);
	NiFpga_Status ReadI8(uint32_t   indicator, int8_t* value);
	NiFpga_Status ReadU8(uint32_t   indicator, uint8_t* value);
	NiFpga_Status ReadI16(uint32_t  indicator, int16_t* value);
	NiFpga_Status ReadU16(uint32_t	indicator, uint16_t* value);
	NiFpga_Status ReadI32(uint32_t	indicator, int32_t* value);
	NiFpga_Status ReadU32(uint32_t	indicator, uint32_t* value);
	NiFpga_Status ReadI64(uint32_t	indicator, int64_t* value);
	NiFpga_Status ReadU64(uint32_t	indicator, uint64_t* value);
	NiFpga_Status ReadSgl(uint32_t	indicator, float* value);
	NiFpga_Status ReadDbl(uint32_t	indicator, double* value);
	NiFpga_Status ReadArrayBool(uint32_t indicator, NiFpga_Bool* value, size_t length);
	NiFpga_Status ReadArrayI8(uint32_t indicator, int8_t* value, size_t length);
	NiFpga_Status ReadArrayU8(uint32_t indicator, uint8_t* value, size_t length);
	NiFpga_Status ReadArrayI16(uint32_t indicator, int16_t* value, size_t length);
	NiFpga_Status ReadArrayU16(uint32_t indicator, uint16_t* value, size_t length);
	NiFpga_Status ReadArrayI32(uint32_t indicator, int32_t* value, size_t length);
	NiFpga_Status ReadArrayU32(uint32_t indicator, uint32_t* value, size_t length);
	NiFpga_Status ReadArrayI64(uint32_t indicator, int64_t* value, size_t length);
	NiFpga_Status ReadArrayU64(uint32_t indicator, uint64_t* value, size_t length);
	NiFpga_Status ReadArraySgl(uint32_t indicator, float* value, size_t length);
	NiFpga_Status ReadArrayDbl(uint32_t indicator, double* value, size_t length);

	//Write Functions
	NiFpga_Status WriteBool(uint32_t  control,  NiFpga_Bool value);
	NiFpga_Status WriteI8(uint32_t    control,  int8_t value);
	NiFpga_Status WriteU8(uint32_t    control,  uint8_t value);
	NiFpga_Status WriteI16(uint32_t   control,  int16_t value);
	NiFpga_Status WriteU16(uint32_t   control,  uint16_t value);
	NiFpga_Status WriteI32(uint32_t   control,  int32_t value);
	NiFpga_Status WriteU32(uint32_t   control,  uint32_t value);
	NiFpga_Status WriteI64(uint32_t   control,  int64_t value);
	NiFpga_Status WriteU64(uint32_t   control,  uint64_t value);
	NiFpga_Status WriteSgl(uint32_t   control,  float value);
	NiFpga_Status WriteDbl(uint32_t   control,  double value);
	NiFpga_Status WriteArrayBool(uint32_t control, NiFpga_Bool* value, size_t length);
	NiFpga_Status WriteArrayI8(uint32_t control, int8_t* value, size_t length);
	NiFpga_Status WriteArrayU8(uint32_t control, uint8_t* value, size_t length);
	NiFpga_Status WriteArrayI16(uint32_t control, int16_t* value, size_t length);
	NiFpga_Status WriteArrayU16(uint32_t control, uint16_t* value, size_t length);
	NiFpga_Status WriteArrayI32(uint32_t control, int32_t* value, size_t length);
	NiFpga_Status WriteArrayU32(uint32_t control, uint32_t* value, size_t length);
	NiFpga_Status WriteArrayI64(uint32_t control, int64_t* value, size_t length);
	NiFpga_Status WriteArrayU64(uint32_t control, uint64_t* value, size_t length);
	NiFpga_Status WriteArraySgl(uint32_t control, float* value, size_t length);
	NiFpga_Status WriteArrayDbl(uint32_t control, double* value, size_t length);

	private:
	NiFpga_Session gSession;

//============================================================================================================================
      //Common

  };
};
#endif
