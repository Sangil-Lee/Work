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

#include "timingAsynEpics.h"

static void userProcess(void *drvPvt);

using timing::timingDriver;

timingAsynEpics::timingAsynEpics(const char *portName, int maxSizeSnapshot, int maxSizeBufferMB, int clientMode, const char* filename, const char *deviceName = "EVR")
   : asynPortDriver(portName, 
                    1,  /*maxAddr */ 
					/*0xC600, maxAddr*/
                    150,
                    asynInt32Mask | asynFloat64Mask | asynInt16ArrayMask | asynFloat64ArrayMask | asynEnumMask | asynOctetMask |asynDrvUserMask, /* Interface mask */
                    asynInt32Mask | asynFloat64Mask | asynInt16ArrayMask | asynFloat64ArrayMask | asynEnumMask | asynOctetMask,  /* Interrupt mask */
                    1, /* asynFlags.  This driver does not block and it is not multi-device, so flag is 0 */
                    1, /* Autoconnect */
                    0, /* Default priority */
                    0 /* Default stack size*/
										),clientThreadMode(clientMode),fileName(filename),driverName("timingAsynEpics"),system_init_ok(0) 
{
	printf("GCC Version-(%d)\n", GCC_VERSION);
    asynStatus status;
    const char *functionName = "timingAsynEpics";

	tsmode = tsMode(deviceName);
	//printf("TSMode(%d)\n", tsmode);

    //ptiming = (struct timingDriver *)malloc(sizeof(timingDriver));
    //ptiming = (timingDriver *)calloc(1,sizeof(timingDriver));
    ptiming = new timingDriver(tsmode);

	//ts2api_init(tsmode);
	//ts2api_getInstance(gpSys);
	evfile = ptiming->GetEVFileDescriptor();

    // Create database entries
    eventId_ = epicsEventCreate(epicsEventEmpty);  

    // Snapshots  
	registerParamListFromFile(fileName);

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
}

void timingAsynEpics::registerParamListFromFile(string filename)
{
	ifstream file(filename.c_str());
	if(file.fail()) return;
	
	RegMap regmap;
	string strToken;
	while(!file.eof())
	{
		getline(file, strToken);
		if(strToken[0] == '#' || strToken.empty()==true) continue;

		char str[strToken.size()];
		strcpy (str, strToken.c_str());

		char *pch = 0;
		if(!(pch = strtok (str," \t,"))) continue;
		memset(&regmap,0, sizeof(RegMap));

		//printf("ALIAS_Name(%s)\n", pch);
		strcpy(regmap.drvname, pch);
		while (pch != NULL)
		{
			if(!(pch = strtok (NULL," \t,"))) continue;
		//	printf("Addr(%s)\n", pch);
			regmap.address = strtol(pch,NULL,16);
			if(!(pch = strtok (NULL," \t,"))) continue;
			regmap.paramtype = getAsynParamType(pch);
		//	printf("Index(%s)\n", pch);
		};
		regmap.index = 0;
		createParamNMap(regmap);
	};	

	setIOCStartTime();
			
	//Default Initial Value
	//setIntegerParam(P_TSMode, tsmode);
	//setIntegerParam(P_Run, 0);
	//setDoubleParam(P_UpdateTime, 1);
	//setDoubleParam(P_Firmware, ptiming->GetFirmware());
	//setDoubleParam(P_Software, ptiming->GetSoftware());
	//setStringParam(P_BuildTime, "2018-06-01");
}

asynParamType timingAsynEpics::getAsynParamType(const char *paramstring)
{
	asynParamType paramtype = asynParamNotDefined;

	if( 0 == strcmp("asynParamNotDefined",paramstring))
		paramtype = asynParamNotDefined;
	else if(0 == strcmp("asynParamInt32",paramstring))
		paramtype = asynParamInt32;
	else if(0 == strcmp("asynParamUInt32Digital",paramstring))
		paramtype = asynParamUInt32Digital;
	else if(0 == strcmp("asynParamFloat64",paramstring))
		paramtype = asynParamFloat64;
	else if(0 == strcmp("asynParamOctet",paramstring))
		paramtype = asynParamOctet;
	else if(0 == strcmp("asynParamInt8Array",paramstring))
		paramtype = asynParamInt8Array;
	else if(0 == strcmp("asynParamInt16Array",paramstring))
		paramtype = asynParamInt16Array;
	else if(0 == strcmp("asynParamInt32Array",paramstring))
		paramtype = asynParamInt32Array;
	else if(0 == strcmp("asynParamFloat32Array",paramstring))
		paramtype = asynParamFloat32Array;
	else if(0 == strcmp("asynParamFloat64Array",paramstring))
		paramtype = asynParamFloat64Array;
	else if(0 == strcmp("asynParamGenericPointer",paramstring))
		paramtype = asynParamGenericPointer;
	
	return (paramtype);
}

asynStatus timingAsynEpics::createParamNMap(RegMap &reg)
{
    asynStatus status = asynSuccess;
	printf("RegMap.drvname(%s),RegMap.paramtype(%d), RegMap.index(%d)\n",reg.drvname, reg.paramtype, reg.index);
    createParam(reg.drvname, reg.paramtype, &reg.index);
	regmapfile[reg.drvname] = reg;
	regmaptable[reg.index] = regmapfile[reg.drvname];

	return (status);
}

int timingAsynEpics::tsMode(const char* mode)
{
		//{1 [evg] | 2 [evr] | 3 [evf] | 4 [evs]}");
		if(strcmp(mode, "EVR") == 0 || strcmp(mode, "evs") == 0)
			tsmode = 2;
		else if(strcmp(mode, "EVF") == 0 || strcmp(mode, "evf") == 0)
			tsmode = 3;
		else if(strcmp(mode, "EVS") == 0 || strcmp(mode, "evs") == 0)
			tsmode = 4;
		else
			tsmode = 1;

		return(tsmode);
}

epicsInt32 timingAsynEpics::readInt32Value(const RegMap &regmap)
{
	unsigned int rdData;
	ptiming->ts2ip_rd(evfile, regmap.address, (unsigned int*)&rdData);
	epicsInt32 value = rdData;

	if (strcmp(regmap.drvname, "FAN_FRONT_A") == 0 || 
		strcmp(regmap.drvname, "FAN_REAR_A") == 0)
	{
		bitset<sizeof(rdData)> bitval(rdData);
		bitval = (bitval>>16);
		bitval &= 0xffff;
		value = bitval.to_ulong();
		printf("%s:index(%d),value(%d),bitvalue(%ld)\n",regmap.drvname,regmap.index, value, bitval.to_ulong());
	} 
	else if (strcmp(regmap.drvname, "FAN_FRONT_B") == 0 ||
			 strcmp(regmap.drvname, "FAN_REAR_B") == 0)
	{
		bitset<sizeof(rdData)> bitval(rdData);
		bitval = (bitval>>0);
		bitval &= 0xffff;
		value = bitval.to_ulong();
		printf("%s:index(%d),value(%d),bitvalue(%ld)\n",regmap.drvname,regmap.index, value, bitval.to_ulong());
	}

	return (value);	
}

asynStatus timingAsynEpics::readInt32(asynUser *pasynUser, epicsInt32 *value)
{
    int function = pasynUser->reason;
    int addr=0;
    asynStatus status = asynSuccess;
    epicsTimeStamp timeStamp; getTimeStamp(&timeStamp);
    static const char *functionName = "readInt32";
    
    status = getAddress(pasynUser, &addr); if (status != asynSuccess) return(status);
    /* We just read the current value of the parameter from the parameter library.
     * Those values are updated whenever anything could cause them to change */
    status = (asynStatus) getIntegerParam(addr, function, value);
    /* Set the timestamp */
    pasynUser->timestamp = timeStamp;
	
	RegMap regmap = regmaptable[function];
	*value = readInt32Value(regmap);

    if (status) 
        epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize, 
                  "%s:%s: status=%d, function=%d, value=%d", 
                  driverName, functionName, status, function, *value);
    else        
        asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
              "%s:%s: function=%d, value=%d\n", 
              driverName, functionName, function, *value);
	status = (asynStatus) callParamCallbacks();
    return(status);
}

asynStatus timingAsynEpics::writeInt32(asynUser *pasynUser, epicsInt32 value)
{
	int function = pasynUser->reason;
	asynStatus status = asynSuccess;
	const char *paramName;
	const char* functionName = "writeInt32";

	int addr = 0;
	status = getAddress(pasynUser, &addr); if (status != asynSuccess) return(status);

	/* Set the parameter in the parameter library. */
	status = (asynStatus) setIntegerParam(function, value);

	/* Fetch the parameter string name for possible use in debugging */
	getParamName(function, &paramName);
	writeInt32Value(regmaptable[function]);

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

void timingAsynEpics::setIOCStartTime()
{
	epicsTimeStamp current;
	epicsTimeGetCurrent (&current);
	char timebuf[20];
	epicsTimeToStrftime(timebuf,20,"%Y/%m/%d %H:%M:%S", &current);
	setParamValue("IOCSTARTTIME", timebuf);
}

int timingAsynEpics::checkParam(const string drvname)
{
	int checkval = 0;
	check_iter = regmapfile.find(drvname);

	if(check_iter == regmapfile.end())
		checkval = -1;

	return (checkval);
}

int timingAsynEpics::setParamValue(const string drvname, const int ival)
{
	int check = checkParam(drvname);
	if(check == 0)
		setIntegerParam(regmapfile[drvname].index, ival);
	return (check);
}

int timingAsynEpics::setParamValue(const string drvname, const double dval)
{
	int check = checkParam(drvname);
	if(check == 0)
		setDoubleParam(regmapfile[drvname].index, dval);
	return (check);
}

int timingAsynEpics::setParamValue(const string drvname, const string svalue)
{
	int check = checkParam(drvname);
	if(check == 0)
		setStringParam(regmapfile[drvname].index, svalue.c_str());
	return (check);
}

asynStatus timingAsynEpics::getParamValue(const string drvname, int maxChars, char *value )
{
    asynStatus status = asynSuccess;
	getStringParam(regmapfile[drvname].index, maxChars, value);
	return (status);
}

asynStatus timingAsynEpics::getParamValue(const string drvname, int &value)
{
    asynStatus status = asynSuccess;
    getIntegerParam(regmapfile[drvname].index, &value);
	return (status);
}

asynStatus timingAsynEpics::getParamValue(const string drvname, double &value)
{
    asynStatus status = asynSuccess;
	getDoubleParam(regmapfile[drvname].index, &value);
	return (status);
}

asynStatus timingAsynEpics::readStringValue(const RegMap &regmap, char *value)
{
    asynStatus status = asynSuccess;

	if (strcmp(regmap.drvname, "BUILD_TIME") == 0 )
	{
		unsigned int rdData = 0;
		ptiming->ts2ip_rd(evfile, regmap.address, (unsigned int*)&rdData);
		//BCDCode for time structure register, Sec(6bit)/Min(6bit)/Hour(5bit), Day(5bit)/Hour(4bit)/Year(5bit)
		int year  = (rdData & 0x007E0000) >> 17;
		int month = (rdData & 0x07800000) >> 23;
		int day   = (rdData & 0xF8000000) >> 27;
		int hour  = (rdData & 0x0001F000) >> 12;
		int min   = (rdData & 0x00000FC0) >> 6;
		int sec   = (rdData & 0x0000003F) >> 0;
		char timebuf[20];
		sprintf(timebuf,"20%d/%d/%d %d:%d:%d", year, month, day, hour, min, sec);
		strcpy(value, (char*)timebuf);
	}
	return (status);
}

asynStatus timingAsynEpics::readOctet(asynUser *pasynUser, char *value, size_t maxChars, size_t *nActual, int *eomReason)
{
    int function = pasynUser->reason;
    int addr=0;
    asynStatus status = asynSuccess;
    epicsTimeStamp timeStamp; getTimeStamp(&timeStamp);
    static const char *functionName = "readOctet";
   
    status = getAddress(pasynUser, &addr); if (status != asynSuccess) return(status);
    /* We just read the current value of the parameter from the parameter library.
     * Those values are updated whenever anything could cause them to change */
    status = (asynStatus)getStringParam(addr, function, (int)maxChars, value);

    /* Set the timestamp */
    pasynUser->timestamp = timeStamp;
	readStringValue(regmaptable[function], value);
		
    if (status) 
        epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize, 
                  "%s:%s: status=%d, function=%d, value=%s", 
                  driverName, functionName, status, function, value);
    else        
        asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
              "%s:%s: function=%d, value=%s\n", 
              driverName, functionName, function, value);
    if (eomReason) *eomReason = ASYN_EOM_END;
    *nActual = strlen(value)+1;

		status = (asynStatus) callParamCallbacks();

    return(status);
}

asynStatus timingAsynEpics::writeOctet(asynUser *pasynUser, const char *value, size_t maxChars, size_t *nActual)
{
    int function = pasynUser->reason;
	asynStatus status = asynSuccess;
    const char* functionName = "writeOctet";

    /* Set the parameter in the parameter library. */
    status = (asynStatus) setStringParam(function, value);

	writeOctetValue(regmaptable[function]);

#if 0
	if (function == P_Starttime) {
		/* Make sure the update time is valid. If not change it and put back in parameter library */
		char svalue[maxChars] = "";
		getStringParam(P_Starttime, maxChars, svalue);
		//printf("writeOctet*****:%s(%ld)\n", svalue, maxChars);
	}
#endif

    status = (asynStatus) callParamCallbacks();

    if (status) 
        epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize, 
                  "%s:%s: status=%d, function=%d, value=%s", 
                  driverName, functionName, status, function, value);
    else        
        asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
              "%s:%s: function=%d, value=%s\n", 
              driverName, functionName, function, value);

	return (status);
}

int timingAsynEpics::writeOctetValue(const RegMap &regmap)
{
    int status = 0;
	if(checkParam(regmap.drvname) == -1) return(-1);

	char svalue[64] = "";
	getStringParam(regmap.index, 64, svalue);

	//ptiming->ts2ip_wr(evfile, regmap.address, value );
    return  (status);
}

int timingAsynEpics::writeFloat64Value(const RegMap &regmap)
{
    int status = 0;
	epicsFloat64 value = 0.0;
	if(checkParam(regmap.drvname) == -1) return(-1);

	getDoubleParam(regmap.index, &value);
	ptiming->ts2ip_wr(evfile, regmap.address, value );
    return  (status);
}

int timingAsynEpics::writeInt32Value(const RegMap &regmap)
{
	epicsInt32 value = 0;
	int status = 0;

	if(checkParam(regmap.drvname) == -1) return(-1);

	getIntegerParam(regmap.index, &value);

	if(strcmp(regmap.drvname, "SEQUENCE_USER_TRIGGER") == 0)
	{
		//User Trigger Register 0->1->0
		int trigger = 1;	
		trigger &= 0x1;
		ptiming->ts2ip_wr(evfile, regmap.address, trigger);
		trigger = 0 & 0x1;
		ptiming->ts2ip_wr(evfile, regmap.address, trigger);
	};

    return  (status);
}

asynStatus timingAsynEpics::writeFloat64(asynUser *pasynUser, epicsFloat64 value)
{
    int addr = 0;
    int function = pasynUser->reason;
    asynStatus status = asynSuccess;
    const char *paramName;
    const char* functionName = "writeFloat64";

    /* Set the parameter in the parameter library. */
    status = (asynStatus) setDoubleParam(function, value);

    status = getAddress(pasynUser, &addr); if (status != asynSuccess) return(status);

    /* Fetch the parameter string name for possible use in debugging */
    getParamName(function, &paramName);
	writeFloat64Value(regmaptable[function]);
    
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
	//struct timespec vartime;
	//long time_elapsed_nanos;

	double updateTime;
	int run = 1;

	lock();

	/* Loop forever 
	 * If the logic will be able to block into scan thread in IOC, this thread should work in IOC.
	 */    
	while (1) { 
		getParamValue("RUN", run);
		getParamValue("UPDATE_TIME", updateTime);

		//getIntegerParam(P_Run, &run);
		//getDoubleParam(P_UpdateTime, &updateTime);
		//printf("Run:%d, Updatetime:%f\n",run, updateTime);

		/* Asynchronous Logic */

		unlock();
		if(run)		epicsEventWaitWithTimeout(eventId_, updateTime);
		else		epicsEventWait(eventId_);
		lock();

		//getIntegerParam(P_Run, &run);

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
epicsShareFunc int timingAsynEpicsConfigure(const char *portName, int maxSizeSnapshot, int maxNbSnapshot, int clientMode, const char* registerfile,const char *deviceName)
{
    new timingAsynEpics(portName, maxSizeSnapshot, maxNbSnapshot, clientMode,registerfile, deviceName);
    return(asynSuccess);
}


/* EPICS iocsh shell commands */
static const iocshArg initArg0 = { "portName"          ,iocshArgString};
static const iocshArg initArg1 = { "max size snapshot" ,iocshArgInt};
static const iocshArg initArg2 = { "max size buffer MB",iocshArgInt};
static const iocshArg initArg3 = { "client mode"       ,iocshArgInt};
static const iocshArg initArg4 = { "register file name",iocshArgString};
static const iocshArg initArg5 = { "device name"       ,iocshArgString};
static const iocshArg * const initArgs[] = {&initArg0,
                                            &initArg1,
                                            &initArg2,
                                            &initArg3,
                                            &initArg4,
                                            &initArg5};
static const iocshFuncDef initFuncDef = {"timingAsynEpicsConfigure",6,initArgs};
static void initCallFunc(const iocshArgBuf *args)
{
    timingAsynEpicsConfigure(args[0].sval, args[1].ival, args[2].ival, args[3].ival, args[4].sval, args[5].sval);
}

void timingAsynEpicsRegister(void)
{
    iocshRegister(&initFuncDef,initCallFunc);
}

epicsExportRegistrar(timingAsynEpicsRegister);
}//end extern "C"
