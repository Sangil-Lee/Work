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

pthread_t m_Tid;
void* threadFunc( void* pTr );

int threadStart(void* pTr )
{
	int rc = pthread_create (&m_Tid, NULL, threadFunc, pTr);
	if( rc != 0 )
		return -1;

	printf("threadStart\r\n");
	return 0;
}

void threadStop()
{
	printf("threadStop\r\n");
	pthread_cancel (m_Tid);
	printf("threadStop\r\n");
}


void* threadFunc( void* pTr )
{
	printf("threadFunc1\r\n");
	((timing::timingDriver *)pTr)->start_thread();
	printf("threadFunc2\r\n");
	pthread_exit(0);
	printf("threadFunc3\r\n");
}
static int timingDebug = 1;
epicsExportAddress(int, timingDebug);

static int timingPrintCount = 20;
epicsExportAddress(int, timingPrintCount);


timingAsynEpics::timingAsynEpics(const char *portName, int maxSizeSnapshot, int maxSizeBufferMB, int clientMode, const char* filename, const char *deviceName, const int deviceNum )
	: asynPortDriver(portName, 
			1,  /*maxAddr */ 
			/*0xC600, maxAddr*/
                    15000,
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
	tsnum  = deviceNum;
	printf("TSMode(%d-%d)\n", tsmode,tsnum);

    //ptiming = (struct timingDriver *)malloc(sizeof(timingDriver));
    //ptiming = (timingDriver *)calloc(1,sizeof(timingDriver));
    ptiming = new timingDriver(tsmode,deviceNum);
	
	if(ptiming->isInit != 1) return;

    // Create database entries
    eventId_ = epicsEventCreate(epicsEventEmpty);  

    // register parameter list from register file.  
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
	char str[200];
	char *pch;

	regmap.index = 0;

	while(!file.eof())
	{
		getline(file, strToken);
		if(strToken[0] == '#' || strToken.empty()==true) continue;

		strcpy (str, strToken.c_str());
		// printf("strToken.size : %d\n",strToken.size());

		memset(&regmap,0, sizeof(RegMap));

		if(!(pch = strtok (str,", "))) continue;
		// printf("ALIAS_Name(%s)\n", pch);
		strcpy(regmap.drvname, pch);

		if(!(pch = strtok (NULL,", "))) continue;
		// printf("address(%s)\n", pch);
		regmap.address = strtol(pch,NULL,16);

		if(!(pch = strtok (NULL,", "))) continue;
		// printf("paramtype(%s)\n", pch);
		regmap.paramtype = getAsynParamType(pch);

		if(!(pch = strtok (NULL,", "))) continue;
		// printf("option1(%s)\n", pch);
		regmap.option1 = strtol(pch,NULL,16);

		if(!(pch = strtok (NULL,", "))) continue;
		// printf("option2(%s)\n", pch);
		regmap.option2 = strtol(pch,NULL,16);
		
		if(!(pch = strtok (NULL,", "))) continue;
		// printf("drvLink(%s)\n", pch);
		strcpy(regmap.drvLink, pch);

		// printf("%s:index(%d),addr(0x%08x),paramtype(%s),option1(0x%08x),option2(0x%08x),drvLink(%s)\n", regmap.drvname, regmap.index, regmap.address, regmap.paramtype, regmap.option1, regmap.option2, regmap.drvLink);
//		printf("%s:index(%d),addr(0x%08x),paramtype(%d),option1(0x%08x),option2(0x%08x)\n", regmap.drvname, regmap.index, regmap.address, regmap.paramtype, regmap.option1, regmap.option2);
//		printf("%s, %d, 0x%08x, %d, 0x%08x, 0x%08x\n", regmap.drvname, regmap.index, regmap.address, regmap.paramtype, regmap.option1, regmap.option2);
		createParamNMap(regmap);
		regmap.index++;
	};	

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

   	createParam(reg.drvname, reg.paramtype, &reg.index);
	regmapfile[reg.drvname] = reg;
	regmaptable[reg.index] = regmapfile[reg.drvname];

	printf("drvname(%s), ptype(%d), index(%d), o1(0x%08x), o2(0x%08x), drvLink(%s)\n",reg.drvname, reg.paramtype, reg.index, reg.option1, reg.option2, reg.drvLink);
	// printf("%s, 0x%08x, %d, %d, 0x%08x, 0x%08x, %s\n",reg.drvname, reg.address, reg.paramtype, reg.index, reg.option1, reg.option2, reg.drvLink);
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
	unsigned int rdData = 0;
	int rdData2 = 0;

	unsigned int IP_Index;
	unsigned int D_Addr;
	unsigned int D1_Type;
	unsigned int D2_Type;
	unsigned int D3_Type;
	unsigned int D2_Offset;

	int l_fd = -1;

	IP_Index = (regmap.address >>28) & 0x0000000F;
	D_Addr   = (regmap.address & 0x0FFFFFFF);

	D1_Type   = (regmap.option1 >>28) & 0x0000000F;
	D2_Type   = (regmap.option1 >>24) & 0x0000000F;
	D2_Offset = (regmap.option1 >>16) & 0x000000FF;
	D3_Type   = (regmap.option1 >>12) & 0x0000000F;

    switch(IP_Index)
	{
		case 0  : l_fd = -1;
			break;
		case 1  : l_fd = ptiming->ev.fd;
			break;
		case 2  : l_fd = ptiming->gtp.fd;
			break;
		default : return 0;
			break;
	};

	if(l_fd != -1) 
	{
		ptiming->ip_rd(l_fd, D_Addr, (unsigned int*)&rdData);
		
		switch(D2_Type){
			case 1  : break;
			case 2  : rdData = (rdData >> D2_Offset);	rdData &= 0xFFFF;	break;
			case 3  : rdData = (rdData >> D2_Offset);	rdData &= 0xFF;		break;
			case 4  : rdData = (rdData >> D2_Offset);	rdData &= 0xF;		break;
			case 5  : rdData = (rdData >> D2_Offset);	rdData &= 0x3;		break;
			case 6  : rdData = (rdData >> D2_Offset);	rdData &= 0x1;		break;
			default : return 0; break;
		}

		if(D3_Type == 6)
		{
			RegMap slvRegmap;

			for(int i = 0; i<regmap.option2; i++)
			{
				slvRegmap = regmaptable[regmap.index+i+1];

				D1_Type   = (slvRegmap.option1 >>28) & 0x0000000F;
				D2_Type   = (slvRegmap.option1 >>24) & 0x0000000F;
				D2_Offset = (slvRegmap.option1 >>16) & 0x000000FF;
				D3_Type   = (slvRegmap.option1 >>12) & 0x0000000F;

				switch(D2_Type){
					case 1  : rdData2 = rdData;		break;
					case 2  : rdData2 = (rdData >> D2_Offset);	rdData2 &= 0xFFFF;	break;
					case 3  : rdData2 = (rdData >> D2_Offset);	rdData2 &= 0xFF;	break;
					case 4  : rdData2 = (rdData >> D2_Offset);	rdData2 &= 0xF;		break;
					case 5  : rdData2 = (rdData >> D2_Offset);	rdData2 &= 0x3;		break;
					case 6  : rdData2 = (rdData >> D2_Offset);	rdData2 &= 0x1;		break;
					default : break;
				};

				setParamValue(slvRegmap.drvname,rdData2);
				// callParamCallbacks();
				// printf("%s-%d\r\n",slvRegmap.drvname, rdData2);
			};
		};

	};

	epicsInt32 value = rdData;
	return (value);	
}

asynStatus timingAsynEpics::readValue(const RegMap &regmap, epicsInt32 &value)
{
	//Register address read.
	//....
	epicsInt32	rdValue = 0;
    asynStatus status = asynSuccess;
	const char* functionName = regmap.drvname;

	unsigned int rdData;
	unsigned int IP_Index;
	unsigned int D_Addr;
	unsigned int D1_Type;
	unsigned int D2_Type;
	unsigned int D3_Type;
	unsigned int isWaveform;
	unsigned int D2_Offset;

	int l_fd = -1;

	IP_Index = (regmap.address >>28) & 0x0000000F;
	D_Addr   = (regmap.address & 0x0FFFFFFF);

	D1_Type		 = (regmap.option1 >>28) & 0x0000000F;
	D2_Type		 = (regmap.option1 >>24) & 0x0000000F;
	D2_Offset	 = (regmap.option1 >>16) & 0x000000FF;
	D3_Type		 = (regmap.option1 >>12) & 0x0000000F;
	isWaveform   = (regmap.option1) & 0x00000001;

    switch(IP_Index)
	{
		case 0  : l_fd = -1;
			break;
		case 1  : l_fd = ptiming->ev.fd;
			break;
		case 2  : l_fd = ptiming->gtp.fd;
			break;
		default : return (status);
			break;
	};

	if(l_fd != -1) 
	{
		ptiming->ip_rd(l_fd, D_Addr, (unsigned int*)&rdData);
		
		switch(D2_Type){
			case 1  : break;
			case 2  : rdData = (rdData >> D2_Offset);	rdData &= 0xFFFF;	break;
			case 3  : rdData = (rdData >> D2_Offset);	rdData &= 0xFF;		break;
			case 4  : rdData = (rdData >> D2_Offset);	rdData &= 0xF;		break;
			case 5  : rdData = (rdData >> D2_Offset);	rdData &= 0x3;		break;
			case 6  : rdData = (rdData >> D2_Offset);	rdData &= 0x1;		break;
			default : return (status);
		};
	}
	else if (isWaveform)
	{
		int wavecount = value;
		vecCode uval = vecCodeMap[regmap.drvLink];

		if(timingDebug)
			printf("-%s-,Sub_drvname(%s), Size(%ld)\n", functionName, regmap.drvLink, uval.size());

		if(uval.size() == 0 || wavecount > 2047) return (status);

		//for(size_t i = 0; i < uval.size();i++)
		unsigned long eval = 0;
		for(int i = 0; i < wavecount;i++)
		{
			eval = uval.at(i);
			if(timingDebug && i < timingPrintCount)
				printf("%s(%s)-[%ld]:(0x%03lx),Value(%ld)\n",functionName,regmap.drvLink, i, eval, eval);

			ptiming->IP_WR(D_Addr+(i*4), eval);
		};
	};

	value = rdValue;
	return (status);	
}

asynStatus timingAsynEpics::readValue(const RegMap &regmap, epicsFloat64 &value)
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

asynStatus timingAsynEpics::readInt32Array(asynUser *pasynUser, epicsInt32 *value, size_t nElements, size_t *nIn)
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
			if(timingDebug && i < (size_t)timingPrintCount)
				printf("%s(%s)-[%ld]:LONG(0x%03lx),Value(%d)\n",functionName,regmap.drvname, i, uval.at(i), value[i]);
		};
		vecCodeMap[regmap.drvname] = uval;
	};
    return status;
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
	
#if 0
	*value = readInt32Value(regmaptable[function]);
#else
	//New
	readValue(regmaptable[function], *value);
#endif

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
	printf("[writeInt32] %s : 0x%08x\r\n",paramName,value);


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


//===================laykim stringin
asynStatus timingAsynEpics::readStringValue(const RegMap &regmap, char *value)
{
    asynStatus status = asynSuccess;

	char strbuf[150];

	unsigned int rdData1=0;
	unsigned int rdData2=0;

	unsigned int year ;
	unsigned int month;
	unsigned int day  ;
	unsigned int hour ;
	unsigned int min  ;
	unsigned int sec  ;
	unsigned int ms   ;
	unsigned int tick ;
	double ns =	0.0f  ;

	unsigned int IP_Index;
	unsigned int D_Addr;
	unsigned int D1_Type;
	unsigned int D2_Type;
	unsigned int D3_Type;
	unsigned int D2_Offset;

	int l_fd = -1;

	IP_Index = (regmap.address >>28) & 0x0000000F;
	D_Addr   = (regmap.address & 0x0FFFFFFF);

	D1_Type   = (regmap.option1 >>28) & 0x0000000F; // scan or not check
	D2_Type   = (regmap.option1 >>24) & 0x0000000F; // data type 32bit, 16bit, 8bit, 4bit, 2bit, 1bit 
	D2_Offset = (regmap.option1 >>16) & 0x000000FF; // offset of data type
	D3_Type   = (regmap.option1 >>12) & 0x0000000F; // normal, pulse, getset, timeType0, timeType1 value

	switch(IP_Index){
		case 1  : l_fd = ptiming->ev.fd;
			break;
		case 2  : l_fd = ptiming->gtp.fd;
			break;
		case 0  : l_fd = -1;
		default : return asynError;
	}

	if(D1_Type != 1) 
	{
		if(l_fd != -1) 
		{
			if(D3_Type == 4)
			{
				writeInt32Value(regmapfile[regmap.drvLink]);
				ptiming->ip_rd(l_fd, regmap.option2*4, (unsigned int*)&rdData2);
			}

			ptiming->ip_rd(l_fd, D_Addr, (unsigned int*)&rdData1);

			switch(D2_Type){
				case 1  : break;
				case 2  : rdData1 = (rdData1 >> D2_Offset);	rdData1 &= 0xFFFF;	break;
				case 3  : rdData1 = (rdData1 >> D2_Offset);	rdData1 &= 0xFF;		break;
				case 4  : rdData1 = (rdData1 >> D2_Offset);	rdData1 &= 0xF;		break;
				case 5  : rdData1 = (rdData1 >> D2_Offset);	rdData1 &= 0x3;		break;
				case 6  : rdData1 = (rdData1 >> D2_Offset);	rdData1 &= 0x1;		break;
				default : return asynError; break;
			}

			if(D3_Type == 3)
			{
				year  = ((rdData1 & 0x007E0000) >> 17); // 17
				day   = ((rdData1 & 0xFF800000) >> 27); // 27 bit
				hour  = ((rdData1 & 0x0001F000) >> 12); // 12
				min   = ((rdData1 & 0x00000FC0) >>  6); // 6
				sec   = ((rdData1 & 0x0000003F) >>  0); // 0

				sprintf(strbuf,"20%02d-%03d %02d:%02d:%02d", year, day, hour, min, sec );
			}
			else if(D3_Type == 4)
			{
				year  = ((rdData1 & 0x007E0000) >> 17); // 17
				day   = ((rdData1 & 0xFF800000) >> 27); // 27 bit
				hour  = ((rdData1 & 0x0001F000) >> 12); // 12
				min   = ((rdData1 & 0x00000FC0) >>  6); // 6
				sec   = ((rdData1 & 0x0000003F) >>  0); // 0

				year  = year + ((rdData2 & 0x80000000) >> 25);
				ms    = ((rdData2 & 0x3FF00000) >> 20); // 0
				tick  = ((rdData2 & 0x000FFFFF) >>  0); // 0
				ns    = ((double)ms)*1000000.0f + ((double)tick)*12.3f;

				sprintf(strbuf,"20%02d-%03d %02d:%02d:%02d [%09.1lf]ns", year, day, hour, min, sec, ns );
			}
			else if(D3_Type == 5)
			{
				year  = ((rdData1 & 0x007E0000) >> 17); // 17
				month = ((rdData1 & 0x07800000) >> 23); // 23
				day   = ((rdData1 & 0xF8000000) >> 27); // 27 bit
				hour  = ((rdData1 & 0x0001F000) >> 12); // 12
				min   = ((rdData1 & 0x00000FC0) >>  6); // 6
				sec   = ((rdData1 & 0x0000003F) >>  0); // 0

				sprintf(strbuf,"20%02d-%02d-%02d %02d:%02d:%02d", year, month, day, hour, min, sec);
			}

		}
	}
	else
	{
		if (strcmp(regmap.drvname, "FIRMWARE") == 0 )
		{
			sprintf(strbuf,"Ver %f"	,	ptiming->firmware );
		}
		else if (strcmp(regmap.drvname, "SOFTWARE") == 0 )
		{
			sprintf(strbuf,"Ver %f"	,	ptiming->software );
		}
		else if (strcmp(regmap.drvname, "SYS_EVNAME") == 0 )
		{
			sprintf(strbuf,"%s%d"	,	ptiming->evName,	ptiming->evNum );
		}
		else if (strcmp(regmap.drvname, "IOCSTARTTIME") == 0 )
		{
			epicsTimeStamp current;
			epicsTimeGetCurrent (&current);
			epicsTimeToStrftime(strbuf,25,"%Y/%m/%d %H:%M:%S", &current);
		}
	}

	strcpy(value, (char*)strbuf);


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

	*nActual = (size_t)writeOctetValue(regmaptable[function]);

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
	status = strlen(svalue);

	// printf("[writeOctetValue] %s:%s[%d]\r\n",regmap.drvname, svalue, status);

	char *pch;
	int i;
	char dType[64] = "";
	int data[4];

	memset(dType,0,sizeof(dType));
	memset(data,0,sizeof(data));

	if(status == 0) return 0;

	if(!(pch = strtok (svalue,","))) return(-1);
	strcpy(dType, pch);

	for(i=0;i<4;i++)
	{
		if(!(pch = strtok (NULL,","))) break;
		data[i] = strtol(pch,NULL,16);
	}

	printf("[writeOctetValue] %s:(%d)%s,%d,%d,%d,%d\r\n",regmap.drvname, i, dType, data[0], data[1], data[2], data[3]);

	//ptiming->ts2ip_wr(evfile, regmap.address, value );
    return  (status);
}

int timingAsynEpics::writeFloat64Value(const RegMap &regmap)
{
    int status = 0;
	epicsFloat64 value = 0.0;
	if(checkParam(regmap.drvname) == -1) return(-1);

	getDoubleParam(regmap.index, &value);
	//ptiming->ts2ip_wr(evfile, regmap.address, value );
	ptiming->IP_WR(regmap.address, value);
    return  (status);
}

//===================laykim longout
int timingAsynEpics::writeInt32Value(const RegMap &regmap)
{
	int status = 0;
	int value = 0;
	int rdData = 0;
	int wrData = 0;
	int tmpData = 0;

	if(checkParam(regmap.drvname) == -1) return(-1);

	// printf("writeInt32Value: %s, 0x%08x, 0x%08x, 0x%08x, %s\r\n",regmap.drvname, regmap.address, regmap.option1, regmap.option2, regmap.drvLink);

	unsigned int IP_Index;
	unsigned int D_Addr;
	unsigned int D1_Type;
	unsigned int D2_Type;
	unsigned int D3_Type;
	unsigned int D2_Offset;

	int l_fd = -1;

	IP_Index = (regmap.address >>28) & 0x0000000F;
	D_Addr   = (regmap.address & 0x0FFFFFFF);

	D1_Type   = (regmap.option1 >>28) & 0x0000000F;
	D2_Type   = (regmap.option1 >>24) & 0x0000000F;
	D2_Offset = (regmap.option1 >>16) & 0x000000FF;
	D3_Type   = (regmap.option1 >>12) & 0x0000000F;

	switch(IP_Index){
		case 0  : l_fd = -1;
			break;
		case 1  : l_fd = ptiming->ev.fd;
			break;
		case 2  : l_fd = ptiming->gtp.fd;
			break;
		default : return 0;
			break;
	}

	if(D3_Type == 1)
		value = 1;
	else
		getParamValue(regmap.drvname, value);

	// printf("writeInt32Value: %s, 0x%08x\r\n",regmap.drvname, value);
	// printf("writeInt32Value: %s, 0x%08x, %d, 0x%08x, %d, %d, %d, %d\r\n",regmap.drvname, value, l_fd, D_Addr, D1_Type, D2_Type, D2_Offset, D3_Type);

	if(l_fd != -1) 
	{
		ptiming->ip_rd(l_fd, D_Addr, (unsigned int*)&rdData);

		switch(D2_Type){
			case 1  : break;
			case 2  : value &= 0xFFFF;	value = (value << D2_Offset);	tmpData = ~(0xFFFF << D2_Offset);	break;
			case 3  : value &= 0xFF;	value = (value << D2_Offset);	tmpData = ~(0xFF   << D2_Offset);	break;
			case 4  : value &= 0xF;		value = (value << D2_Offset);	tmpData = ~(0xF    << D2_Offset);	break;
			case 5  : value &= 0x3;		value = (value << D2_Offset);	tmpData = ~(0x3    << D2_Offset);	break;
			case 6  : value &= 0x1;		value = (value << D2_Offset);	tmpData = ~(0x1    << D2_Offset);	break;
			default : return 0; break;
		}
		wrData = rdData & tmpData;
		wrData |= value;

		// printf("writeInt32Value");
		switch(D3_Type){
			case 0  : 
				ptiming->ip_wr(l_fd, D_Addr, (unsigned int)wrData);
				// printf("0: %s-value(%08x),rdData(%08x),tmpData(%08x),wrData(%08x)",regmap.drvname, value, rdData, tmpData, wrData);
				break;
			case 1  : 
				ptiming->ip_wr(l_fd, D_Addr, (unsigned int)value);
				ptiming->ip_wr(l_fd, D_Addr, 0);
				// printf("1: %s-value(%08x)",regmap.drvname, value);
				break;
			case 2  : l_fd = ptiming->gtp.fd;
				ptiming->ip_wr(l_fd, D_Addr, (unsigned int)(value | regmap.option2));
				// printf("2: %s-value(%08x)",regmap.drvname, value | regmap.option2);
				writeInt32Value(regmapfile[regmap.drvLink]);
				break;
			default : return 0;
				break;
		}
		// printf("\r\n");
	}
	else
	{
		if (strcmp(regmap.drvname, "SYS_BIST") == 0 )
		{
			ptiming->bist(value);
		}
	}


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
void timingAsynEpics::userProcess()
{
	//struct timespec vartime;
	//long time_elapsed_nanos;

	int fan_0;
	int run = 1;

	lock();

	/* Loop forever 
	 * If the logic will be able to block into scan thread in IOC, this thread should work in IOC.
	 */    
	while (1) { 
		//getParamValue("RUN", run);
		//getParamValue("EV_FAN_0", fan_0);
		fan_0 = readInt32Value(regmapfile["EV_FAN_0"]);

		printf("FanOut:%d\n", fan_0);
		// setParamValue("EV_FAN_0", fan_0);
		/* Asynchronous Logic */
		callParamCallbacks();
		unlock();
		epicsEventWaitWithTimeout(eventId_, 5);
		//else		epicsEventWait(eventId_);
		lock();

		//getIntegerParam(P_Run, &run);

		if(!run)		continue;
	};
}

#if 0
//Multi Return Test
tuple < float, int, int, int > timingAsynEpics::cal(int n1, int n2)
{
	return make_tuple((float)n1/n2, n1%n2, n1+n2, n1*n2);
}

void timingAsynEpics::caltest()
{
	float re1;
	int re2, re3, re4;

	tie(re1, re2, re3, re4) = cal(5, 10);

	auto result = cal(5,10);

	printf("re1(%f), re2(%d), re3(%d), re4(%d)\n", re1, re2, re3, re4);
	printf("re1(%f), re2(%d), re3(%d), re4(%d)\n", get<0>(result), get<1>(result), get<2>(result), get<3>(result)); 

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
epicsShareFunc int timingAsynEpicsConfigure(const char *portName, int maxSizeSnapshot, int maxNbSnapshot, int clientMode, const char* registerfile,const char *deviceName, const int deviceNum)
{
    new timingAsynEpics(portName, maxSizeSnapshot, maxNbSnapshot, clientMode,registerfile, deviceName, deviceNum);
    return(asynSuccess);
}


/* EPICS iocsh shell commands */
static const iocshArg initArg0 = { "portName"          ,iocshArgString};
static const iocshArg initArg1 = { "max size snapshot" ,iocshArgInt};
static const iocshArg initArg2 = { "max size buffer MB",iocshArgInt};
static const iocshArg initArg3 = { "client mode"       ,iocshArgInt};
static const iocshArg initArg4 = { "register file name",iocshArgString};
static const iocshArg initArg5 = { "device name"       ,iocshArgString};
static const iocshArg initArg6 = { "device number"     ,iocshArgInt};
static const iocshArg * const initArgs[] = {&initArg0,
                                            &initArg1,
                                            &initArg2,
                                            &initArg3,
                                            &initArg4,
                                            &initArg5,
                                            &initArg6};
static const iocshFuncDef initFuncDef = {"timingAsynEpicsConfigure",7,initArgs};
static void initCallFunc(const iocshArgBuf *args)
{
    timingAsynEpicsConfigure(args[0].sval, args[1].ival, args[2].ival, args[3].ival, args[4].sval, args[5].sval, args[6].ival);
}

void timingAsynEpicsRegister(void)
{
    iocshRegister(&initFuncDef,initCallFunc);
}

epicsExportRegistrar(timingAsynEpicsRegister);
}//end extern "C"
