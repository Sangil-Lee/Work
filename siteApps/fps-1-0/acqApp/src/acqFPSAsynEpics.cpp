#include "acqFPSAsynEpics.h"

// #include <cstdlib>
// #include <cstring>
// #include <bitset>
// #include <epicsTypes.h>
// #include <epicsTime.h>
// #include <epicsThread.h>
// #include <epicsString.h>
// #include <epicsTimer.h>
// #include <epicsMutex.h>
// #include <epicsEvent.h>
// #include <registryFunction.h>
// #include <epicsExport.h>
// #include <dbCommon.h>
// #include <iocsh.h>
// #include <bitset>


acqFPSAsynEpics::acqFPSAsynEpics(const char *portName, const char* file )
   : asynPortDriver(portName, 
                    1,  /*maxAddr */ 
                    300, /* max index */
                    asynInt32Mask | asynUInt32DigitalMask | asynInt32ArrayMask | asynFloat64Mask | asynInt16ArrayMask | asynFloat64ArrayMask | asynEnumMask | asynOctetMask |asynDrvUserMask, /* Interface mask */
                    asynInt32Mask | asynUInt32DigitalMask | asynInt32ArrayMask | asynFloat64Mask | asynInt16ArrayMask | asynFloat64ArrayMask | asynEnumMask | asynOctetMask,  /* Interrupt mask */
                    1, /* asynFlags.  This driver does not block and it is not multi-device, so flag is 0 */
                    1, /* Autoconnect */
                    0, /* Default priority */
                    0 /* Default stack size*/
				),driverName("acqFPSAsynEpics"), devnode(11), filename(file)
{
    asynStatus status;

    // const char *functionName = "acqFPSAsynEpics";
	pacqFPSDriver = new acqFPSDriver(devnode);
	registerParamListFromFile(filename);

	eventId_ = epicsEventCreate(epicsEventEmpty);

	/* create to asyn thread */
	status = (asynStatus)(epicsThreadCreate("acqFPSAsynEpicsUserTask",
							epicsThreadPriorityMedium,
							epicsThreadGetStackSize(epicsThreadStackMedium),
							(EPICSTHREADFUNC)::asynProcess, this) == NULL);
	if (status) {
		return ;
	}
	
	// setParamValue("DRYDATASTR", "");
	// setParamValue("WETDATASTR", "");
}

int Board_Name_env = 0;
void acqFPSAsynEpics::asynProcess()
{
    double updateTime = 0;
    int run = 1;
	int result = 0;
	memset(&Store_Data, 0, sizeof(Store_Data));

	/**
	 * 	IP 주소를 이용하여 ID 부여
	 */ 
	//string target = getenv("TARGET");
	Board_Name_env = atoi(getenv("TARGET"));
	pacqFPSDriver->WriteUInt32Data(0x0a,Board_Name_env ,Bram1_Idx);
	// bram_ctrl_4200[0x0a] = temp;
	printf("Acquisition ID : %d\n", Board_Name_env);

    // lock();
    while (1) 
	{
		// CheckFile();

		result = StartFPS();

		if(result == 1){
			EventPMS();
		}
        // unlock();
        if(run)     epicsEventWaitWithTimeout(eventId_, updateTime);	//need mutex code
        else        epicsEventWait(eventId_);
        // lock();
    }
}

void asynProcess(void *drvPvt)
{
    acqFPSAsynEpics *pPvt = (acqFPSAsynEpics *)drvPvt;
    pPvt->asynProcess();
}


void acqFPSAsynEpics::registerParamListFromFile(string filename)
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

int acqFPSAsynEpics::checkParam(const string drvname)
{
	int checkval = 0;
	check_iter = regmapfile.find(drvname);

	if(check_iter == regmapfile.end())
		checkval = -1;

	return (checkval);
}

asynStatus acqFPSAsynEpics::createParamNMap(RegMap &reg)
{
    asynStatus status = asynSuccess;
    createParam(reg.drvname, reg.paramtype, &reg.index);
	// printf("Type : %d , Index : %03d , Drv Name : %s\n", reg.paramtype, reg.index, reg.drvname);
	regmapfile[reg.drvname] = reg;
	regmaptable[reg.index] = regmapfile[reg.drvname];

	return (status);
}

/**
 *	Record : longin
 * 	DTYP :	asynInt32
 */  
epicsInt32 acqFPSAsynEpics::readInt32Value(const RegMap &regmap)
{	
	epicsUInt32 value = 0;
	// value = pacqFPSDriver->ReadUInt32Data(regmap.address, Bram1_Idx);
	int temp;
	switch (regmap.address)
	{
		case 0x203:	// PV Name : FileCheck , Period : 2 Sec
			temp = pacqFPSDriver->ReadUInt32Data(PMS_FLAG,Bram1_Idx);
			if(temp == 0){
				char filename[FILE_MAX_SIZE] = {0,};  //include path

				Get_PMS_FileName(filename); // included file path

				value = access(filename, F_OK);   // Check file existence?    existence == 0 / don't existence == Positive integer
				value = (value == 0)? 1:0;
				pms_file_exist_status = value;
				// printf("file check value : %d , file name : %s \n", value, filename);
			}else{
			}
			// printf("file check temp value : %d\n",temp);
			break;

		case 0x0A:	// ID Number
			temp = Board_Name_env;
			// int temp = atoi(getenv("TARGET"));
			pacqFPSDriver->WriteUInt32Data(0x0a,temp ,Bram1_Idx);
			printf("Acquisition ID : %d\n", temp);
			// printf("read Bram : %d \n",pacqFPSDriver->ReadUInt32Data(0x0a,Bram1_Idx));
			value = temp;
			break;
		
		default:
			value = pacqFPSDriver->ReadUInt32Data(regmap.address, Bram1_Idx);
			break;
	}


	return (value);	
}

/**
 *	Record : longout
 * 	DTYP :	asynInt32
 */  
int acqFPSAsynEpics::writeInt32Value(const RegMap &regmap)
{
	epicsInt32 value = 0;
	unsigned int getValue = 0;
	int status = 0;

	if(checkParam(regmap.drvname) == -1) 
		return(-1);

	getIntegerParam(regmap.index, &value);
	getValue = value;

	char nodename[5] 			= {0,};	//get certification
	int temp;
	char scp_buf[FILE_MAX_SIZE] = {0,};
	
	if( (regmap.address == 0x202) || (regmap.address == 0x204) )	// FileSave
	{
		if(getValue && pms_file_exist_status)
		{
			Get_NodeName(nodename);		//AN[0-9][0-9]
			// printf("get nodename (%s)\n", nodename );
			printf(" %s PMS File Name : (%s) \n",nodename, pmsdata_filename);

			//sprintf(scp_buf ,"scp -i /root/.ssh/%s %s jong@192.168.3.250:/home/jong/Escp", nodename, pmsdata_filename);
			// sprintf(scp_buf ,"scp -i /mnt/sdcard/setup/%s %s jong@192.168.3.200:/home/jong/Escp", nodename, pmsdata_filename);
			sprintf(scp_buf ,". /mnt/sdcard/script/.FileSave.sh %s %s &", nodename, pmsdata_filename);
			temp = system(scp_buf);
		}else{
			// printf("FileSave \n");
		}

	}
	else if( regmap.address == 0x201 )	// PMSEN
	{
		// printf("%s value : %d \n",regmap.drvname, getValue);
		if(getValue == 1)
		{
			//bram1[d] pms data clear / pms error time clear
			pacqFPSDriver->WriteUInt32Data(PMS_CLEAR, 1);
			pacqFPSDriver->WriteUInt32Data(PMS_CLEAR, 0);

			///////////////////////////////////////////////////////////////////////////////////////////////////

			temp = pacqFPSDriver->ReadUInt32Data(PMS_FLAG,Bram1_Idx);
			//  printf("read pms_unlock_val : %x \n", temp);
			temp |= (1<<15);
			//  printf("modify pms_unlock_val : %x \n", temp);
			//bram1[0] LSB 1bit pms_flag
			pacqFPSDriver->WriteUInt32Data(PMS_FLAG, temp);
			temp = pacqFPSDriver->ReadUInt32Data(PMS_FLAG,Bram1_Idx);
			//  printf("write & read pms_unlock_val : %x \n", temp);

			/* PMS Port Num Clear */ 
			//NumCount = 0;
		}else{
		}
		
	}
	else{		
		printf("driveName : %s | getValue : %d | address : %x\n",regmap.drvname, getValue, regmap.address);
	}
	// printf(" pms_file_exist_status : %d \n",pms_file_exist_status);

    return  (status);
}

asynStatus acqFPSAsynEpics::readInt32(asynUser *pasynUser, epicsInt32 *value)
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

asynStatus acqFPSAsynEpics::writeInt32(asynUser *pasynUser, epicsInt32 value)
{
	int function = pasynUser->reason;
	asynStatus status = asynSuccess;
	const char *paramName;
	const char* functionName = "writeInt32";

	int addr = 0;
	status = getAddress(pasynUser, &addr); if (status != asynSuccess) return(status);

	// printf("writeInt32))))))))\n");

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


void acqFPSAsynEpics::setIOCStartTime()
{
	epicsTimeStamp current;
	epicsTimeGetCurrent (&current);

	char timebuf[20];
	epicsTimeToStrftime(timebuf,20,"%Y/%m/%d %H:%M:%S", &current);
	setParamValue("TimeStart", timebuf);
}

int acqFPSAsynEpics::setParamValue(const string drvname, const int ival)
{
	int check = checkParam(drvname);
	// printf("int setParamValue :: %d\n", check);	//기본적으로 이 함수만 호출되며 값은 0이므로 문제없다
	if(check == 0)
		setIntegerParam(regmapfile[drvname].index, ival);
	return (check);
}

int acqFPSAsynEpics::setParamValue(const string drvname, const double dval)
{
	int check = checkParam(drvname);
	// printf("name : %s , double setParamValue :: %d\n",drvname, dval);
	if(check == 0)
		setDoubleParam(regmapfile[drvname].index, dval);
	return (check);
}


int acqFPSAsynEpics::setParamValue(const string drvname, const string svalue)
{
	int check = checkParam(drvname);
	// printf("name : %s , string setParamValue :: %d, svalue : %s\n",drvname.c_str(), check, svalue.c_str());
	if(check == 0)
		setStringParam(regmapfile[drvname].index, svalue.c_str());
	return (check);
}

int acqFPSAsynEpics::setParamValue(const string drvname, const epicsUInt32 value, epicsUInt32 mask = 0)
{
	int check = checkParam(drvname);
	if(check == 0)
		setUIntDigitalParam(regmapfile[drvname].index, value, mask);

	return (check);
}

 /* thread 할떄 추가*/
asynStatus acqFPSAsynEpics::getParamValue(const string drvname, int maxChars, char *value )
{
    asynStatus status = asynSuccess;
	getStringParam(regmapfile[drvname].index, maxChars, value);
	return (status);
}

asynStatus acqFPSAsynEpics::getParamValue(const string drvname, int &value)
{
    asynStatus status = asynSuccess;
	// printf("drvname(%s), value(%d), Index(%d)\n", drvname.c_str(), value, regmapfile[drvname].index);
    getIntegerParam(regmapfile[drvname].index, &value);
	return (status);
}

asynStatus acqFPSAsynEpics::getParamValue(const string drvname, double &value)
{
    asynStatus status = asynSuccess;
	getDoubleParam(regmapfile[drvname].index, &value);
	return (status);
}

asynParamType acqFPSAsynEpics::getAsynParamType(const char *paramstring)
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

asynStatus acqFPSAsynEpics::readOctet(asynUser *pasynUser, char *value, size_t maxChars, size_t *nActual, int *eomReason)
{
    int function = pasynUser->reason;
    int addr=0;
    asynStatus status = asynSuccess;
    epicsTimeStamp timeStamp; getTimeStamp(&timeStamp);
    static const char *functionName = "readOctet";
   
    //status = getAddress(pasynUser, &addr); if (status != asynSuccess) return(status);
    /* We just read the current value of the parameter from the parameter library.
     * Those values are updated whenever anything could cause them to change */
    //status = (asynStatus)getStringParam(addr, function, (int)maxChars, value);
    status = getAddress(pasynUser, &addr); if (status != asynSuccess) return(status);
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

    return(status);
}

asynStatus acqFPSAsynEpics::readStringValue(const RegMap &regmap, char *value)
{
    asynStatus status = asynSuccess;

	// if (strcmp(regmap.drvname, "DRYDATASTR") == 0 || 
	// 	strcmp(regmap.drvname, "WETDATASTR") == 0)
	// {
	// 	bitset<32> bitdata =  pacqFPSDriver->ReadUInt32Data(regmap.address, Bram1_Idx);
	// 	//cout << bitdata.to_string() << endl;
	// 	strcpy(value, bitdata.to_string().c_str());
	// };

	return (status);
}

asynStatus acqFPSAsynEpics::writeUInt32Digital(asynUser *pasynUser, epicsUInt32 value, epicsUInt32 mask)
{
	int function = pasynUser->reason;
	int addr=0;
	asynStatus status = asynSuccess;
	const char* functionName = "writeUInt32Digital";

	status = getAddress(pasynUser, &addr); if (status != asynSuccess) return(status);
	
	/* Set the parameter in the parameter library. */
	status = (asynStatus) setUIntDigitalParam(addr, function, value, mask);

	printf("-%s-, value(%d), mask(%d)\n", functionName, value, mask);

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

asynStatus acqFPSAsynEpics::readUInt32Digital(asynUser *pasynUser, epicsUInt32 *value, epicsUInt32 mask)
{
	int function = pasynUser->reason;
	int addr=0;
	asynStatus status = asynSuccess;
	epicsTimeStamp timeStamp; getTimeStamp(&timeStamp);
	static const char *functionName = "readUInt32Digital";

	status = getAddress(pasynUser, &addr); if (status != asynSuccess) return(status);
	/* We just read the current value of the parameter from the parameter library.
	 *      * Those values are updated whenever anything could cause them to change */
	status = (asynStatus) getUIntDigitalParam(addr, function, value, mask);
	/* Set the timestamp */
	pasynUser->timestamp = timeStamp;
	if (status)
		epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize,
				"%s:%s: status=%d, function=%d, value=%u mask=%u",
				driverName, functionName, status, function, *value, mask);
	else
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER,
				"%s:%s: function=%d, value=%u, mask=%u\n",
				driverName, functionName, function, *value, mask);
	return(status);
}


extern "C" {
/** EPICS iocsh callable function to call constructor for the testAsynPortDriver class.
  * \param[in] portName The name of the asyn port driver to be created.
  * \param[in] maxSizeSnapshot The maximum  number of sample in one snapshot
  * \param[in] maxNbSnapshot The number of snapshot buffered
 */
epicsShareFunc int acqFPSAsynEpicsConfigure(const char *portName, const char* registerfile)
{
    new acqFPSAsynEpics(portName, registerfile);
    return(asynSuccess);
}


/* EPICS iocsh shell commands */
static const iocshArg initArg0 = { "portName"          ,iocshArgString};
static const iocshArg initArg1 = { "register file name",iocshArgString};
static const iocshArg * const initArgs[] = {&initArg0,
                                            &initArg1,
                                            };
static const iocshFuncDef initFuncDef = {"acqFPSAsynEpicsConfigure",2,initArgs};
static void initCallFunc(const iocshArgBuf *args)
{
    acqFPSAsynEpicsConfigure(args[0].sval, args[1].sval);
}

void acqFPSAsynEpicsRegister(void)
{
    iocshRegister(&initFuncDef,initCallFunc);
}

epicsExportRegistrar(acqFPSAsynEpicsRegister);
}//end extern "C"
