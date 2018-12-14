#include <cstdlib>
#include <cstring>
#include <sstream>
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
#include <errlog.h>

#include "daqControlAsynEpics.h"

static void userProcess(void *drvPvt);

using daqControl::daqControlDriver;

static int daqControlDebug = 1;
epicsExportAddress(int, daqControlDebug);

static int daqControlPrintCount = 20;
epicsExportAddress(int, daqControlPrintCount);

/** DB Scan for output record **
  * Periodic Scan: Passive, 0.1, 0.2, 0.5, 1, 2, 5, 10 -> overloading function call.
  * To use setParamValue in overload function, should be used Periodic Scan
  * In case I/O Intr, can't call overloading function.
  * Event Scan(epicsEvent): I/O Intr -> setParamValue
 */

daqControlAsynEpics::daqControlAsynEpics(const char *portName, const char* filename, const int slotNum )
	: asynPortDriver(portName, 
			1,  /*maxAddr */ 
            1000,
			asynInt32Mask | asynUInt32DigitalMask | asynFloat64Mask | asynInt16ArrayMask | asynInt32ArrayMask| asynFloat64ArrayMask | asynEnumMask | asynOctetMask |asynDrvUserMask, /* Interface mask */
			asynInt32Mask | asynUInt32DigitalMask | asynFloat64Mask | asynInt16ArrayMask | asynInt32ArrayMask| asynFloat64ArrayMask | asynEnumMask | asynOctetMask,  /* Interrupt mask */
			1, /* asynFlags.  This driver does not block and it is not multi-device, so flag is 0 */
			1, /* Autoconnect */
			0, /* Default priority */
			0 /* Default stack size*/
			),fileName(filename),driverName("daqControlAsynEpics"), sw_event(true) 
{
	epicsPrintf("GCC Version-(%d)\n", GCC_VERSION);
    asynStatus status;
    const char *functionName = "daqControlAsynEpics";

    pdaqControl = new daqControlDriver(1,slotNum);
	
    // Create database entries
    eventId_ = epicsEventCreate(epicsEventEmpty);  

    // register parameter list from register file.  
	registerParamListFromFile(fileName);

	if (sw_event == true) 
	{
		status = (asynStatus)(epicsThreadCreate("daqControlAsynEpicsUserTask",
					epicsThreadPriorityMedium,
					epicsThreadGetStackSize(epicsThreadStackMedium),
					(EPICSTHREADFUNC)::userProcess,
					this) == NULL);
		if (status) {
			epicsPrintf("%s:%s: epicsThreadCreate failure\n", driverName, functionName);
			return;
		}
	} else {
		status = asynError;
	};
}

void daqControlAsynEpics::registerParamListFromFile(string filename)
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

		if(!(pch = strtok (str,", "))) continue;
		strcpy(regmap.drvname, pch);

		if(!(pch = strtok (NULL,", "))) continue;
		regmap.address = strtol(pch,NULL,16);

		if(!(pch = strtok (NULL,", "))) continue;
		regmap.paramtype = getAsynParamType(pch);

		if(!(pch = strtok (NULL,", "))) continue;
		regmap.option = strtoul(pch,NULL,16);

		if(!(pch = strtok (NULL,", "))) continue;
		strcpy(regmap.drvlink, pch);

		createParamNMap(regmap);
		regmap.index++;
	};	

	if(daqControlDebug)
		epicsPrintf("%s\n", "=============================================================================================================================================================================");
}

asynParamType daqControlAsynEpics::getAsynParamType(const char *paramstring)
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

asynStatus daqControlAsynEpics::createParamNMap(RegMap &regmap)
{
	unsigned int IP_Index = 0, D_Addr = 0, D1_Type = 0,
				D2_Type = 0, D3_Type = 0, D2_Offset = 0;

    asynStatus status = asynSuccess;

   	createParam(regmap.drvname, regmap.paramtype, &regmap.index);
	regmaptable[regmap.index] = regmapfile[regmap.drvname] = regmap;

	//IP_Index = (regmap.address >>28) & 0x0000000F;
	IP_Index  = getRShiftMask(regmap.address, 28, 0xF);
	D_Addr	  = getRShiftMask(regmap.address,  0, 0x0FFFFFFF);

	//Option Check (32bits)
	D1_Type   = getRShiftMask(regmap.option, 28, 0xF);	// scan or not check(4bit), High 4bit of 32bit
	D2_Type   = getRShiftMask(regmap.option, 24, 0xF);	// data type 32bit, 16bit, 8bit, 4bit, 2bit, 1bit(4bit)
	D2_Offset = getRShiftMask(regmap.option, 16, 0xFF);  // offset of data type(16bit)
	D3_Type   = getRShiftMask(regmap.option, 12, 0xF);   // normal, pulse, getset, timeType0, timeType1 value(4bit)


#if 0
	getOptionBitset(optbit, regmap.option,  0, 0xFFF);   //12bit to 32 bitset
#else
	getOptionBitset(optbit, regmap.option);				//full option to 32 bitset
#endif
	string strOpt = optbit.to_string();

	stringstream sstream;

	for(size_t i = 1; i < optbit.size()+1; i ++)
	{
		if(i%4 == 0) {
			//cout << strOpt.at(i-1) << ",";
			sstream << strOpt.at(i-1) << ",";
		} else {
			//cout << strOpt.at(i-1);
			sstream << strOpt.at(i-1);
		};
	}
	//cout << endl;
	//sstream << endl;
	string strOption = sstream.str();

	//32 bitset bit check from 0 ~ 11 bit.
	if(regmap.index == 0 && daqControlDebug) {
		epicsPrintf("%s\n", "=============================================================================================================================================================================");
		epicsPrintf("%4s, %-26s, %10s, %5s, %10s, %4s, %4s, %5s, %4s, %-26s, %-31s\n", "index", "drvname", "paramtype", "IP", "Addr", "D1", "D2", "Offset", "D3", "drvlink", "option");
		epicsPrintf("%s\n", "-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------");
	};

	if(daqControlDebug) {
		epicsPrintf("%5d, %-26s, %10d, %5d, 0x%08x, %4d, %4d, %6d, %4d, %-26s, %-31s(0x%08x)\n", regmap.index, regmap.drvname, regmap.paramtype, IP_Index, D_Addr, D1_Type, D2_Type, D2_Offset, D3_Type,  regmap.drvlink, strOption.c_str(),regmap.option);
	};

	return (status);
}

int daqControlAsynEpics::tsMode(const char* mode)
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

epicsInt32 daqControlAsynEpics::readInt32Value(const RegMap &regmap)
{
	epicsInt32 value = 0;
	return (value);	
}

asynStatus daqControlAsynEpics::readValue(const RegMap &regmap, epicsInt32 &value)
{
	//Register address read.
    asynStatus status = asynSuccess;
	unsigned int rdData = 0;
	value = rdData;
	return (status);	
}

asynStatus daqControlAsynEpics::readValue(const RegMap &regmap, epicsFloat64 &value)
{
	epicsFloat64 rdValue = 0.0;
    asynStatus status = asynSuccess;

	value = rdValue;

	return (status);	
}

asynStatus daqControlAsynEpics::writeFloat64Array(asynUser *pasynUser, epicsFloat64 *value, size_t nElements)
{
    int function = pasynUser->reason;
    asynStatus status = asynSuccess;
    epicsTimeStamp timeStamp;
    const char *functionName = "writeFlot64Array";

    getTimeStamp(&timeStamp);
    pasynUser->timestamp = timeStamp;

    RegMap regmap = regmaptable[function];
    vecCodeMap[regmap.drvname].clear();

    if(daqControlDebug > 3)
        epicsPrintf("%s(%s),nEle(%ld)\n",functionName,regmap.drvname, nElements);

    return status;
}

asynStatus daqControlAsynEpics::readInt32(asynUser *pasynUser, epicsInt32 *value)
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
	
	//New
	readValue(regmaptable[function], *value);

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

asynStatus daqControlAsynEpics::writeInt32(asynUser *pasynUser, epicsInt32 value)
{
	int function = pasynUser->reason;
	asynStatus status = asynSuccess;
	const char *paramName;
	const char* functionName = "writeInt32";

	int addr = 0;
	status = getAddress(pasynUser, &addr); if (status != asynSuccess) return(status);

	/* Set the parameter in the parameter library. */
	status = (asynStatus) setIntegerParam(function, value);

	if(daqControlDebug > 3)
		epicsPrintf("[%s] : %d\n",functionName,value);


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

asynStatus daqControlAsynEpics::writeUInt32Digital(asynUser *pasynUser, epicsUInt32 value, epicsUInt32 mask)
{
    int function = pasynUser->reason;
    int addr=0;
    asynStatus status = asynSuccess;
    const char* functionName = "writeUInt32Digital";

    status = getAddress(pasynUser, &addr); if (status != asynSuccess) return(status);

    /* Set the parameter in the parameter library. */
    //status = (asynStatus) setUIntDigitalParam(function, value, mask);

	writeUInt32Value(regmaptable[function], (epicsUInt32)value, mask);

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

int daqControlAsynEpics::writeUInt32Value(const RegMap &regmap, epicsUInt32 value, epicsUInt32 mask)
{
    int status = 0;
	if(checkParam(regmap.drvname) == -1) return(-1);

	if(daqControlDebug > 3)
		epicsPrintf("%s: value:%d\n", regmap.drvname, value);

	setParamValue(regmap.drvname, value, mask);

    return status;
}


int daqControlAsynEpics::checkParam(const string drvname)
{
	int checkval = 0;
	check_iter = regmapfile.find(drvname);

	if(check_iter == regmapfile.end())
		checkval = -1;

	return (checkval);
}

int daqControlAsynEpics::setParamValue(const string drvname, epicsUInt32 value, epicsUInt32 mask)
{
	int check = checkParam(drvname);
	if(check == 0)
		setUIntDigitalParam(regmapfile[drvname].index, value, mask);
	return (check);
}

int daqControlAsynEpics::setParamValue(const string drvname, epicsInt32 value)
{
	int check = checkParam(drvname);
	if(check == 0)
		setIntegerParam(regmapfile[drvname].index, value);
	return (check);
}

int daqControlAsynEpics::setParamValue(const string drvname, epicsFloat64 value)
{
	int check = checkParam(drvname);
	if(check == 0)
		setDoubleParam(regmapfile[drvname].index, value);
	return (check);
}

int daqControlAsynEpics::setParamValue(const string drvname, const string svalue)
{
	int check = checkParam(drvname);
	if(check == 0)
		setStringParam(regmapfile[drvname].index, svalue.c_str());
	return (check);
}

asynStatus daqControlAsynEpics::getParamValue(const string drvname, int maxChars, char *value )
{
    asynStatus status = asynSuccess;
	getStringParam(regmapfile[drvname].index, maxChars, value);
	return (status);
}

asynStatus daqControlAsynEpics::getParamValue(const string drvname, epicsInt32 &value)
{
    asynStatus status = asynSuccess;
    getIntegerParam(regmapfile[drvname].index, &value);
	return (status);
}

asynStatus daqControlAsynEpics::getParamValue(const string drvname, epicsFloat64 &value)
{
    asynStatus status = asynSuccess;
	getDoubleParam(regmapfile[drvname].index, &value);
	return (status);
}

asynStatus daqControlAsynEpics::getParamValue(const string drvname, epicsUInt32 &value, epicsUInt32 mask)
{
    asynStatus status = asynSuccess;
	getUIntDigitalParam(regmapfile[drvname].index, &value, mask);
	return (status);
}


asynStatus daqControlAsynEpics::readStringValue(const RegMap &regmap, char *value)
{
    asynStatus status = asynSuccess;

	char strbuf[150];
	strcpy(value, (char*)strbuf);

	return (status);
}

asynStatus daqControlAsynEpics::readOctet(asynUser *pasynUser, char *value, size_t maxChars, size_t *nActual, int *eomReason)
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

asynStatus daqControlAsynEpics::writeOctet(asynUser *pasynUser, const char *value, size_t maxChars, size_t *nActual)
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

asynStatus daqControlAsynEpics::writeOctetValue(const RegMap &regmap, const char *value)
{
	asynStatus status = asynSuccess;
	if(checkParam(regmap.drvname) == -1) return(asynError);

    return  (status);
}

asynStatus daqControlAsynEpics::writeStringValue(const RegMap &regmap, const char *strValue)
{
	asynStatus status = asynSuccess;
	if(checkParam(regmap.drvname) == -1) return(asynError);

	const char *functionName = "writeStringValue";

                                                            // 0x1 -> wave setup, 0x3-> last index + 0x8000000, Sequence Stop Bit(SEQA, SEQB).
	if(daqControlDebug > 3)
		epicsPrintf("-%s-,Sub_drvname(%s),strValue(%s)\n", functionName, regmap.drvlink, strValue);

    return  (status);
}


int daqControlAsynEpics::writeFloat64Value(const RegMap &regmap, epicsFloat64 value)
{
    int status = 0;
	if(checkParam(regmap.drvname) == -1) return(-1);

	setParamValue(regmap.drvname, value);

    return  (status);
}

int daqControlAsynEpics::writeInt32Value(const RegMap &regmap, epicsInt32 value)
{
    int status = 0;
	if(checkParam(regmap.drvname) == -1) return(-1);

	if(daqControlDebug > 3)
		epicsPrintf("%s: value:%d\n", regmap.drvname, value);

	if(strcmp (regmap.drvname, "USER_TASK_RUN") == 0)
		epicsEventSignal(eventId_);
	else
		setParamValue(regmap.drvname, value);

	static int cnt = 0;
	setParamValue("EV_SEQTRG_EN_0", cnt++);		//Temporary Test.

    return  (status);
}

asynStatus daqControlAsynEpics::writeFloat64(asynUser *pasynUser, epicsFloat64 value)
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
	writeFloat64Value(regmaptable[function], value);
    
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

void daqControlAsynEpics::getOptionBitset(OptionBit &optionbit, epicsUInt32 option, epicsUInt32 shift, epicsUInt32 mask)
{
	optionbit = (mask == 0) ? (option >> shift) : (option >> shift) & mask;
}

epicsUInt32 daqControlAsynEpics::getRShiftMask(epicsUInt32 option, epicsUInt32 shift, epicsUInt32 mask = 0)
{
	return (mask == 0) ? (option >> shift) : (option >> shift) & mask;
}

epicsUInt32 daqControlAsynEpics::getLShiftMask(epicsUInt32 option, epicsUInt32 shift, epicsUInt32 mask)
{
	return (mask == 0) ? (option << shift) : (mask << shift) | option;
}

epicsUInt32 daqControlAsynEpics::setBitOnOff(epicsUInt32 option, epicsUInt32 nbit, bool bit)
{
	epicsUInt32 retVal = (bit==1) ? option |= 1 << nbit : option &= ~(1 << nbit);
	return (retVal);
}

epicsUInt32 daqControlAsynEpics::setExclusiveBit(epicsUInt32 option, epicsUInt32 nbit)
{
	return (option ^= 1 << nbit);
}

bool daqControlAsynEpics::checkBit(epicsUInt32 option, epicsUInt32 nbit)
{
	return ((option >> nbit)&1);
}


void daqControlAsynEpics::userProcess()
{
	//struct timespec vartime;
	//long time_elapsed_nanos;
	int run = 1;
	double updateTime = 1.0;

	lock();

	/* Loop forever 
	 * If the logic will be able to block into scan thread in IOC, this thread should work in IOC.
	 */    

	//Temporary Test
	//static int count = 0;
	while (1) { 

		getParamValue("USER_TASK_RUN", run);
		getParamValue("USER_TASK_UPDATETIME", updateTime);

		unlock();
		if(run) epicsEventWaitWithTimeout(eventId_, updateTime);
		else (void) epicsEventWait(eventId_);
		lock();

		getParamValue("USER_TASK_RUN", run);
		if(!run)		continue;

		//Waked Event
		//SCAN = I/O Intr, Temporary Test
		//epicsPrintf("User Process Called(%d)\n", count++);
		//setParamValue("EV_SEQTRG_EN_0", count);
	};
}

void userProcess(void *drvPvt)
{
	daqControlAsynEpics *pPvt = (daqControlAsynEpics *)drvPvt;

	pPvt->userProcess();
}

extern "C" {

/** EPICS iocsh callable function to call constructor for the testAsynPortDriver class.
  * \param[in] portName The name of the asyn port driver to be created.
  * \param[in] maxSizeSnapshot The maximum  number of sample in one snapshot
  * \param[in] maxNbSnapshot The number of snapshot buffered
 */
epicsShareFunc int daqControlAsynEpicsConfigure(const char *portName, const char* registerfile, const int slotNum)
{
    new daqControlAsynEpics(portName, registerfile, slotNum);
    return(asynSuccess);
}


/* EPICS iocsh shell commands */
static const iocshArg initArg0 = { "portName"          ,iocshArgString};
static const iocshArg initArg1 = { "register file name",iocshArgString};
static const iocshArg initArg2 = { "slot number",		iocshArgInt};
static const iocshArg * const initArgs[] = {&initArg0,
                                            &initArg1,
                                            &initArg2};
static const iocshFuncDef initFuncDef = {"daqControlAsynEpicsConfigure",3,initArgs};
static void initCallFunc(const iocshArgBuf *args)
{
    daqControlAsynEpicsConfigure(args[0].sval, args[1].sval, args[2].ival);
}

void daqControlAsynEpicsRegister(void)
{
    iocshRegister(&initFuncDef,initCallFunc);
}

epicsExportRegistrar(daqControlAsynEpicsRegister);
}//end extern "C"
