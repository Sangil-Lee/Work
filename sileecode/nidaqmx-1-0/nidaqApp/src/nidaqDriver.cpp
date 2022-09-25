//Wrapping class for system calls of nidaq device driver.

#include <stdlib.h>
//#include <termios.h>

#include "nidaqDriver.h"


namespace nidaq
{

int32 CVICALLBACK EveryNCallback(TaskHandle taskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void *callbackData);
int32 CVICALLBACK DoneCallback(TaskHandle taskHandle, int32 status, void *callbackData);
	//==============================================================================
	//----===@ nidaqDriver
	// Parameters  :
	// Description :
	//==============================================================================
	nidaqDriver::nidaqDriver(const string device, float64 scaleparameter[],
			  const int32 scale,const int32 moduletype, const int32 termConfig,const float64 samplRate):software(0.1),g_taskHandle(0)
	{
		/*********************************************/
		// DAQmx Configure Code
		/*********************************************/
		DAQmxCreateTask("",&g_taskHandle);

		char devNames[60];
		int32 sts= DAQmxGetSysDevNames(devNames, sizeof(devNames));
		printf("Device Names: %s\n", devNames);

		if(moduletype == 0)
		{
			if(scale)
			{
				createAIVoltChannel(device, scaleparameter[0], scaleparameter[1], DAQmx_Val_RSE, DAQmx_Val_FromCustomScale, "UserLineScale");

				configSampleClock();
				setScale(nidaqDriver::LinearScale, "UserLineScale", scaleparameter[2], scaleparameter[3], DAQmx_Val_Volts, "bar");
			}
			else
			{
				createAIVoltChannel(device, scaleparameter[0], scaleparameter[1]);
				//Default Sampling Clock Timing.
				configSampleClock();
			};
		}
		else if(moduletype == 1 )
		{
			//Current Module
			printf("For future, will be developed ...\n");
		};


		/*********************************************/
		// DAQmx Start Code
		/*********************************************/
		DAQmxStartTask(g_taskHandle);

	};

	//==============================================================================
	//----===@ ~nidaqDriver
	// Parameters  :
	// Description :
	//==============================================================================
	nidaqDriver::~nidaqDriver()
	{
		char        errBuff[2048]={'\0'};
		int32       error = 0;

		if( DAQmxFailed(error) )
			DAQmxGetExtendedErrorInfo(errBuff,2048);

		if( g_taskHandle!=0 )  {
			/*********************************************/
			// DAQmx Stop Code
			/*********************************************/
			DAQmxStopTask(g_taskHandle);
			DAQmxClearTask(g_taskHandle);
		};

		if( DAQmxFailed(error) )
			printf("DAQmx Error: %s\n",errBuff);
	}

	//==============================================================================
	//----===@ ~nidaqDriver
	// Parameters  :
	// Description :
	//==============================================================================
	nidaqDriver* nidaqDriver::GetInstance()
	{
		return this;
	};


	int nidaqDriver::createAIVoltChannel(const string device, const float64 minVal, const float64 maxVal, const int32 termConfig, const int32 units, const string scaleUnits )
	{
		//int32 DAQmxCreateAIVoltageChan (TaskHandle taskHandle, const char physicalChannel[], const char nameToAssignToChannel[], int32 terminalConfig, float64 minVal, float64 maxVal, int32 units, const char customScaleName[]);
		//return(DAQmxCreateAIVoltageChan(g_taskHandle, device.c_str(), "", termConfig, minVal, maxVal, DAQmx_Val_Volts, NULL));
		return(DAQmxCreateAIVoltageChan(g_taskHandle, device.c_str(), "", termConfig, minVal, maxVal, units, scaleUnits.c_str()));
	};

	int nidaqDriver::createDIChannel(const string device)
	{
		return(DAQmxCreateDIChan(g_taskHandle,device.c_str(),"", DAQmx_Val_ChanForAllLines));
	};

	int nidaqDriver::configSampleClock(const float64 sampRate, const int32 activeEdge, const uInt64 buffsize)
	{
		//int32 DAQmxCfgSampClkTiming (TaskHandle taskHandle, const char source[], float64 rate, int32 activeEdge, int32 sampleMode, uInt64 sampsPerChanToAcquire);
		return (DAQmxCfgSampClkTiming(g_taskHandle, "", sampRate, activeEdge, DAQmx_Val_ContSamps, buffsize));
	};

	int nidaqDriver::ReadAnalog(float64 readArray[], int32 *chanRead, const int32 numSampsPerChan, const float64 timeout)
	{
		/*********************************************/
		// DAQmx Read Code
		/*********************************************/

		/*********************************************/
		//DAQmxReadAnalogF64 (TaskHandle taskHandle, int32 numSampsPerChan, float64 timeout, bool32 fillMode, float64 readArray[], uInt32 arraySizeInSamps, int32 *sampsPerChanRead, bool32 *reserved);
		//DAQmxReadAnalogF64(taskHandle,1000,10.0,DAQmx_Val_GroupByChannel,data,1000,&read,NULL);
		/*********************************************/

		int status = DAQmxReadAnalogF64(g_taskHandle, numSampsPerChan, timeout, DAQmx_Val_GroupByChannel, readArray, (uInt32)numSampsPerChan, chanRead, NULL);

		return (status);

	};

	int nidaqDriver::ReadDigital(uInt32 *readData, int32 *chanRead)
	{
		int status = DAQmxReadDigitalU32(g_taskHandle,1, 10.0, DAQmx_Val_GroupByChannel, readData, 1, chanRead, NULL);
		return (status);
	};

	int nidaqDriver::setScale(e_scaleType scaletype, const string userScaleName, const float64 slope, const float64 yIntercept, const int32 prescaleunit, const string scaledunit)
	{
		//y = mx + b, (y=scaled value, x=raw value(0 ~ 10 volts), m = slope, b = y intercept (offset))
		//min ~ max (if m = 10, b = 5 -> max( y = 10 x 10 = 100), min (y = 0 x 10 = 0), b = not realated for min/max value)
		//CreateAIVoltChannel(device, termConfig, min_val, max_val, DAQmx_Val_FromCustomScale, "UserLineScale");
#if 1
		switch (scaletype)
		{
				//DAQmxCreateLinScale(const char name[], float64 slope, float64 yIntercept, int32 preScaledUnits, const char scaledUnit[]);
				//
			case LinearScale:
			default:
				DAQmxCreateLinScale(userScaleName.c_str(), slope, yIntercept, prescaleunit, scaledunit.c_str());
				break;
			case RangeMap:
				//Raw value 0~10 to 0~100
				DAQmxCreateMapScale(userScaleName.c_str(), 0.0, 10.0, 0.0, 100.0, prescaleunit, scaledunit.c_str());
				break;
			case Polynomial:
				{
					float64 forwardCoeff[3];	//3-order polynomial y = a2*x^2 + a1*x + a0, y = 4 * x^2 + 3 * x + 1
					forwardCoeff[0] = 1; 
					forwardCoeff[1] = 3; 
					forwardCoeff[2] = 4; 

					float64 reverseCoeff[3];
					DAQmxCalculateReversePolyCoeff(forwardCoeff, 3, 0.0, 10.0, 1000, -1, reverseCoeff);
					DAQmxCreatePolynomialScale(userScaleName.c_str(), forwardCoeff, 3, reverseCoeff,3, prescaleunit, scaledunit.c_str());
				};
				break;
			case MapTable:
				{
					float64 preScaledVal[3];	//3-order polynomial y = a2*x^2 + a1*x + a0, y = 4 * x^2 + 3 * x + 1
					preScaledVal[0] = 1; 
					preScaledVal[1] = 3; 
					preScaledVal[2] = 4; 

					float64 scaledVal[3];	//3-order polynomial y = a2*x^2 + a1*x + a0, y = 4 * x^2 + 3 * x + 1
					scaledVal[0] = 10.23; 
					scaledVal[1] = 12.23; 
					scaledVal[2] = 13.23; 

					DAQmxCreateTableScale(userScaleName.c_str(), preScaledVal, 3, scaledVal, 3, prescaleunit, scaledunit.c_str());
				
				};
				break;
		};
#else

		DAQmxCreateLinScale("UserLineScale", 10/*slope*/, 5/*y intercepts*/, DAQmx_Val_Volts, "bar");

#endif
		return 0;

	};

	void nidaqDriver::compileAPITest()
	{
		//DAQ MX Current Module
		DAQmxCreateAICurrentChan(g_taskHandle,"SC1Mod1/ai0","",DAQmx_Val_Cfg_Default,0.0,0.02,DAQmx_Val_Amps,DAQmx_Val_Default,249.0,"");
		DAQmxCfgSampClkTiming(g_taskHandle,"",1000.0,DAQmx_Val_Rising,DAQmx_Val_ContSamps,1000);

		DAQmxRegisterEveryNSamplesEvent(g_taskHandle,DAQmx_Val_Acquired_Into_Buffer,1000,0,EveryNCallback,NULL);
		DAQmxRegisterDoneEvent(g_taskHandle,0,DoneCallback,NULL);
	};

int32 CVICALLBACK EveryNCallback(TaskHandle taskHandle, int32 everyNsamplesEventType, uInt32 nSamples, void *callbackData)
{
	static int  totalRead=0;
	int32       read=0;
	float64     data[1000];

	/*********************************************/
	// DAQmx Read Code
	/*********************************************/
	DAQmxReadAnalogF64(taskHandle,1000,10.0,DAQmx_Val_GroupByScanNumber,data,1000,&read,NULL);
	if( read>0 ) {
		printf("Acquired %d samples. Total %d\r",(int)read,(int)(totalRead+=read));
		fflush(stdout);
	}
	return 0;
}

int32 CVICALLBACK DoneCallback(TaskHandle taskHandle, int32 status, void *callbackData)
{
	// Check to see if an error stopped the task.
	return 0;
}

}; //name space end




