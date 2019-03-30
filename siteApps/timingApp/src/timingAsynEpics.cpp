#include <cstdlib>
#include <epicsTypes.h>
#include <epicsTime.h>
#include <epicsThread.h>
#include <epicsString.h>
#include <epicsTimer.h>
#include <epicsMutex.h>
#include <epicsEvent.h>
#include <registryFunction.h>
#include <epicsExport.h>
#include <iocsh.h>

#include "timingAsynEpics.h"

#define FREQUENCY 1000       /* Frequency in Hz */
#define AMPLITUDE 1.0        /* Plus and minus peaks of sin wave */
#define NUM_DIVISIONS 10     /* Number of scope divisions in X and Y */
#define MIN_UPDATE_TIME 0.02 /* Minimum update time, to prevent CPU saturation */

static void userProcess(void *drvPvt);

timingAsynEpics::timingAsynEpics(const char *portName, int maxSizeSnapshot, int maxSizeBufferMB, int clientMode, const char *deviceName)
   : asynPortDriver(portName, 
                    1, /* maxAddr */ 
                    (int)NUM_SCOPE_PARAMS,
                    asynInt32Mask | asynFloat64Mask | asynInt16ArrayMask | asynFloat64ArrayMask | asynEnumMask | asynDrvUserMask, /* Interface mask */
                    asynInt32Mask | asynFloat64Mask | asynInt16ArrayMask | asynFloat64ArrayMask | asynEnumMask,  /* Interrupt mask */
                    1, /* asynFlags.  This driver does not block and it is not multi-device, so flag is 0 */
                    1, /* Autoconnect */
                    0, /* Default priority */
                    0 /* Default stack size*/
										),driverName("timingAsynEpics")
{
    asynStatus status;
    const char *functionName = "timingAsynEpics";

    // Create database entries
    eventId_ = epicsEventCreate(epicsEventEmpty);  
    //pregmap = (struct TimingRegmap *)malloc(sizeof(TimingRegmap));
    //pregmap = (TimingRegmap *)calloc(1,sizeof(TimingRegmap));
    pregmap = new TimingRegmap();

    // Snapshots  
    //createParam(P_WaveformString1,           asynParamFloat64Array,  &P_Waveform1);

    // System list
    //createParam(P_RefClockSourceSelectString,    asynParamInt32,      &P_RefClockSourceSelect);
    createParam(P_RunString,                asynParamInt32,         &P_Run);
    createParam(P_UpdateTimeString,         asynParamFloat64,       &P_UpdateTime);

		//Default Initial Value
		setIntegerParam(P_Run, 0);
		setDoubleParam(P_UpdateTime, 0.5);

    if (system_init_ok==1) 
		{
				status = (asynStatus)(epicsThreadCreate("timingAsynEpicsUserTask",
										epicsThreadPriorityMedium,
										epicsThreadGetStackSize(epicsThreadStackMedium),
										(EPICSTHREADFUNC)::userProcess,
										this) == NULL);
				if (status) {
						printf("%s:%s: epicsThreadCreate failure\n", driverName, functionName);
						return;
				}
		} else {
				status = asynError;
		};

		//printf("Number:%d, Last:(%d), First(%d)\n", NUM_SCOPE_PARAMS,&LAST_SCOPE_COMMAND, &FIRST_SCOPE_COMMAND);

}

asynStatus timingAsynEpics::writeInt32(asynUser *pasynUser, epicsInt32 value)
{
    int function = pasynUser->reason;
    asynStatus status = asynSuccess;
    const char *paramName;
    const char* functionName = "writeInt32";

    /* Set the parameter in the parameter library. */
    status = (asynStatus) setIntegerParam(function, value);
    
    /* Fetch the parameter string name for possible use in debugging */
    getParamName(function, &paramName);

    if (function == P_Run) {
        /* If run was set then wake up the userProcess task */
        if (value) epicsEventSignal(eventId_);
    } else {
        /* All other parameters just get set in parameter list, no need to
         * act on them here */
    }
    
    /* Do callbacks so higher layers see any changes */
    status = (asynStatus) callParamCallbacks();
    
    if (status) 
        epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize, 
                  "%s:%s: status=%d, function=%d, name=%s, value=%d", 
                  driverName, functionName, status, function, paramName, value);
    else        
        asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
              "%s:%s: function=%d, name=%s, value=%d\n", 
              driverName, functionName, function, paramName, value);
    return status;
}

asynStatus timingAsynEpics::writeFloat64(asynUser *pasynUser, epicsFloat64 value)
{
    int function = pasynUser->reason;
    asynStatus status = asynSuccess;
    int run;
    const char *paramName;
    const char* functionName = "writeFloat64";

    /* Set the parameter in the parameter library. */
    status = (asynStatus) setDoubleParam(function, value);

    /* Fetch the parameter string name for possible use in debugging */
    getParamName(function, &paramName);

    if (function == P_UpdateTime) {
        /* Make sure the update time is valid. If not change it and put back in parameter library */
        if (value < MIN_UPDATE_TIME) {
            asynPrint(pasynUser, ASYN_TRACE_WARNING,
                "%s:%s: warning, update time too small, changed from %f to %f\n", 
                driverName, functionName, value, MIN_UPDATE_TIME);
            value = MIN_UPDATE_TIME;
            setDoubleParam(P_UpdateTime, value);
        }
        /* If the update time has changed and we are running then wake up the userProcess task */
        getIntegerParam(P_Run, &run);
        if (run) epicsEventSignal(eventId_);
    } else {
        /* All other parameters just get set in parameter list, no need to
         * act on them here */
    }
    
    /* Do callbacks so higher layers see any changes */
    status = (asynStatus) callParamCallbacks();
    
    if (status) 
        epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize, 
                  "%s:%s: status=%d, function=%d, name=%s, value=%f", 
                  driverName, functionName, status, function, paramName, value);
    else        
        asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
              "%s:%s: function=%d, name=%s, value=%f\n", 
              driverName, functionName, function, paramName, value);
    return status;
}
#if 0
void timingAsynEpics::userProcess()
{
    int err;
    double sleepTime_s;
    struct timespec vartime;
    long time_elapsed_nanos;

    /* Loop forever */    
    while (1) { 
        if (scanRateHz!=0) {
            sleepTime_s = 1.0 / (double)scanRateHz;

						setDoubleParam(P_ReadLatencyUs, time_elapsed_nanos/1000.0); 
						callParamCallbacks();

						//doCallbacksInt16Array(daq->sample, daq->nb_sample, P_Waveform1 + i, 0);
            epicsThreadSleep(sleepTime_s);
        } else {
            epicsThreadSleep(1);
        }
    }
}
#else
void timingAsynEpics::userProcess()
{
    double updateTime;
    //struct timespec vartime;
    //long time_elapsed_nanos;
		int run;

		lock();
    /* Loop forever */    
    while (1) { 
			getIntegerParam(P_Run, &run);
			getDoubleParam(P_UpdateTime, &updateTime);
			unlock();
			if(run)	epicsEventWaitWithTimeout(eventId_, updateTime);
			else		epicsEventWait(eventId_);
			lock();

			getIntegerParam(P_Run, &run);
			if(!run)		continue;

			//
			
    }
}

#endif

void userProcess(void *drvPvt)
{
		timingAsynEpics *pPvt = (timingAsynEpics *)drvPvt;

    if (pPvt->clientThreadMode==10) 
		{
        fprintf(stderr, "start snapshot user task (mode real samples)\n");
        pPvt->userProcess();
    }
}

extern "C" {

/** EPICS iocsh callable function to call constructor for the testAsynPortDriver class.
  * \param[in] portName The name of the asyn port driver to be created.
  * \param[in] maxSizeSnapshot The maximum  number of sample in one snapshot
  * \param[in] maxNbSnapshot The number of snapshot buffered
 */
epicsShareFunc int timingAsynEpicsConfigure(const char *portName, int maxSizeSnapshot, int maxNbSnapshot, int clientMode, const char *deviceName)
{
    new timingAsynEpics(portName, maxSizeSnapshot, maxNbSnapshot, clientMode, deviceName);
    return(asynSuccess);
}


/* EPICS iocsh shell commands */
static const iocshArg initArg0 = { "portName"          ,iocshArgString};
static const iocshArg initArg1 = { "max size snapshot" ,iocshArgInt};
static const iocshArg initArg2 = { "max size buffer MB",iocshArgInt};
static const iocshArg initArg3 = { "client mode"       ,iocshArgInt};
static const iocshArg initArg4 = { "device name"       ,iocshArgString};
static const iocshArg * const initArgs[] = {&initArg0,
                                            &initArg1,
                                            &initArg2,
                                            &initArg3,
                                            &initArg4};
static const iocshFuncDef initFuncDef = {"timingAsynEpicsConfigure",5,initArgs};
static void initCallFunc(const iocshArgBuf *args)
{
    timingAsynEpicsConfigure(args[0].sval, args[1].ival, args[2].ival, args[3].ival, args[4].sval);
}

void timingAsynEpicsRegister(void)
{
    iocshRegister(&initFuncDef,initCallFunc);
}

epicsExportRegistrar(timingAsynEpicsRegister);
}//end extern "C"
