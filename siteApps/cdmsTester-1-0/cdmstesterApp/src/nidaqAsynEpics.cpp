//#include <boost/range/numeric.hpp>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <bitset>
#include <numeric>
#include <valarray>
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

#include "nidaqAsynEpics.h"

static void userProcess(void *drvPvt);

using nidaq::nidaqDriver;

static int nidaqDebug = 0;
epicsExportAddress(int, nidaqDebug);

static int nidaqPrintCount = 20;
epicsExportAddress(int, nidaqPrintCount);


nidaqAsynEpics::nidaqAsynEpics(const char *portName, const char* filename, const char *deviceName)
	: asynPortDriver(portName, 
			1,  /*maxAddr */ 
			15000,
			asynInt32Mask | asynUInt32DigitalMask | asynFloat64Mask | asynInt16ArrayMask | asynInt32ArrayMask| asynFloat64ArrayMask | asynEnumMask | asynOctetMask |asynDrvUserMask, /* Interface mask */
			asynInt32Mask | asynUInt32DigitalMask | asynFloat64Mask | asynInt16ArrayMask | asynInt32ArrayMask| asynFloat64ArrayMask | asynEnumMask | asynOctetMask,  /* Interrupt mask */
			1, /* asynFlags.  This driver does not block and it is not multi-device, so flag is 0 */
			1, /* Autoconnect */
			0, /* Default priority */
			0 /* Default stack size*/
			),fileName(filename),driverName("nidaqAsynEpics"),system_init_ok(1),gTask(0)
{
	printf("GCC Version-(%d)\n", GCC_VERSION);
    asynStatus status;
    const char *functionName = "nidaqAsynEpics";

    // Create database entries
    eventId_ = epicsEventCreate(epicsEventEmpty);  

	DAQmxCreateTask("", &gTask);

    // register parameter list from register file.  
	registerParamListFromFile(fileName);

	//DAQmxCfgSampClkTiming(gTask, "", 500.0/*Sampling Rate, 500Hz*/, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 1000);
	DAQmxCfgSampClkTiming(gTask, "", 1000.0/*1kHz*/, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 1000);
	DAQmxStartTask(gTask);

	if (system_init_ok==1) 
	{
		status = (asynStatus)(epicsThreadCreate("nidaqAsynEpicsUserTask",
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
nidaqAsynEpics::~nidaqAsynEpics()
{
	if( gTask!=0 )  {
		/*********************************************/
		// DAQmx Stop Code
		/*********************************************/
		DAQmxStopTask(gTask);
		DAQmxClearTask(gTask);
	};
}

void nidaqAsynEpics::registerParamListFromFile(string filename)
{
	ifstream file(filename.c_str());
	if(file.fail()) return;
	
	RegMap regmap;
	string strToken;
	char str[200];
	char *pch;

	regmap.index = 0;

	while(!file.eof())
	{
		getline(file, strToken);
		if(strToken[0] == '#' || strToken.empty()==true) continue;

		strcpy (str, strToken.c_str());
		memset(&regmap,0, sizeof(RegMap));

		if(!(pch = strtok (str,"\t, "))) continue;
		strcpy(regmap.drvname, pch);

		if(!(pch = strtok (NULL,"\t, "))) continue;
		regmap.options = strtoul(pch,NULL,16);

		if(!(pch = strtok (NULL,"\t, "))) continue;
		regmap.scaleparameter[0] = strtof(pch,NULL);
		if(!(pch = strtok (NULL,"\t, "))) continue;
		regmap.scaleparameter[1] = strtof(pch,NULL);
		if(!(pch = strtok (NULL,"\t, "))) continue;
		regmap.scaleparameter[2] = strtof(pch,NULL);
		if(!(pch = strtok (NULL,"\t, "))) continue;
		regmap.scaleparameter[3] = strtof(pch,NULL);

		if(!(pch = strtok (NULL,"\t, "))) continue;
		regmap.paramtype = getAsynParamType(pch);

		if(!(pch = strtok (NULL,"\t, "))) continue;
		strcpy(regmap.devicename, pch);

		if(!(pch = strtok (NULL,"\t, "))) continue;
		strncpy(regmap.drvlink, pch, strlen(pch)-1);	// string '\0' null termination remove

		createParamNMap(regmap);
		regmap.index++;
	};	

}

asynParamType nidaqAsynEpics::getAsynParamType(const char *paramstring)
{
	asynParamType paramtype = asynParamNotDefined;

	if( 0 == strcmp("asynParamNotDefined",paramstring))			paramtype = asynParamNotDefined;
	else if(0 == strcmp("asynParamInt32",paramstring))			paramtype = asynParamInt32;
	else if(0 == strcmp("asynParamUInt32Digital",paramstring))	paramtype = asynParamUInt32Digital;
	else if(0 == strcmp("asynParamFloat64",paramstring))		paramtype = asynParamFloat64;
	else if(0 == strcmp("asynParamOctet",paramstring))			paramtype = asynParamOctet;
	else if(0 == strcmp("asynParamInt8Array",paramstring))		paramtype = asynParamInt8Array;
	else if(0 == strcmp("asynParamInt16Array",paramstring))		paramtype = asynParamInt16Array;
	else if(0 == strcmp("asynParamInt32Array",paramstring))		paramtype = asynParamInt32Array;
	else if(0 == strcmp("asynParamFloat32Array",paramstring))	paramtype = asynParamFloat32Array;
	else if(0 == strcmp("asynParamFloat64Array",paramstring))	paramtype = asynParamFloat64Array;
	else if(0 == strcmp("asynParamGenericPointer",paramstring)) paramtype = asynParamGenericPointer;
	
	return (paramtype);
}

asynStatus nidaqAsynEpics::createParamNMap(RegMap &regmap)
{
    asynStatus status = asynSuccess;
   	createParam(regmap.drvname, regmap.paramtype, &regmap.index);
#if 0
	getOptionBitset(optbit, regmap.option,  0, 0xFFF);   //12bit to 32 bitset
#else
	getOptionBitset(optbit, regmap.options);			//full option to 32 bitset
#endif
	float64 opt1 = 0.0, opt2 = 10.0, opt3 = 1.0, opt4 = 0.0;
	int current = 0;

	if(optbit.test(16) == true) current = 1;
	
	unsigned int scalemode = getRShiftMask(regmap.options, 0, 0xF);
	unsigned long optval = optbit.to_ulong();

	if(optval>=1)
	{
		opt1 = regmap.scaleparameter[0];
		opt2 = regmap.scaleparameter[1];
		opt3 = regmap.scaleparameter[2];
		opt4 = regmap.scaleparameter[3];
		printf("devname(%s),scalemode(%d),opt1val(%f),opt2val(%f),opt3val(%f),opt4val(%f),optval(%lu)\n",regmap.devicename, scalemode, opt1, opt2, opt3, opt4, optval);
		//regmap.pNIDAQ = make_shared<nidaq::nidaqDriver>(regmap.devicename, regmap.scaleparameter, scalemode, current);
	};

#if 1
	if(scalemode == 1) {
		DAQmxCreateAIVoltageChan(gTask, regmap.devicename, "", DAQmx_Val_RSE, opt3, opt4, DAQmx_Val_FromCustomScale, "UserRangeMap1");
		DAQmxCreateMapScale("UserRangeMap1",opt1,opt2,opt3,opt4,DAQmx_Val_Volts, "bar");
	}else if(scalemode == 2) {
		DAQmxCreateAIVoltageChan(gTask, regmap.devicename, "", DAQmx_Val_RSE, opt3, opt4, DAQmx_Val_FromCustomScale, "UserRangeMap2");
		DAQmxCreateMapScale("UserRangeMap2",opt1,opt2,opt3,opt4,DAQmx_Val_Volts, "bar");
	}else if(scalemode == 3) {
		DAQmxCreateAIVoltageChan(gTask, regmap.devicename, "", DAQmx_Val_RSE, opt3, opt4, DAQmx_Val_FromCustomScale, "UserRangeMap3");
		DAQmxCreateMapScale("UserRangeMap3",opt1,opt2,opt3,opt4,DAQmx_Val_Volts, "bar");
	};
	
#else
	DAQmxCreateAIVoltageChan(gTask, regmap.devicename, "", DAQmx_Val_RSE, opt1, opt2, DAQmx_Val_Volts, NULL);
#endif

	regmaptable[regmap.index] = regmapfile[regmap.drvname] = regmap;

	string strOpt = optbit.to_string();
	stringstream sstream;

	for(size_t i = 1; i < 33; i ++)
	{
		if((i%4)== 0 && (i!=32)) 
			sstream << strOpt.at(i-1) << ",";
		else 
			sstream << strOpt.at(i-1);
	};
	string strOption = sstream.str();

	if(regmap.index == 0 && nidaqDebug)
	{
		printf("%s\n", "==========================================================================================");
		printf("%4s, %-34s(%s), %10s, %10s, %-10s\n", "index", "drvname","options", "paramtype", "devicename", "drvlink");
		printf("%s\n", "------------------------------------------------------------------------------------------");
	}

	if(nidaqDebug)
		printf("%5d, %-34s(%s), %10d, %10s(len:%u), %-10s(len:%d)\n", regmap.index, regmap.drvname, strOption.c_str(),regmap.paramtype,regmap.devicename, 
				(int)strlen(regmap.devicename),regmap.drvlink,(int)strlen(regmap.drvlink));

	return (status);
}

epicsInt32 nidaqAsynEpics::readInt32Value(const RegMap &regmap)
{
	unsigned int rdData = 0;

	epicsInt32 value = rdData;
	return (value);	
}

asynStatus nidaqAsynEpics::readValue(const RegMap &regmap, epicsInt32 &value)
{
	//Register address read.
    asynStatus status = asynSuccess;

	epicsInt32 rdData = 0;

	value = rdData;
	return (status);	
}

asynStatus nidaqAsynEpics::readValue(const RegMap &regmap, epicsFloat64 &value)
{
	//Register address read.
	//....
	//epicsFloat64 rdValue = 0.0;
    asynStatus status = asynSuccess;

	float64		rdValue = 0.0;
	value = rdValue;

	return (status);	
}

asynStatus nidaqAsynEpics::readFloat64Array(asynUser *pasynUser, epicsFloat64 *value, size_t nElements, size_t *nIn)
{
    int function = pasynUser->reason;
    asynStatus status = asynSuccess;
    epicsTimeStamp timeStamp;
    const char *functionName = "readFlot64Array";

    getTimeStamp(&timeStamp);
    pasynUser->timestamp = timeStamp;

    RegMap regmap = regmaptable[function];

#if 1
    if(nidaqDebug > 2)
        printf("%s(%s),nEle(%ld)\n",functionName,regmap.drvname, nElements);
#endif

	float64	rdValue[nElements];
	int count = 0;

	//regmap.pNIDAQ->ReadAnalog(rdValue, &count, nElements, 2.0);

	string strData = string(regmap.drvlink);
	if(strData.compare("NA") != 0)
	{
		float64 avg = accumulate(rdValue, rdValue+nElements, 0.0, plus<float64>()) / nElements;
		if(nidaqDebug)	
		{
			float64 min = *std::min_element(rdValue, rdValue+nElements);
			float64 max = *std::max_element(rdValue, rdValue+nElements);
			printf("Min:%f, Avg: %f, Max: %f\n",min, avg, max);
		};
		if((setParamValue(regmap.drvlink, avg)) == 0)
		{
			callParamCallbacks();
			epicsEventSignal(eventId_);
		};
	};

	memcpy(value, rdValue, nElements*sizeof(epicsFloat64));
	doCallbacksFloat64Array(value, nElements, regmap.index, 0);
	*nIn = nElements;

    return status;
}

asynStatus nidaqAsynEpics::readInt32(asynUser *pasynUser, epicsInt32 *value)
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
	
#if 0
	*value = readInt32Value(regmaptable[function]);
#else
	//New
	//readValue(regmaptable[function], *value);
#endif

    if (status) 
        epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize, 
                  "%s:%s: status=%d, function=%d, value=%d", 
                  driverName, functionName, status, function, *value);
    else        
        asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, "%s:%s: function=%d, value=%d\n", 
											driverName, functionName, function, *value);
	status = (asynStatus) callParamCallbacks();
    return(status);
}
asynStatus nidaqAsynEpics::readFloat64(asynUser *pasynUser, epicsFloat64 *value)
{
    int function = pasynUser->reason;
    int addr=0;
    asynStatus status = asynSuccess;
    epicsTimeStamp timeStamp; getTimeStamp(&timeStamp);
    static const char *functionName = "readFloat64";
    
    status = getAddress(pasynUser, &addr); if (status != asynSuccess) return(status);
    /* We just read the current value of the parameter from the parameter library.
     * Those values are updated whenever anything could cause them to change */
    status = (asynStatus) getDoubleParam(addr, function, value);
	//asynStatus nidaqAsynEpics::getParamValue(const string drvname, double &value)
	//
    /* Set the timestamp */
    pasynUser->timestamp = timeStamp;

	//readValue(regmaptable[function], *value);

    if (status) 
        epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize, 
                  "%s:%s: status=%d, function=%d, value=%f", 
                  driverName, functionName, status, function, *value);
    else        
        asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, "%s:%s: function=%d, value=%f\n", 
											driverName, functionName, function, *value);
	status = (asynStatus) callParamCallbacks();

    return(status);
}

asynStatus nidaqAsynEpics::writeInt32(asynUser *pasynUser, epicsInt32 value)
{
	int function = pasynUser->reason;
	asynStatus status = asynSuccess;
	const char *paramName;
	const char* functionName = "writeInt32";

	int addr = 0;
	status = getAddress(pasynUser, &addr); if (status != asynSuccess) return(status);

	/* Set the parameter in the parameter library. */
	status = (asynStatus) setIntegerParam(function, value);

	if(nidaqDebug)
		printf("[%s] : %d\n",functionName,value);


	/* Fetch the parameter string name for possible use in debugging */
	getParamName(function, &paramName);

	writeInt32Value(regmaptable[function], value);

	/* Do callbacks so higher layers see any changes */
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

asynStatus nidaqAsynEpics::writeUInt32Digital(asynUser *pasynUser, epicsUInt32 value, epicsUInt32 mask)
{
    int function = pasynUser->reason;
    int addr=0;
    asynStatus status = asynSuccess;
    const char* functionName = "writeUInt32Digital";

    status = getAddress(pasynUser, &addr); if (status != asynSuccess) return(status);

    /* Set the parameter in the parameter library. */
    status = (asynStatus) setUIntDigitalParam(addr, function, value, mask);

	writeInt32Value(regmaptable[function], (epicsUInt32)value);

    if (status) 
        epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize, 
                  "%s:%s: status=%d, function=%d, value=%u, mask=%u", 
                  driverName, functionName, status, function, value, mask);
    else        
        asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
              "%s:%s: function=%d, value=%d, mask=%u\n", 
              driverName, functionName, function, value, mask);
    return status;
}


int nidaqAsynEpics::checkParam(const string drvname)
{
	int checkval = 0;
	check_iter = regmapfile.find(drvname);

	if(check_iter == regmapfile.end())
		checkval = -1;

	return (checkval);
}

int nidaqAsynEpics::setParamValue(const string drvname, const int ival)
{
	int check = checkParam(drvname);
	if(check == 0)
		setIntegerParam(regmapfile[drvname].index, ival);
	return (check);
}

int nidaqAsynEpics::setParamValue(const string drvname, const double dval)
{
	int check = checkParam(drvname);
	if(check == 0)
		setDoubleParam(regmapfile[drvname].index, dval);
	return (check);
}

int nidaqAsynEpics::setParamValue(const string drvname, const string svalue)
{
	int check = checkParam(drvname);
	if(check == 0)
		setStringParam(regmapfile[drvname].index, svalue.c_str());
	return (check);
}

asynStatus nidaqAsynEpics::getParamValue(const string drvname, int maxChars, char *value )
{
    asynStatus status = asynSuccess;
	getStringParam(regmapfile[drvname].index, maxChars, value);
	return (status);
}

asynStatus nidaqAsynEpics::getParamValue(const string drvname, int &value)
{
    asynStatus status = asynSuccess;
    getIntegerParam(regmapfile[drvname].index, &value);
	return (status);
}

asynStatus nidaqAsynEpics::getParamValue(const string drvname, double &value)
{
    asynStatus status = asynSuccess;
	getDoubleParam(regmapfile[drvname].index, &value);
	return (status);
}


//===================laykim stringin
asynStatus nidaqAsynEpics::readStringValue(const RegMap &regmap, char *value)
{
    asynStatus status = asynSuccess;

	char strbuf[150];
	strcpy(value, (char*)strbuf);

	return (status);
}

asynStatus nidaqAsynEpics::readOctet(asynUser *pasynUser, char *value, size_t maxChars, size_t *nActual, int *eomReason)
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

asynStatus nidaqAsynEpics::writeOctet(asynUser *pasynUser, const char *value, size_t maxChars, size_t *nActual)
{
    int function = pasynUser->reason;
	asynStatus status = asynSuccess;
    const char* functionName = "writeOctet";

    /* Set the parameter in the parameter library. */
    status = (asynStatus) setStringParam(function, value);

	// status = writeOctetValue(regmaptable[function],value);
	status = writeStringValue(regmaptable[function],value);

    if (status) 
        epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize, 
                  "%s:%s: status=%d, function=%d, value=%s", 
                  driverName, functionName, status, function, value);
    else        
        asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
              "%s:%s: function=%d, value=%s\n", 
              driverName, functionName, function, value);

	*nActual = strlen(value);
	return (status);
}

asynStatus nidaqAsynEpics::writeOctetValue(const RegMap &regmap, const char *value)
{
	asynStatus status = asynSuccess;
	if(checkParam(regmap.drvname) == -1) return(asynError);

    return  (status);
}

asynStatus nidaqAsynEpics::writeStringValue(const RegMap &regmap, const char *strValue)
{
	asynStatus status = asynSuccess;
	if(checkParam(regmap.drvname) == -1) return(asynError);

	if(nidaqDebug)
		printf("Sub_drvname(%s),strValue(%s)\n", regmap.drvname, strValue);


    return  (status);
}


int nidaqAsynEpics::writeFloat64Value(const RegMap &regmap)
{
    int status = 0;
	epicsFloat64 value = 0.0;
	if(checkParam(regmap.drvname) == -1) return(-1);

	getDoubleParam(regmap.index, &value);
    return  (status);
}

int nidaqAsynEpics::writeInt32Value(const RegMap &regmap, epicsInt32 value)
{
	if(checkParam(regmap.drvname) == -1) return(-1);

	int status = 0;

    return  (status);
}

//===================laykim longout
int nidaqAsynEpics::writeInt32Value(const RegMap &regmap)
{
	int status = 0;

	if(checkParam(regmap.drvname) == -1) return(-1);

															//0x1 -> wave setup, 0x3-> last index + 0x8000000, Sequence Stop Bit(SEQA, SEQB).

    return  (status);
}

asynStatus nidaqAsynEpics::writeFloat64(asynUser *pasynUser, epicsFloat64 value)
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


void nidaqAsynEpics::UserProcess()
{
	//struct timespec vartime;
	//long time_elapsed_nanos;
#if 0
	while(true)
	{
		int32       read = 1000;
		//DAQmxReadAnalogF64(taskHandle,1000,10,DAQmx_Val_GroupByScanNumber,data,2000,&read,NULL);
		DAQmxReadAnalogF64(taskHandle,1000,-1,DAQmx_Val_GroupByChannel,data,2000,&read,NULL);
		/*********************************************/
		//DAQmxReadAnalogF64(taskHandle, 1, 2.0, DAQmx_Val_GroupByChannel, data, 1, &read, NULL);

		for ( int index = 0; index < 2000; index++) {
			printf("Data[%d] :%f\n",index, data[index]);
		}	
		printf("Acquired %d points\n",(int)read);
		sleep(1);
	};
#endif

	lock();

	const int read = 1000;
	int32 	  bufread = 0;
	const int pvcount = 3;
	float64 dataValue[pvcount*read];
	double vbx_wave_1[read];
	double vbx_wave_2[read];
	double cm_wave_1[read];

	/* Loop forever 
	 * If the logic will be able to block into scan thread in IOC, this thread should work in IOC.
	 */    
	while (1) { 
        // Release the lock while we wait for a command to start or wait for updateTime
        unlock();
		epicsEventWaitWithTimeout(eventId_, 0.5);

#if 0
        if (run) epicsEventWaitWithTimeout(eventId_, 0.0025);	//400Hz
        else     (void) epicsEventWait(eventId_);
#endif

        // Take the lock again
        lock(); 

		//DAQmxReadAnalogF64(gTask,1000,5.0,DAQmx_Val_GroupByScanNumber,dataValue,read*pvcount,&bufread,NULL);
		DAQmxReadAnalogF64(gTask,1000,5.0,DAQmx_Val_GroupByChannel,dataValue,read*pvcount,&bufread,NULL);

		for ( int index = 0; index < read; index++) {
			//printf("dataValue[%d] :%f\n",index, dataValue[index]);
			vbx_wave_1[index]  = dataValue[index];
			vbx_wave_2[index]  = dataValue[index+1000];
			cm_wave_1[index] = dataValue[index+2000];
#if 0
			printf("vbx_wave_1[%d] = %f\n", index, vbx_wave_1[index]);
			printf("vbx_wave_2[%d] = %f\n", index, vbx_wave_2[index]);
			printf(" cm_wave_1[%d] = %f\n", index, cm_wave_1[index]);
#endif
		};

		float64 vbx_1_avg = accumulate(vbx_wave_1, vbx_wave_1+read, 0.0, plus<float64>()) / read;
		float64 vbx_2_avg = accumulate(vbx_wave_2, vbx_wave_2+read, 0.0, plus<float64>()) / read;
		float64 cm_1_avg = accumulate(cm_wave_1, cm_wave_1+read, 0.0, plus<float64>()) / read;

		if(nidaqDebug)	
		{
#if 0
			float64 vbx_min = *std::min_element(vbx_wave_1, vbx_wave_1+read);
			float64 vbx_max = *std::max_element(vbx_wave_1, vbx_wave_1+read);
			float64 vbx_min_1 = *std::min_element(vbx_wave_2, vbx_wave_2+read);
			float64 vbx_max_1 = *std::max_element(vbx_wave_2, vbx_wave_2+read);
			float64 cm_1_min = *std::min_element(cm_wave_1, cm_wave_1+read);
			float64 cm_1_max = *std::max_element(cm_wave_1, cm_wave_1+read);
			printf("Min(VB(%f),CM1(%f)), Avg(VB(%f),CM1(%f)), Max(VB(%f),CM1(%f))\n",vbx_min,cm_1_min, vbx_1_avg, cm_1_avg, vbx_max, cm_1_max);
#endif
		};

    	RegMap regmap_vb_1   = regmapfile["NIDAQ_MX_SLOT2_WAVE_0"];
    	RegMap regmap_vb_2   = regmapfile["NIDAQ_MX_SLOT2_WAVE_1"];
    	RegMap regmap_cm_1 = regmapfile["NIDAQ_MX_SLOT2_WAVE_2"];

		if((setParamValue(regmap_vb_1.drvlink, vbx_1_avg)) == 0)
		{
			callParamCallbacks();
		};

		if((setParamValue(regmap_vb_2.drvlink, vbx_2_avg)) == 0)
		{
			callParamCallbacks();
		};

		if((setParamValue(regmap_cm_1.drvlink, cm_1_avg)) == 0)
		{
			callParamCallbacks();
		};

		doCallbacksFloat64Array(vbx_wave_1,  read, regmapfile["NIDAQ_MX_SLOT2_WAVE_0"].index, 0);
		doCallbacksFloat64Array(vbx_wave_2,  read, regmapfile["NIDAQ_MX_SLOT2_WAVE_1"].index, 0);
		doCallbacksFloat64Array(cm_wave_1, read, regmapfile["NIDAQ_MX_SLOT2_WAVE_2"].index, 0);
	
#if 0
		//epicsThreadSleep(1.0);
        /* run could have changed while we were waiting */
        getIntegerParam(P_Run, &run);
        if (!run) continue;

        callParamCallbacks();
        doCallbacksFloat64Array(pData_, maxPoints, P_Waveform, 0);
#endif
	};
}

void nidaqAsynEpics::getOptionBitset(OptionBit &optionbit, epicsUInt32 option, epicsUInt32 shift, epicsUInt32 mask)
{
	optionbit = (mask == 0) ? (option >> shift) : (option >> shift) & mask;
}

epicsUInt32 nidaqAsynEpics::getRShiftMask(epicsUInt32 option, epicsUInt32 shift, epicsUInt32 mask = 0)
{
	return (mask == 0) ? (option >> shift) : (option >> shift) & mask;
}

epicsUInt32 nidaqAsynEpics::getLShiftMask(epicsUInt32 option, epicsUInt32 shift, epicsUInt32 mask)
{
	return (mask == 0) ? (option << shift) : (mask << shift) | option;
}

epicsUInt32 nidaqAsynEpics::setBitOnOff(epicsUInt32 option, epicsUInt32 nbit, bool bit)
{
	epicsUInt32 retVal = (bit==1) ? option |= 1 << nbit : option &= ~(1 << nbit);
	return (retVal);
}

epicsUInt32 nidaqAsynEpics::setExclusiveBit(epicsUInt32 option, epicsUInt32 nbit)
{
	return (option ^= 1 << nbit);
}

bool nidaqAsynEpics::checkBit(epicsUInt32 option, epicsUInt32 nbit)
{
	return ((option >> nbit)&1);
}

void userProcess(void *drvPvt)
{
	nidaqAsynEpics *pPvt = (nidaqAsynEpics *)drvPvt;
	if(pPvt!=NULL) pPvt->UserProcess();
}

extern "C" {

/** EPICS iocsh callable function to call constructor for the testAsynPortDriver class.
  * \param[in] portName The name of the asyn port driver to be created.
  * \param[in] maxSizeSnapshot The maximum  number of sample in one snapshot
  * \param[in] maxNbSnapshot The number of snapshot buffered
 */
epicsShareFunc int nidaqAsynEpicsConfigure(const char *portName, const char* registerfile, const char *deviceName)
{
    new nidaqAsynEpics(portName, registerfile, deviceName);
    return(asynSuccess);
}


/* EPICS iocsh shell commands */
static const iocshArg initArg0 = { "portName"          ,iocshArgString};
static const iocshArg initArg1 = { "register file name",iocshArgString};
static const iocshArg initArg2 = { "device name"       ,iocshArgString};
static const iocshArg * const initArgs[] = {&initArg0,
                                            &initArg1,
                                            &initArg2};

static const iocshFuncDef initFuncDef = {"nidaqAsynEpicsConfigure",3,initArgs};
static void initCallFunc(const iocshArgBuf *args)
{
    nidaqAsynEpicsConfigure(args[0].sval, args[1].sval, args[2].sval);
}

void nidaqAsynEpicsRegister(void)
{
    iocshRegister(&initFuncDef,initCallFunc);
}

epicsExportRegistrar(nidaqAsynEpicsRegister);
}//end extern "C"
