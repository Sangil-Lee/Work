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

#include <NIDAQmx.h>

using namespace std;

namespace nidaq
{
  class nidaqDriver 
  {
    public:

	  nidaqDriver(const string device, const int32 termConfig=DAQmx_Val_RSE, const float64 minVal=0.0, const float64 maxVal=10.0, const float64 samplRate = 10000.0);
      ~nidaqDriver();
      // double GetFirmware(){return firmware;}
      // double GetSoftware(){return software;}
      // void SetFirmware(double fware){firmware = fware; printf("FWare:%f\n", firmware);}
      // void SetSoftware(double sware){software = sware;}

      nidaqDriver* GetInstance();

	  //int ReadAnalog();
	  //int ReadAnalog( const int32 numSampsPerChan, const float64 timeout, float64 readArray[], int32 *chanRead);
	  int ReadAnalog(float64 readArray[], int32 *chanRead, const int32 numSampsPerChan = 1000, const float64 timeout = 10.0);
	  int ConfigSampleClock(const float64 sampRate = 10000.0, const int32 activeEdge=DAQmx_Val_Rising, const uInt64 buffsize = 1000);
	  int CreateAIVoltChannel( const string device, const int32 termConfig = DAQmx_Val_RSE, const float64 minVal = 0.0, const float64 maxVal = 0.0, const int32 units = DAQmx_Val_Volts, const string scaleUnits = "\0" );

//============================================================================================================================
    public:
      double firmware;
      double software;

	private:
		TaskHandle  g_taskHandle;
		enum e_scaleType{LinearScale=0, Polynomial, RangeMap, MapTable };
		//int setScale(e_scaleType scaletype);
		int setScale(e_scaleType scaletype, const string userScaleName, const float64 slope = 1.0, const float64 yIntercept = 0.0, const int32 prescaleunit = DAQmx_Val_Volts, const string scaledunit = "volts");

//============================================================================================================================
      //Common

  };
};
#endif
