#ifndef   _TIMING_REGISTER_MAP_H
#define   _TIMING_REGISTER_MAP_H

#include <cstdio>
#include <string>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <NIDAQmx.h>

using namespace std;

namespace nidaq
{
  class nidaqDriver 
  {
    public:

	  nidaqDriver(const string device,const float64 minVal=0.0, const float64 maxVal=10.0, const float64 slope = 1.0, 
			  const float64 offset = 0.0, const int32 scale = 0, const int32 moduletype = 0, const int32 termConfig=DAQmx_Val_RSE,const float64 samplRate = 10000.0);

	  nidaqDriver(const string device, float64 scaleparameter[], const int32 scale = 0,  const int32 moduletype = 0, const int32 termConfig=DAQmx_Val_RSE,const float64 samplRate = 10000.0);

      ~nidaqDriver();

      nidaqDriver* GetInstance();

	  int ReadAnalog(float64 readArray[], int32 *chanRead, const int32 numSampsPerChan = 1000, const float64 timeout = 10.0);

//============================================================================================================================
    public:
      double firmware;
      double software;

	private:
		TaskHandle  g_taskHandle;
		enum e_scaleType{LinearScale=0, Polynomial, RangeMap, MapTable };

		int setScale(e_scaleType scaletype, const string userScaleName, const float64 slope = 1.0, const float64 yIntercept = 0.0, const int32 prescaleunit = DAQmx_Val_Volts, const string scaledunit = "volts");
		int createAIVoltChannel(const string device, const float64 minVal = 0.0, const float64 maxVal = 10.0, const int32 termConfig = DAQmx_Val_RSE, const int32 units = DAQmx_Val_Volts, const string scaleUnits = "\0" );
		int configSampleClock(const float64 sampRate = 10000.0, const int32 activeEdge=DAQmx_Val_Rising, const uInt64 buffsize = 1000);
		void compileAPITest();

//============================================================================================================================
      //Common

  };
};
#endif
