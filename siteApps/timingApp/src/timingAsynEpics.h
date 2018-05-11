#ifndef		_TIMING_ASYN_EPICS_H
#define		_TIMING_ASYN_EPICS_H

#include "asynPortDriver.h"
#include "timingRegMap.h"

#define P_RunString									"SCOPE_RUN"                  /* asynInt32,    r/w */
#define P_UpdateTimeString					"SCOPE_UPDATE_TIME"          /* asynFloat64,  r/w */

extern "C" {
}

class timingAsynEpics : public asynPortDriver 
{
	public:
			timingAsynEpics(const char *portName, int maxSizeSnapshot, int maxSizeBufferMB, int clientMode, const char *deviceName);
			void userProcess();
			virtual asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);
			virtual asynStatus writeFloat64(asynUser *pasynUser, epicsFloat64 value);

			int clientThreadMode;

	protected:
			/** Values used for pasynUser->reason, and indexes into the parameter library. */
			// User parameters
			int P_Run;
#define FIRST_SCOPE_COMMAND P_Run
			int P_UserAddressSelect;
			int P_ReadLatencyUs;
			int P_ChannelEn;
			int P_Firmware;
			int P_Software;
			int P_UpdateTime;
#define LAST_SCOPE_COMMAND P_UpdateTime

	private:
		const char *driverName;
    epicsEventId eventId_;
    int scanRateHz;
    int system_init_ok;
		struct TimingRegmap *pregmap;

};

#define NUM_SCOPE_PARAMS (&LAST_SCOPE_COMMAND - &FIRST_SCOPE_COMMAND + 1)

#endif
