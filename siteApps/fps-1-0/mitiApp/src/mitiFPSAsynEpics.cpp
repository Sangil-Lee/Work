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

#include "mitiFPSAsynEpics.h"

mitiFPSAsynEpics::mitiFPSAsynEpics(const char *portName, const char* filename )
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
    asynStatus status;
    const char *functionName = "mitiFPSAsynEpics";

	registerParamListFromFile(filename);
}

void mitiFPSAsynEpics::registerParamListFromFile(string filename)
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
}

void mitiFPSAsynEpics::setIOCStartTime()
{
	epicsTimeStamp current;
	epicsTimeGetCurrent (&current);
	char timebuf[20];
	epicsTimeToStrftime(timebuf,20,"%Y/%m/%d %H:%M:%S", &current);
	//setParamValue("IOCSTARTTIME", timebuf);
}

asynParamType mitiFPSAsynEpics::getAsynParamType(const char *paramstring)
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
int mitiFPSAsynEpics::setParamValue(const string drvname, const int ival)
{
	int check = checkParam(drvname);
	if(check == 0)
		setIntegerParam(regmapfile[drvname].index, ival);
	return (check);
}

int mitiFPSAsynEpics::setParamValue(const string drvname, const double dval)
{
	int check = checkParam(drvname);
	if(check == 0)
		setDoubleParam(regmapfile[drvname].index, dval);
	return (check);
}

int mitiFPSAsynEpics::setParamValue(const string drvname, const string svalue)
{
	int check = checkParam(drvname);
	if(check == 0)
		setStringParam(regmapfile[drvname].index, svalue.c_str());
	return (check);
}

asynStatus mitiFPSAsynEpics::getParamValue(const string drvname, int maxChars, char *value )
{
    asynStatus status = asynSuccess;
	getStringParam(regmapfile[drvname].index, maxChars, value);
	return (status);
}

asynStatus mitiFPSAsynEpics::getParamValue(const string drvname, int &value)
{
    asynStatus status = asynSuccess;
    getIntegerParam(regmapfile[drvname].index, &value);
	return (status);
}

asynStatus mitiFPSAsynEpics::getParamValue(const string drvname, double &value)
{
    asynStatus status = asynSuccess;
	getDoubleParam(regmapfile[drvname].index, &value);
	return (status);
}

asynStatus mitiFPSAsynEpics::createParamNMap(RegMap &reg)
{
    asynStatus status = asynSuccess;
    createParam(reg.drvname, reg.paramtype, &reg.index);
	regmapfile[reg.drvname] = reg;
	regmaptable[reg.index] = regmapfile[reg.drvname];

	//printf("RegMap.drvname(%s),RegMap.paramtype(%d), RegMap.index(%d)\n",reg.drvname, reg.paramtype, reg.index);
	return (status);
}

int mitiFPSAsynEpics::checkParam(const string drvname)
{
	int checkval = 0;
	check_iter = regmapfile.find(drvname);

	if(check_iter == regmapfile.end())
		checkval = -1;

	return (checkval);
}

asynStatus mitiFPSAsynEpics::readInt32(asynUser *pasynUser, epicsInt32 *value)
{
    int function = pasynUser->reason;
    int addr=0;
    asynStatus status = asynSuccess;
    epicsTimeStamp timeStamp; getTimeStamp(&timeStamp);

    static const char *functionName = "readInt32";
    
    /* We just read the current value of the parameter from the parameter library.
     * Those values are updated whenever anything could cause them to change */
    status = (asynStatus) getIntegerParam(function, value);
    /* Set the timestamp */
    pasynUser->timestamp = timeStamp;
	
	//RegMap regmap = regmaptable[function];
	//*value = readInt32Value(regmaptable[function]);
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

int mitiFPSAsynEpics::readValue(const RegMap &regmap, epicsInt32 &value)
{
	//Register address read.
	//....

	//epicsInt32	rdValue;
	unsigned int	rdValue;

	if (strcmp(regmap.drvname, "") == 0 )
	{
		bitset<sizeof(rdValue)> bitval(rdValue);
		bitval = (bitval>>16);
		bitval &= 0xffff;
		value = bitval.to_ulong();
	};

	value = rdValue;

	return (0);	
}

int mitiFPSAsynEpics::readValue(const RegMap &regmap, epicsFloat64 &value)
{
	//Register address read.
	//....
	epicsFloat64 rdValue = 0.0;


	if (strcmp(regmap.drvname, "") == 0 )
	{
	} 

	value = rdValue;

	return (0);	
}

asynStatus mitiFPSAsynEpics::readOctet(asynUser *pasynUser, char *value, size_t maxChars, size_t *nActual, int *eomReason)
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

int mitiFPSAsynEpics::readValue(const RegMap &regmap, char *value)
{
    int status = asynSuccess;

	if (strcmp(regmap.drvname, "BUILD_TIME") == 0 )
	{
		//ptiming->ts2ip_rd(evfile, regmap.address, (unsigned int*)&rdData);
		//BCDCode for time structure register, Sec(6bit)/Min(6bit)/Hour(5bit), Day(5bit)/Hour(4bit)/Year(5bit)
		char timebuf[20];
		strcpy(value, (char*)timebuf);
	}
	return (status);
}

int mitiFPSAsynEpics::writeValue(const RegMap & regmap)
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
epicsShareFunc int mitiFPSAsynEpicsConfigure(const char *portName, const char* registerfile)
{
    new mitiFPSAsynEpics(portName, registerfile);
    return(asynSuccess);
}


/* EPICS iocsh shell commands */
static const iocshArg initArg0 = { "portName"          ,iocshArgString};
static const iocshArg initArg1 = { "register file name",iocshArgString};
static const iocshArg * const initArgs[] = { &initArg0, &initArg1 };
static const iocshFuncDef initFuncDef = {"mitiFPSAsynEpicsConfigure",2,initArgs};
static void initCallFunc(const iocshArgBuf *args)
{
    mitiFPSAsynEpicsConfigure(args[0].sval, args[1].sval);
}

void mitiFPSAsynEpicsRegister(void)
{
    iocshRegister(&initFuncDef,initCallFunc);
}

epicsExportRegistrar(mitiFPSAsynEpicsRegister);
}//end extern "C"
