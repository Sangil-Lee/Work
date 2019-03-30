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

#include "asynDriverSupport.h"

static int asynDebug = 1;
epicsExportAddress(int, asynDebug);

static int asynPrintCount = 20;
epicsExportAddress(int, asynPrintCount);

asynDriverSupport::asynDriverSupport(const char *portName, const char* filename )
   : asynPortDriver(portName, 
                    1,  /*maxAddr */ 
                    150,
                    asynInt32Mask | asynFloat64Mask | asynInt8ArrayMask| asynInt32ArrayMask | asynFloat64ArrayMask | asynEnumMask | asynOctetMask |asynDrvUserMask, /* Interface mask */
                    asynInt32Mask | asynFloat64Mask | asynInt8ArrayMask| asynInt32ArrayMask | asynFloat64ArrayMask | asynEnumMask | asynOctetMask,  /* Interrupt mask */
                    1, /* asynFlags.  This driver does not block and it is not multi-device, so flag is 0 */
                    1, /* Autoconnect */
                    0, /* Default priority */
                    0 /* Default stack size*/
										)
{
    const char *functionName = "asynDriverSupport";

	if(asynDebug)
		printf("In %s \n", functionName);

	registerParamListFromFile(filename);
}

void asynDriverSupport::registerParamListFromFile(string filename)
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

		strcpy(regmap.drvname, pch);
		while (pch != NULL)
		{
			if(!(pch = strtok (NULL," \t,"))) continue;
			regmap.address = strtol(pch,NULL,16);
			if(!(pch = strtok (NULL," \t,"))) continue;
			regmap.paramtype = getAsynParamType(pch);
		};
		regmap.index = 0;
		createParamNMap(regmap);
	};	

	setIOCStartTime();
}

void asynDriverSupport::setIOCStartTime()
{
	epicsTimeStamp current;
	epicsTimeGetCurrent (&current);
	char timebuf[20];
	epicsTimeToStrftime(timebuf,20,"%Y/%m/%d %H:%M:%S", &current);
	//setParamValue("IOCSTARTTIME", timebuf);
}

asynStatus asynDriverSupport::writeInt32Array(asynUser *pasynUser, epicsInt32 *value, size_t nElements)
{
	if(asynDebug)
		printf("NELM(%ld)\n", nElements);
    asynStatus status = asynSuccess;
	for(size_t i = 0; i < (size_t)nElements;i++)
	{
		value[i]=value[i];
		if(asynDebug)
			printf("Value[%ld]:%d ",i, value[i]);
	}
	if(asynDebug)
		printf("\n");

    return status;
}

asynStatus asynDriverSupport::readInt8Array(asynUser *pasynUser, epicsInt8 *value, size_t nElements, size_t *nIn)
{
    int function = pasynUser->reason;
    asynStatus status = asynSuccess;
    epicsTimeStamp timeStamp;
    const char *functionName = "readInt8Array";

	if(asynDebug)
		printf("In %s\n", functionName);

    getTimeStamp(&timeStamp);
    pasynUser->timestamp = timeStamp;

	RegMap regmap = regmaptable[function];

	if (strcmp(regmap.drvname,"") == 0 )
	{
	}

    return status;
}

asynStatus asynDriverSupport::readInt32Array(asynUser *pasynUser, epicsInt32 *value, size_t nElements, size_t *nIn)
{
    int function = pasynUser->reason;
    asynStatus status = asynSuccess;
    epicsTimeStamp timeStamp;
    const char *functionName = "readInt32Array";

    getTimeStamp(&timeStamp);
    pasynUser->timestamp = timeStamp;

	RegMap regmap = regmaptable[function];
	vecCodeMap[regmap.drvname].clear();

	if (strcmp(regmap.drvname,"EV_SEQA_CFG")	== 0 || strcmp(regmap.drvname,"EV_SEQA_TSTAMP") == 0 ||
		strcmp(regmap.drvname,"EV_SEQB_CFG")	== 0 || strcmp(regmap.drvname,"EV_SEQB_TSTAMP") == 0 ||
		strcmp(regmap.drvname,"EV_MAP_RAM")		== 0 )
	{
		vecCode uval;
		for(size_t i = 0; i < (size_t)nElements;i++)
		{
			if(value[i] <= 0) break;
			uval.push_back((value[i] & 0x7ff));
			if(asynDebug && i < (size_t)asynPrintCount)
				printf("%s(%s)-[%ld]:LONG(0x%03lx),Value(%d)\n",functionName,regmap.drvname, i, uval.at(i), value[i]);
		};
		vecCodeMap[regmap.drvname] = uval;
	}
    return status;
}

asynParamType asynDriverSupport::getAsynParamType(const char *paramstring)
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
int asynDriverSupport::setParamValue(const string drvname, const int ival)
{
	int check = checkParam(drvname);
	if(check == 0)
		setIntegerParam(regmapfile[drvname].index, ival);
	return (check);
}

int asynDriverSupport::setParamValue(const string drvname, const double dval)
{
	int check = checkParam(drvname);
	if(check == 0)
		setDoubleParam(regmapfile[drvname].index, dval);
	return (check);
}

int asynDriverSupport::setParamValue(const string drvname, const string svalue)
{
	int check = checkParam(drvname);
	if(check == 0)
		setStringParam(regmapfile[drvname].index, svalue.c_str());
	return (check);
}

asynStatus asynDriverSupport::getParamValue(const string drvname, int maxChars, char *value )
{
    asynStatus status = asynSuccess;
	getStringParam(regmapfile[drvname].index, maxChars, value);
	return (status);
}

asynStatus asynDriverSupport::getParamValue(const string drvname, int &value)
{
    asynStatus status = asynSuccess;
    getIntegerParam(regmapfile[drvname].index, &value);
	return (status);
}

asynStatus asynDriverSupport::getParamValue(const string drvname, double &value)
{
    asynStatus status = asynSuccess;
	getDoubleParam(regmapfile[drvname].index, &value);
	return (status);
}

asynStatus asynDriverSupport::createParamNMap(RegMap &reg)
{
    asynStatus status = asynSuccess;
    createParam(reg.drvname, reg.paramtype, &reg.index);
	regmapfile[reg.drvname] = reg;
	regmaptable[reg.index] = regmapfile[reg.drvname];

	return (status);
}

int asynDriverSupport::checkParam(const string drvname)
{
	int checkval = 0;
	check_iter = regmapfile.find(drvname);
	if(check_iter == regmapfile.end()) checkval = -1;
	return (checkval);
}

asynStatus asynDriverSupport::readInt32(asynUser *pasynUser, epicsInt32 *value)
{
    int function = pasynUser->reason;
    asynStatus status = asynSuccess;
    epicsTimeStamp timeStamp; getTimeStamp(&timeStamp);

    static const char *functionName = "readInt32";
    
    /* We just read the current value of the parameter from the parameter library.
     * Those values are updated whenever anything could cause them to change */
    status = (asynStatus) getIntegerParam(function, value);
    /* Set the timestamp */
    pasynUser->timestamp = timeStamp;
	
	readValue(regmaptable[function], *value);

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

asynStatus asynDriverSupport::readValue(const RegMap &regmap, epicsInt32 &value)
{
	//Register address read.
	//....
	epicsInt32	rdValue = 0;
    asynStatus status = asynSuccess;
	const char* functionName = regmap.drvname;

	if (strcmp(regmap.drvname, "SYS_SEQA_CFG_SET") == 0 ||
		strcmp(regmap.drvname, "SYS_SEQB_CFG_SET") == 0 )
	{
		//caput Ctrl:Asyn:Driver:EV_SEQA_CFG_SET.PROC 1
		//caput Ctrl:Asyn:Driver:EV_SEQB_CFG_SET.PROC 1
		string drvname = string(regmap.drvname);
		string sub_drvname = string("EV") + drvname.substr(3,drvname.size()-7);
		vecCode uval = vecCodeMap[sub_drvname];

		if(asynDebug)
			printf("-%s-,Sub_drvname(%s), Size(%ld)\n", functionName, sub_drvname.c_str(), uval.size());

		if(uval.size() == 0 ) return (status);

		unsigned long mergebit = 0;
		for(size_t i = 0; i < uval.size();i++)
		{
			mergebit = uval.at(i);
			if(asynDebug && i < (size_t)asynPrintCount)
				printf("%s(%s)-[%ld]:(0x%03lx),Value(%ld)\n",functionName,sub_drvname.c_str(), i, mergebit, mergebit);
		};
		size_t count = uval.size()-1;
		mergebit = uval.at(count) + 0x80000000;
		rdValue = 1;
		if(asynDebug)
			printf("%s-Last[%ld]:(0x%03lx),Value(%ld)\n",functionName, count, mergebit, mergebit);
	}
	else if (strcmp(regmap.drvname, "SYS_SEQA_TSTAMP_SET") == 0 ||
			 strcmp(regmap.drvname, "SYS_SEQB_TSTAMP_SET") == 0 ||
			 strcmp(regmap.drvname, "SYS_MAP_RAM_SET")	== 0 )
	{
		//caput Ctrl:Asyn:Driver:EV_SEQA_TSTAMP_SET.PROC 1 ||
		//caput Ctrl:Asyn:Driver:EV_SEQB_TSTAMP_SET.PROC 1
		string drvname = string(regmap.drvname);
		string sub_drvname = string("EV") + drvname.substr(3,drvname.size()-7);

		vecCode uval = vecCodeMap[sub_drvname];

		if(asynDebug)
			printf("-%s-,Sub_drvname(%s), Size(%ld)\n", functionName, sub_drvname.c_str(), uval.size());

		if(uval.size() == 0 ) return (status);

		unsigned long mergebit = 0;
		for(size_t i = 0; i < uval.size();i++)
		{
			mergebit = uval.at(i);
			if(asynDebug && i < (size_t)asynPrintCount)
				printf("%s(%s)-[%ld]:(0x%03lx),Value(%ld)\n",functionName,sub_drvname.c_str(), i, mergebit, mergebit);
		};
		rdValue = 1;
	};

	value = rdValue;
	return (status);	
}

asynStatus asynDriverSupport::readValue(const RegMap &regmap, epicsFloat64 &value)
{
	//Register address read.
	//....
	epicsFloat64 rdValue = 0.0;

    asynStatus status = asynSuccess;

	if (strcmp(regmap.drvname, "") == 0 )
	{
	} 

	value = rdValue;

	return (status);	
}

asynStatus asynDriverSupport::readOctet(asynUser *pasynUser, char *value, size_t maxChars, size_t *nActual, int *eomReason)
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
	readValue(regmaptable[function], value);
		
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

asynStatus asynDriverSupport::readValue(const RegMap &regmap, char *value)
{
    asynStatus status = asynSuccess;

	if (strcmp(regmap.drvname, "BUILD_TIME") == 0 )
	{
		//ptiming->ts2ip_rd(evfile, regmap.address, (unsigned int*)&rdData);
		//BCDCode for time structure register, Sec(6bit)/Min(6bit)/Hour(5bit), Day(5bit)/Hour(4bit)/Year(5bit)
		char timebuf[20];
		strcpy(value, (char*)timebuf);
	}
	return (status);
}

int asynDriverSupport::writeValue(const RegMap & regmap)
{
	int status = 0;

	if( -1 == checkParam(regmap.drvname) ) return(-1);

	switch(regmap.paramtype)
	{
		case asynParamInt32:
		case asynParamUInt32Digital:
			{
				epicsInt32 value = 0;
				getIntegerParam(regmap.index, &value);

				//register write.
				// special function
				if(strcmp(regmap.drvname, "") == 0)
				{
					return (status);
				};
				//normal write
			};
			break;
		case asynParamFloat64:
			{
				epicsFloat64 value = 0.0;
				getDoubleParam(regmap.index, &value);

				//register write.
				// special function
				if(strcmp(regmap.drvname, "") == 0)
				{
					
					return (status);
				};
				//normal write
			};
			break;
		case asynParamOctet:
			{
				char value[64] = "";
				getStringParam(regmap.index, 64, value);

				//register write.
				// special function
				if(strcmp(regmap.drvname, "") == 0)
				{
					return (status);
				};
				//normal write
			};
			break;
		case asynParamInt8Array:
			break;
		case asynParamInt16Array:
			break;
		case asynParamInt32Array:
			break;
		case asynParamFloat32Array:
			break;
		case asynParamFloat64Array:
			{
			};
			break;

		case asynParamNotDefined:
		default:

			break;
	};



	return (status);
}


extern "C" {
/** EPICS iocsh callable function to call constructor for the testAsynPortDriver class.
  * \param[in] portName The name of the asyn port driver to be created.
  * \param[in] maxSizeSnapshot The maximum  number of sample in one snapshot
  * \param[in] maxNbSnapshot The number of snapshot buffered
 */
epicsShareFunc int asynDriverSupportConfigure(const char *portName, const char* registerfile)
{
    new asynDriverSupport(portName, registerfile);
    return(asynSuccess);
}


/* EPICS iocsh shell commands */
static const iocshArg initArg0 = { "portName"          ,iocshArgString};
static const iocshArg initArg1 = { "register file name",iocshArgString};
static const iocshArg * const initArgs[] = { &initArg0, &initArg1 };
static const iocshFuncDef initFuncDef = {"asynDriverSupportConfigure",2,initArgs};
static void initCallFunc(const iocshArgBuf *args)
{
    asynDriverSupportConfigure(args[0].sval, args[1].sval);
}

void asynDriverSupportRegister(void)
{
    iocshRegister(&initFuncDef,initCallFunc);
}

epicsExportRegistrar(asynDriverSupportRegister);
}//end extern "C"
