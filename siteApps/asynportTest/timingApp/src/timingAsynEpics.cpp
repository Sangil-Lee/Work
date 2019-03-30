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

#define FREQUENCY 1000       /* Frequency in Hz */
#define AMPLITUDE 1.0        /* Plus and minus peaks of sin wave */
#define NUM_DIVISIONS 10     /* Number of scope divisions in X and Y */
#define MIN_UPDATE_TIME 0.02 /* Minimum update time, to prevent CPU saturation */

static void userProcess(void *drvPvt);
extern struct s_ts2sys* gpSys;


TimingRegmap::TimingRegmap():firmware(0.1),software(3.14125)
{
}

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
										),fileName(filename),driverName("timingAsynEpics"),system_init_ok(0) 
{
	printf("GCC Version-(%d)\n", GCC_VERSION);
    asynStatus status;
    const char *functionName = "timingAsynEpics";

	tsmode = tsMode(deviceName);
	printf("TSMode(%d)\n", tsmode);


	ts2api_init(tsmode);
	ts2api_getInstance(gpSys);

    // Create database entries
    eventId_ = epicsEventCreate(epicsEventEmpty);  
    //pregmap = (struct TimingRegmap *)malloc(sizeof(TimingRegmap));
    //pregmap = (TimingRegmap *)calloc(1,sizeof(TimingRegmap));
    pregmap = new TimingRegmap();
	clientThreadMode = clientMode;

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
	//setDoubleParam(P_Firmware, pregmap->GetFirmware());
	//setDoubleParam(P_Software, pregmap->GetSoftware());
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
	ts2ip_rd(gpSys->ip.ev.fd, regmap.address, (unsigned int*)&rdData);
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
	RegMap regmap = regmaptable[function];

#if 0
	if (function == P_Run) {
		/* If run was set then wake up the userProcess task */
		if (value) epicsEventSignal(eventId_);
	} 
	else if (function == P_EVS_Time)
	{
		regmap = regmaptable[function];
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, C_evgW_set_h8(2) | C_evgW_set_m8(3) | C_evgW_set_s8(4) );
	}
	else if (function == P_EVS_Day)
	{
		regmap = regmaptable[function];
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address + 4, C_evgW_set_d16(360));
	}
	else if (function == P_EVS_Set)
	{
		regmap = regmaptable[function];
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address,1);
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address,0);
	}
	else if (function == P_SEQ_User_Trigger)
	{
		//ADRR = 0x078
		//printf("USER Trigger Addr(%x)\n", addr);
		int value;
		regmap = regmaptable[function];
		getIntegerParam(P_SEQ_User_Trigger, &value);
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
			"%s:%s: function=(%d), name=(%s), regmap.drvname(%s), regmap.address(%x), trigger value=(%d)\n", 
			driverName, functionName, function, paramName, regmap.drvname, regmap.address, value);
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address,C_usrTrg(1));
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address,C_usrTrg(0));
		value = 0;
		setIntegerParam(P_SEQ_User_Trigger, value);
	}
	else if (function == P_PG_Update)
	{
		int pg_update;
		getIntegerParam(P_PG_Update, &pg_update);
		regmap = regmaptable[function];
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
			"%s:%s: function=(%d), name=(%s), regmap.drvname(%s), regmap.address(%x), update value=(%d)\n", 
			driverName, functionName, function, paramName, regmap.drvname, regmap.address, pg_update);
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address,C_evr_setCtrlReg(1));
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address,C_evr_setCtrlReg(0));
	}
	else if (function == P_PG_Width_1 || function == P_PG_Width_2  || function == P_PG_Width_3  || function == P_PG_Width_4 ||
	         function == P_PG_Width_5 || function == P_PG_Width_6  || function == P_PG_Width_7  || function == P_PG_Width_8 ||
	         function == P_PG_Width_9 || function == P_PG_Width_10 || function == P_PG_Width_11 || function == P_PG_Width_12 ||
	         function == P_PG_Width_13|| function == P_PG_Width_14 || function == P_PG_Width_15 || function == P_PG_Width_16 ||
	         function == P_PG_Width_17|| function == P_PG_Width_18 || function == P_PG_Width_19 || function == P_PG_Width_20 ||
	         function == P_PG_Width_21|| function == P_PG_Width_22 || function == P_PG_Width_23 || function == P_PG_Width_24 ||
	         function == P_PG_Width_25|| function == P_PG_Width_26 || function == P_PG_Width_27 || function == P_PG_Width_28 ||
	         function == P_PG_Width_29|| function == P_PG_Width_30 || function == P_PG_Width_31 || function == P_PG_Width_32)
	{
		int width;
        getIntegerParam(function, &width);
		
		regmap = regmaptable[function];
		//printf("Regmaptable: drvname(%s), address(%x)\n",regmap.drvname, regmap.address);
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
			"%s:%s: function=(%d), name=(%s), regmap.drvname(%s), regmap.address(%x), width value=(%d)\n", 
			driverName, functionName, function, paramName, regmap.drvname, regmap.address, width);
		//0xC480
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, width);
	}
	else if (function == P_PG_Delay_1  || function == P_PG_Delay_2  || function == P_PG_Delay_3  || function == P_PG_Delay_4 ||
	         function == P_PG_Delay_5  || function == P_PG_Delay_6  || function == P_PG_Delay_7  || function == P_PG_Delay_8 ||
	         function == P_PG_Delay_9  || function == P_PG_Delay_10 || function == P_PG_Delay_11 || function == P_PG_Delay_12 ||
	         function == P_PG_Delay_13 || function == P_PG_Delay_14 || function == P_PG_Delay_15 || function == P_PG_Delay_16 ||
	         function == P_PG_Delay_17 || function == P_PG_Delay_18 || function == P_PG_Delay_19 || function == P_PG_Delay_20 ||
	         function == P_PG_Delay_21 || function == P_PG_Delay_22 || function == P_PG_Delay_23 || function == P_PG_Delay_24 ||
	         function == P_PG_Delay_25 || function == P_PG_Delay_26 || function == P_PG_Delay_27 || function == P_PG_Delay_28 ||
	         function == P_PG_Delay_29 || function == P_PG_Delay_30 || function == P_PG_Delay_31 || function == P_PG_Delay_32)

	{
		int delay;
        getIntegerParam(function, &delay);
		
		regmap = regmaptable[function];
		//printf("Delay Regmap Table: drvname(%s),address(%x), delay value(%d)\n",regmap.drvname, regmap.address, delay);
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
			"%s:%s: function=(%d), name=(%s), regmap.drvname(%s), regmap.address(%x), delay value=(%d)\n", 
			driverName, functionName, function, paramName, regmap.drvname, regmap.address, delay);
		//0xC400
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, delay);
	}
	else if (function == P_PG_POL_0)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(0,0);
		else 		polbit.set(0,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
			"%s:%s: function=(%d), name=(%s), regmap.drvname(%s), regmap.address(%x), polality value=(%d)\n", 
			driverName, functionName, function, paramName, regmap.drvname, regmap.address, pol);
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	
	}
	else if (function == P_PG_POL_1)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(1,0);
		else 		polbit.set(1,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
			"%s:%s: function=(%d), name=(%s), regmap.drvname(%s), regmap.address(%x), polality value=(%d)\n", 
			driverName, functionName, function, paramName, regmap.drvname, regmap.address, pol);
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	
	}
	else if (function == P_PG_POL_2)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(2,0);
		else 		polbit.set(2,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
			"%s:%s: function=(%d), name=(%s), regmap.drvname(%s), regmap.address(%x), polality value=(%d)\n", 
			driverName, functionName, function, paramName, regmap.drvname, regmap.address, pol);
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_3)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(3,0);
		else 		polbit.set(3,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
			"%s:%s: function=(%d), name=(%s), regmap.drvname(%s), regmap.address(%x), polality value=(%d)\n", 
			driverName, functionName, function, paramName, regmap.drvname, regmap.address, pol);
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_4)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(4,0);
		else 		polbit.set(4,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
			"%s:%s: function=(%d), name=(%s), regmap.drvname(%s), regmap.address(%x), polality value=(%d)\n", 
			driverName, functionName, function, paramName, regmap.drvname, regmap.address, pol);
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_5)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(5,0);
		else 		polbit.set(5,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
			"%s:%s: function=(%d), name=(%s), regmap.drvname(%s), regmap.address(%x), polality value=(%d)\n", 
			driverName, functionName, function, paramName, regmap.drvname, regmap.address, pol);
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_6)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(6,0);
		else 		polbit.set(6,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
			"%s:%s: function=(%d), name=(%s), regmap.drvname(%s), regmap.address(%x), polality value=(%d)\n", 
			driverName, functionName, function, paramName, regmap.drvname, regmap.address, pol);
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_7)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(7,0);
		else 		polbit.set(7,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
			"%s:%s: function=(%d), name=(%s), regmap.drvname(%s), regmap.address(%x), polality value=(%d)\n", 
			driverName, functionName, function, paramName, regmap.drvname, regmap.address, pol);
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_8)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(8,0);
		else 		polbit.set(8,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
			"%s:%s: function=(%d), name=(%s), regmap.drvname(%s), regmap.address(%x), polality value=(%d)\n", 
			driverName, functionName, function, paramName, regmap.drvname, regmap.address, pol);
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_9)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(9,0);
		else 		polbit.set(9,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
			"%s:%s: function=(%d), name=(%s), regmap.drvname(%s), regmap.address(%x), polality value=(%d)\n", 
			driverName, functionName, function, paramName, regmap.drvname, regmap.address, pol);
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_10)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(10,0);
		else 		polbit.set(10,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
			"%s:%s: function=(%d), name=(%s), regmap.drvname(%s), regmap.address(%x), polality value=(%d)\n", 
			driverName, functionName, function, paramName, regmap.drvname, regmap.address, pol);
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_11)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(11,0);
		else 		polbit.set(11,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
			"%s:%s: function=(%d), name=(%s), regmap.drvname(%s), regmap.address(%x), polality value=(%d)\n", 
			driverName, functionName, function, paramName, regmap.drvname, regmap.address, pol);
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_12)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(12,0);
		else 		polbit.set(12,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
			"%s:%s: function=(%d), name=(%s), regmap.drvname(%s), regmap.address(%x), polality value=(%d)\n", 
			driverName, functionName, function, paramName, regmap.drvname, regmap.address, pol);
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_13)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(13,0);
		else 		polbit.set(13,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
			"%s:%s: function=(%d), name=(%s), regmap.drvname(%s), regmap.address(%x), polality value=(%d)\n", 
			driverName, functionName, function, paramName, regmap.drvname, regmap.address, pol);
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_14)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(14,0);
		else 		polbit.set(14,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
			"%s:%s: function=(%d), name=(%s), regmap.drvname(%s), regmap.address(%x), polality value=(%d)\n", 
			driverName, functionName, function, paramName, regmap.drvname, regmap.address, pol);
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_15)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(15,0);
		else 		polbit.set(15,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
			"%s:%s: function=(%d), name=(%s), regmap.drvname(%s), regmap.address(%x), polality value=(%d)\n", 
			driverName, functionName, function, paramName, regmap.drvname, regmap.address, pol);
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_16)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(16,0);
		else 		polbit.set(16,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
			"%s:%s: function=(%d), name=(%s), regmap.drvname(%s), regmap.address(%x), polality value=(%d)\n", 
			driverName, functionName, function, paramName, regmap.drvname, regmap.address, pol);
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_17)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(17,0);
		else 		polbit.set(17,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
			"%s:%s: function=(%d), name=(%s), regmap.drvname(%s), regmap.address(%x), polality value=(%d)\n", 
			driverName, functionName, function, paramName, regmap.drvname, regmap.address, pol);
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_18)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(18,0);
		else 		polbit.set(18,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
			"%s:%s: function=(%d), name=(%s), regmap.drvname(%s), regmap.address(%x), polality value=(%d)\n", 
			driverName, functionName, function, paramName, regmap.drvname, regmap.address, pol);
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_19)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(19,0);
		else 		polbit.set(19,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
			"%s:%s: function=(%d), name=(%s), regmap.drvname(%s), regmap.address(%x), polality value=(%d)\n", 
			driverName, functionName, function, paramName, regmap.drvname, regmap.address, pol);
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_20)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(20,0);
		else 		polbit.set(20,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
			"%s:%s: function=(%d), name=(%s), regmap.drvname(%s), regmap.address(%x), polality value=(%d)\n", 
			driverName, functionName, function, paramName, regmap.drvname, regmap.address, pol);
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_21)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(21,0);
		else 		polbit.set(21,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
			"%s:%s: function=(%d), name=(%s), regmap.drvname(%s), regmap.address(%x), polality value=(%d)\n", 
			driverName, functionName, function, paramName, regmap.drvname, regmap.address, pol);
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_22)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(22,0);
		else 		polbit.set(22,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
			"%s:%s: function=(%d), name=(%s), regmap.drvname(%s), regmap.address(%x), polality value=(%d)\n", 
			driverName, functionName, function, paramName, regmap.drvname, regmap.address, pol);
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_23)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(23,0);
		else 		polbit.set(23,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
			"%s:%s: function=(%d), name=(%s), regmap.drvname(%s), regmap.address(%x), polality value=(%d)\n", 
			driverName, functionName, function, paramName, regmap.drvname, regmap.address, pol);
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_24)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(24,0);
		else 		polbit.set(24,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
			"%s:%s: function=(%d), name=(%s), regmap.drvname(%s), regmap.address(%x), polality value=(%d)\n", 
			driverName, functionName, function, paramName, regmap.drvname, regmap.address, pol);
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_25)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(25,0);
		else 		polbit.set(25,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
			"%s:%s: function=(%d), name=(%s), regmap.drvname(%s), regmap.address(%x), polality value=(%d)\n", 
			driverName, functionName, function, paramName, regmap.drvname, regmap.address, pol);
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_26)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(26,0);
		else 		polbit.set(26,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
			"%s:%s: function=(%d), name=(%s), regmap.drvname(%s), regmap.address(%x), polality value=(%d)\n", 
			driverName, functionName, function, paramName, regmap.drvname, regmap.address, pol);
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_27)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(27,0);
		else 		polbit.set(27,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
			"%s:%s: function=(%d), name=(%s), regmap.drvname(%s), regmap.address(%x), polality value=(%d)\n", 
			driverName, functionName, function, paramName, regmap.drvname, regmap.address, pol);
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_28)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(28,0);
		else 		polbit.set(28,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
			"%s:%s: function=(%d), name=(%s), regmap.drvname(%s), regmap.address(%x), polality value=(%d)\n", 
			driverName, functionName, function, paramName, regmap.drvname, regmap.address, pol);
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_29)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(29,0);
		else 		polbit.set(29,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
			"%s:%s: function=(%d), name=(%s), regmap.drvname(%s), regmap.address(%x), polality value=(%d)\n", 
			driverName, functionName, function, paramName, regmap.drvname, regmap.address, pol);
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_30)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(30,0);
		else 		polbit.set(30,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
			"%s:%s: function=(%d), name=(%s), regmap.drvname(%s), regmap.address(%x), polality value=(%d)\n", 
			driverName, functionName, function, paramName, regmap.drvname, regmap.address, pol);
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_31)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(31,0);
		else 		polbit.set(31,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
			"%s:%s: function=(%d), name=(%s), regmap.drvname(%s), regmap.address(%x), polality value=(%d)\n", 
			driverName, functionName, function, paramName, regmap.drvname, regmap.address, pol);
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
#endif


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

int timingAsynEpics::setParamValue(const string drvname, const int ival)
{
	setIntegerParam(regmapfile[drvname].index, ival);
	return (0);
}
int timingAsynEpics::setParamValue(const string drvname, const double dval)
{
	setDoubleParam(regmapfile[drvname].index, dval);
	return (0);
}
int timingAsynEpics::setParamValue(const string drvname, const string svalue)
{
	setStringParam(regmapfile[drvname].index, svalue.c_str());
	return (0);
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
		ts2ip_rd(gpSys->ip.ev.fd, regmap.address, (unsigned int*)&rdData);
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

	RegMap regmap = regmaptable[function];

#if 0
    if (function == P_Starttime) {
			/* Make sure the update time is valid. If not change it and put back in parameter library */
			char svalue[maxChars] = "";
			getStringParam(P_Starttime, maxChars, svalue);
			//printf("writeOctet*****:%s(%ld)\n", svalue, maxChars);
		}
#endif

    status = (asynStatus) callParamCallbacks();
		return (status);
}

asynStatus timingAsynEpics::writeFloat64Value(const RegMap &regmap)
{
    asynStatus status = asynSuccess;
	epicsFloat64 value = 0.0;
	getDoubleParam(regmap.index, &value);
	ts2ip_wr(gpSys->ip.ev.fd, regmap.address, value );
    return  (status);
}

asynStatus timingAsynEpics::writeFloat64(asynUser *pasynUser, epicsFloat64 value)
{
    int function = pasynUser->reason;
    asynStatus status = asynSuccess;
    int run;
    int addr=0;
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
    double updateTime;
    //struct timespec vartime;
    //long time_elapsed_nanos;
		int run;

		lock();

    /* Loop forever 
		 * If the logic will be able to block into scan thread in IOC, this thread should work in IOC.
		*/    

    while (1) { 
			//getIntegerParam(P_Run, &run);
			//getDoubleParam(P_UpdateTime, &updateTime);
			//printf("Run:%d, Updatetime:%f\n",run, updateTime);

			/* Asynchronous Logic */
			
			unlock();
			if(run)	epicsEventWaitWithTimeout(eventId_, updateTime);
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
