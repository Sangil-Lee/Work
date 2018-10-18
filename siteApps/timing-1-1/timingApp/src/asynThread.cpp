#include <cstdlib>
#include <cstring>
#include <bitset>
#include <epicsTypes.h>
#include <epicsTime.h>
#include <epicsThread.h>
#include <epicsString.h>
#include <epicsTimer.h>
#include <epicsMutex.h>
#include <epicsEvent.h>
#include <registryFunction.h>
#include <epicsExport.h>
#include <dbCommon.h>
#include <iocsh.h>

#include "fpsmitiAsynEpics.h"

static void asynProcess(void *drvPvt);

using timing::timingDriver;

fpsmitiAsynEpics::fpsmitAsynEpics(const char *portName, int maxSizeSnapshot, int maxSizeBufferMB, int clientMode, const char* filename, const char *deviceName, const int evrNum )
   : asynPortDriver(portName, 
                    1,  /*maxAddr */ 
                    150,
                    asynInt32Mask | asynFloat64Mask | asynInt16ArrayMask | asynFloat64ArrayMask | asynEnumMask | asynOctetMask |asynDrvUserMask, /* Interface mask */
                    asynInt32Mask | asynFloat64Mask | asynInt16ArrayMask | asynFloat64ArrayMask | asynEnumMask | asynOctetMask,  /* Interrupt mask */
                    1, /* asynFlags.  This driver does not block and it is not multi-device, so flag is 0 */
                    1, /* Autoconnect */
                    0, /* Default priority */
                    0 /* Default stack size*/
					)
{
	status = (asynStatus)(epicsThreadCreate("timingAsynEpicsUserTask",
				epicsThreadPriorityMedium,
				epicsThreadGetStackSize(epicsThreadStackMedium),
				(EPICSTHREADFUNC)::asynProcess,
				this) == NULL);

}

void fpsmitiAsynEpics::asynProcess()
{
	//struct timespec vartime;
	//long time_elapsed_nanos;

	double updateTime;
	int run = 1;

	lock();

	/* Loop forever 
	 * If the logic will be able to block into scan thread in IOC, this thread should work in IOC.
	 */    
	while (1) { 

		/* Asynchronous Logic */
		// Here ... 

		unlock();
		if(run)		epicsEventWaitWithTimeout(eventId_, updateTime);
		else		epicsEventWait(eventId_);
		lock();

		if(!run)		continue;
	}
}

void asynProcess(void *drvPvt)
{
	fpsmitiAsynEpics *pPvt = (fpsmitiAsynEpics *)drvPvt;
	pPvt->asynProcess();
}
