#include "mitiFPSAsynEpics.h"

/* intech define */

#define def_drvname_strcmp(str) ( strcmp(regmap.drvname, str) == 0 )

void asynProcess(void *drvPvt)
{
    mitiFPSAsynEpics *pPvt = (mitiFPSAsynEpics *)drvPvt;
    pPvt->asynProcess();
}

mitiFPSAsynEpics::mitiFPSAsynEpics(const char *portName, const char* file )
   : asynPortDriver(portName, 
                    1,  /*maxAddr */ 
                    250, /* max index */
                    asynInt32Mask | asynFloat64Mask | asynInt16ArrayMask | asynFloat64ArrayMask | asynEnumMask | asynOctetMask |asynDrvUserMask, /* Interface mask */
                    asynInt32Mask | asynFloat64Mask | asynInt16ArrayMask | asynFloat64ArrayMask | asynEnumMask | asynOctetMask,  /* Interrupt mask */
                    1, /* asynFlags.  This driver does not block and it is not multi-device, so flag is 0 */
                    1, /* Autoconnect */
                    0, /* Default priority */
                    0 /* Default stack size*/
				),driverName("mitiFPSAsynEpics"), devnode(12), filename(file)
{
    asynStatus status;
    // const char *functionName = "mitiFPSAsynEpics";
	pmitiFPSDriver = new mitiFPSDriver(devnode);
	registerParamListFromFile(filename);

	eventId_ = epicsEventCreate(epicsEventEmpty);

	status = (asynStatus)(
		epicsThreadCreate(
			"mitiFPSAsynEpicsUserTask",
			epicsThreadPriorityMedium,
			epicsThreadGetStackSize(epicsThreadStackMedium),
			(EPICSTHREADFUNC)::asynProcess, this
		) == NULL
	);
	if (status) {
		return ;
	}

	// const char *functionName = "mitiFPSAsynEpics";	
}

void mitiFPSAsynEpics::asynProcess()
{
    double updateTime = 0;
    int run = 1;

	Board_Name_env = atoi(getenv("TARGET"));
	pmitiFPSDriver->WriteUInt32Data(0x1B, Bram0_Idx, Board_Name_env);
	// bram_ctrl_4200[0x0a] = temp;
	printf("Mitigation ID : %d\n", Board_Name_env);

    lock();
    while (1) 
	{
        unlock();
		
        if(run)     
			epicsEventWaitWithTimeout(eventId_, updateTime);	//need mutex code
        else        
			epicsEventWait(eventId_);
		
        lock();
    }
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
	setParamValue("TimeStart", timebuf);
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

/**
 * record type	: longin 
 * DTYP			: asynInt32
 */
asynStatus mitiFPSAsynEpics::readInt32(asynUser *pasynUser, epicsInt32 *value)
{
    int function = pasynUser->reason;
    int addr=0;
    asynStatus status = asynSuccess;
    epicsTimeStamp timeStamp; getTimeStamp(&timeStamp);
    static const char *functionName = "readInt32";
	const char *paramName;

    status = getAddress(pasynUser, &addr); if (status != asynSuccess) return(status);
    
    /* We just read the current value of the parameter from the parameter library.
     * Those values are updated whenever anything could cause them to change */
    status = (asynStatus) getIntegerParam(addr, function, value);
    /* Set the timestamp */

    pasynUser->timestamp = timeStamp;
	
	RegMap regmap = regmaptable[function];

	getParamName(function, &paramName);
	// printf("writeInt32 : driverName(%s) functionName(%s) paramName(%s)\n", driverName, functionName, paramName);

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

/**
 * record type	: longin 
 * DTYP			: asynInt32
 */
epicsInt32 mitiFPSAsynEpics::readInt32Value(const RegMap &regmap)
{
	epicsUInt32 value = 0;
	unsigned int get_BramAddr = regmap.address & 0xF00;
	switch (get_BramAddr)
	{
		case 0x000:
				//ChStatR
				value = pmitiFPSDriver->ReadUInt32Data(regmap.address, Bram1_Idx);
			break;
		case 0x200:
				value = pmitiFPSDriver->ReadUInt32Data(regmap.address, Bram0_Idx);
				if(regmap.address == 0x20c){		// TempR
					if(value < 20 || value > 37)
					{
						printf("Error Temp Value (%d), 0x%x \n", value,value);
						value = OLD_TEMP;
						return (value);
					}
					OLD_TEMP = value;
				}else if(regmap.address == 0x21B){	// IDN
					int temp = Board_Name_env;
					pmitiFPSDriver->WriteUInt32Data(0x1B, Bram0_Idx, temp);
					printf("Mitigation ID : %d\n", temp);
					value = temp;
				}else{
					//FwVer	, LinkErr , GTH_LINK_STATUS , PMS_DATA_INTERLOCK
				}
			break;
		case 0x600:
				// not have
				value = pmitiFPSDriver->ReadUInt32Data(regmap.address, Bram2_Idx);
			break;
		
		default:
			break;
	}

	return (value);	
}

/**
 * 	record type	: longout
 *  DTYP 		: asynInt32
 */ 
asynStatus mitiFPSAsynEpics::writeInt32(asynUser *pasynUser, epicsInt32 value)
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

	// printf("writeInt32 : name (%s)\n", paramName);

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

/**
 * 	record type	: longout
 *  DTYP 		: asynInt32
 */ 
int mitiFPSAsynEpics::writeInt32Value(const RegMap &regmap)
{
	epicsInt32 value = 0;
	unsigned int getValue = 0;
	int status = 0;

	if(checkParam(regmap.drvname) == -1) return(-1);

	getIntegerParam(regmap.index, &value);
	getValue = value;
	
	int setBram_Addr = (regmap.address & 0xF00);
	switch (setBram_Addr)
	{
		case 0x000:
				pmitiFPSDriver->WriteUInt32Data(regmap.address, Bram1_Idx, getValue);
				//printf("0x4000 driveName : %s | getValue : %x | address : 0x%x\n",regmap.drvname, getValue, regmap.address);
			break;
			
		case 0x300:		// AllFileSave Btn
				// printf("0x300 driveName : %s | getValue : %x | address : 0x%x\n",regmap.drvname, getValue, regmap.address);
			break;

		case 0x200:
				if(regmap.address == 0x208){		// OpticSwRst Btn
					pmitiFPSDriver->WriteUInt32Data(regmap.address, Bram0_Idx, ((getValue == 1)?0xF:getValue) );

				}else if(regmap.address == 0x207){	//	DEVICE_RELEASE Btn
					pmitiFPSDriver->WriteUInt32Data(regmap.address, Bram0_Idx, getValue);
					printf("0x4200 driveName : %s | getValue : %x | address : 0x%x\n",regmap.drvname, getValue, regmap.address);

				}else{
					pmitiFPSDriver->WriteUInt32Data(regmap.address, Bram0_Idx, getValue);
				}
				// printf("0x4200 driveName : %s | getValue : %x | address : 0x%x\n",regmap.drvname, getValue, regmap.address);
			break;

		case 0x600:
				// Link & Node & Channel Mask
				pmitiFPSDriver->WriteUInt32Data(regmap.address, Bram2_Idx, getValue);
				//printf("0x4600 driveName : %s | getValue : %x | address : 0x%x\n",regmap.drvname, getValue, regmap.address);
			break;
		
		default:
			printf("default driveName : %s | getValue : %x | address : 0x%x\n",regmap.drvname, getValue, regmap.address);
			break;
	}

	return  (status);
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
	// printf(" index : %d , value  : %d status : %d\n", regmapfile[drvname].index , value, status);
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
	// printf("Type : %d , Index : %03d ,  Drv Name : %s\n", reg.paramtype, reg.index, reg.drvname);
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

/**
 * 	record type	: stringin
 *  DTYP 		: asynOctetRead
 */ 
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

/**
 * 	record type	: stringin
 *  DTYP 		: asynOctetRead
 */ 
int mitiFPSAsynEpics::readValue(const RegMap &regmap, char *value)
{
    int status = asynSuccess;

	// Drv Name : TimeStart

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
