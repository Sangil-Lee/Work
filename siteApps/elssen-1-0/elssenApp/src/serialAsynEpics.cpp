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
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <random>
#include <ctime>

#include "serialAsynEpics.h"

static void userProcess(void *drvPvt);

static int serialDebug = 0;
epicsExportAddress(int, serialDebug);

SerailAsynEpics::SerailAsynEpics(const char *portName, const char* filename, const char* serialPort)
	: asynPortDriver(portName, 
			1,  /*maxAddr */ 
			15000,
			asynInt32Mask | asynUInt32DigitalMask | asynFloat64Mask | asynInt16ArrayMask | asynInt32ArrayMask| asynFloat64ArrayMask | asynEnumMask | asynOctetMask |asynDrvUserMask, /* Interface mask */
			asynInt32Mask | asynUInt32DigitalMask | asynFloat64Mask | asynInt16ArrayMask | asynInt32ArrayMask| asynFloat64ArrayMask | asynEnumMask | asynOctetMask,  /* Interrupt mask */
			1, /* asynFlags.  This driver does not block and it is not multi-device, so flag is 0 */
			1, /* Autoconnect */
			0, /* Default priority */
			0 /* Default stack size*/
			),fileName(filename),driverName("SerailAsynEpics"),system_init_ok(1)
{
	printf("GCC Version-(%d)\n", GCC_VERSION);
    asynStatus status;
    const char *functionName = "SerailAsynEpics";

    // Create database entries
    eventId_ = epicsEventCreate(epicsEventEmpty);  
	mSerialPort = string(serialPort);

    // register parameter list from register file.  
	registerParamListFromFile(fileName);

	if (system_init_ok==1) 
	{
		status = (asynStatus)(epicsThreadCreate("SerialAsynEpicsUserTask",
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
SerailAsynEpics::~SerailAsynEpics()
{
}

void SerailAsynEpics::registerParamListFromFile(string filename)
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
		regmap.paramtype = getAsynParamType(pch);

		if(!(pch = strtok (NULL,"\t, "))) continue;
		strncpy(regmap.drvlink, pch, strlen(pch)-1);	// string '\0' null termination remove

		createParamNMap(regmap);
		regmap.index++;
	};	

}

asynParamType SerailAsynEpics::getAsynParamType(const char *paramstring)
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

asynStatus SerailAsynEpics::createParamNMap(RegMap &regmap)
{
    asynStatus status = asynSuccess;
   	createParam(regmap.drvname, regmap.paramtype, &regmap.index);

	regmaptable[regmap.index] = regmapfile[regmap.drvname] = regmap;

	return (status);
}

epicsInt32 SerailAsynEpics::readInt32Value(const RegMap &regmap)
{
	unsigned int rdData = 0;

	epicsInt32 value = rdData;
	return (value);	
}

asynStatus SerailAsynEpics::readValue(const RegMap &regmap, epicsInt32 &value)
{
	//Register address read.
    asynStatus status = asynSuccess;

	epicsInt32 rdData = 0;

	value = rdData;
	return (status);	
}

asynStatus SerailAsynEpics::readValue(const RegMap &regmap, epicsFloat64 &value)
{
	//Register address read.
	//....
	//epicsFloat64 rdValue = 0.0;
    asynStatus status = asynSuccess;

	float	rdValue = 0.0;
	value = rdValue;

	return (status);	
}

asynStatus SerailAsynEpics::readFloat64Array(asynUser *pasynUser, epicsFloat64 *value, size_t nElements, size_t *nIn)
{
    int function = pasynUser->reason;
    asynStatus status = asynSuccess;
    epicsTimeStamp timeStamp;
    const char *functionName = "readFlot64Array";

    getTimeStamp(&timeStamp);
    pasynUser->timestamp = timeStamp;

    RegMap regmap = regmaptable[function];

	float	rdValue[nElements];
	int count = 0;

	//regmap.pNIDAQ->ReadAnalog(rdValue, &count, nElements, 2.0);

	string strData = string(regmap.drvlink);
	if(strData.compare("NA") != 0)
	{
		float avg = accumulate(rdValue, rdValue+nElements, 0.0, plus<float>()) / nElements;
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

asynStatus SerailAsynEpics::readInt32(asynUser *pasynUser, epicsInt32 *value)
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
asynStatus SerailAsynEpics::readFloat64(asynUser *pasynUser, epicsFloat64 *value)
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
	//asynStatus SerailAsynEpics::getParamValue(const string drvname, double &value)
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

asynStatus SerailAsynEpics::writeInt32(asynUser *pasynUser, epicsInt32 value)
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

asynStatus SerailAsynEpics::writeUInt32Digital(asynUser *pasynUser, epicsUInt32 value, epicsUInt32 mask)
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


int SerailAsynEpics::checkParam(const string drvname)
{
	int checkval = 0;
	check_iter = regmapfile.find(drvname);

	if(check_iter == regmapfile.end())
		checkval = -1;

	return (checkval);
}

int SerailAsynEpics::setParamValue(const string drvname, const int ival)
{
	int check = checkParam(drvname);
	if(check == 0)
		setIntegerParam(regmapfile[drvname].index, ival);
	return (check);
}

int SerailAsynEpics::setParamValue(const string drvname, const double dval)
{
	int check = checkParam(drvname);
	if(check == 0)
		setDoubleParam(regmapfile[drvname].index, dval);
	return (check);
}

int SerailAsynEpics::setParamValue(const string drvname, const string svalue)
{
	int check = checkParam(drvname);
	if(check == 0)
		setStringParam(regmapfile[drvname].index, svalue.c_str());
	return (check);
}

asynStatus SerailAsynEpics::getParamValue(const string drvname, int maxChars, char *value )
{
    asynStatus status = asynSuccess;
	getStringParam(regmapfile[drvname].index, maxChars, value);
	return (status);
}

asynStatus SerailAsynEpics::getParamValue(const string drvname, int &value)
{
    asynStatus status = asynSuccess;
    getIntegerParam(regmapfile[drvname].index, &value);
	return (status);
}

asynStatus SerailAsynEpics::getParamValue(const string drvname, double &value)
{
    asynStatus status = asynSuccess;
	getDoubleParam(regmapfile[drvname].index, &value);
	return (status);
}


//===================laykim stringin
asynStatus SerailAsynEpics::readStringValue(const RegMap &regmap, char *value)
{
    asynStatus status = asynSuccess;

	char strbuf[150];
	strcpy(value, (char*)strbuf);

	return (status);
}

asynStatus SerailAsynEpics::readOctet(asynUser *pasynUser, char *value, size_t maxChars, size_t *nActual, int *eomReason)
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

asynStatus SerailAsynEpics::writeOctet(asynUser *pasynUser, const char *value, size_t maxChars, size_t *nActual)
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

asynStatus SerailAsynEpics::writeOctetValue(const RegMap &regmap, const char *value)
{
	asynStatus status = asynSuccess;
	if(checkParam(regmap.drvname) == -1) return(asynError);

    return  (status);
}

asynStatus SerailAsynEpics::writeStringValue(const RegMap &regmap, const char *strValue)
{
	asynStatus status = asynSuccess;
	if(checkParam(regmap.drvname) == -1) return(asynError);

    return  (status);
}


int SerailAsynEpics::writeFloat64Value(const RegMap &regmap)
{
    int status = 0;
	epicsFloat64 value = 0.0;
	if(checkParam(regmap.drvname) == -1) return(-1);

	getDoubleParam(regmap.index, &value);
    return  (status);
}

int SerailAsynEpics::writeInt32Value(const RegMap &regmap, epicsInt32 value)
{
	if(checkParam(regmap.drvname) == -1) return(-1);

	int status = 0;

    return  (status);
}

//===================laykim longout
int SerailAsynEpics::writeInt32Value(const RegMap &regmap)
{
	int status = 0;

	if(checkParam(regmap.drvname) == -1) return(-1);

															//0x1 -> wave setup, 0x3-> last index + 0x8000000, Sequence Stop Bit(SEQA, SEQB).

    return  (status);
}

asynStatus SerailAsynEpics::writeFloat64(asynUser *pasynUser, epicsFloat64 value)
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


void SerailAsynEpics::UserProcess()
{
	//struct timespec vartime;
	//long time_elapsed_nanos;
	SerialPort serialport;

	try {
		serialport.Open(mSerialPort);
	}
	catch(const OpenFailed & fail )
	{
		std::cerr << "The serial port did not open " << std::endl;
	};

#if 0
	RegMap regmap;
	int			index= 0;
	for(auto iter : regmapfile)
	{
		index++;
	};
#endif

    // Set the baud rate of the serial port.
    serialport.SetBaudRate(BaudRate::BAUD_115200) ;
    // Set the number of data bits.
    serialport.SetCharacterSize(CharacterSize::CHAR_SIZE_8) ;
    // Turn off hardware flow control.
    serialport.SetFlowControl(FlowControl::FLOW_CONTROL_NONE) ;
    // Disable parity.
    serialport.SetParity(Parity::PARITY_NONE) ;
    // Set the number of stop bits.
    serialport.SetStopBits(StopBits::STOP_BITS_1) ;
    
    // Wait for data to be available at the serial port.
    while(!serialport.IsDataAvailable()) 
    {
        usleep(1000) ;
    };

    // Specify a timeout value (in milliseconds).
    size_t ms_timeout = 250 ;

    // Char variable to store data coming from the serial port.
    char data_byte ;

    // Read one byte from the serial port and print it to the terminal.
    try
    {
        // Read a single byte of data from the serial port.
        serialport.ReadByte(data_byte, ms_timeout) ;

        // Show the user what is being read from the serial port.
        std::cout << data_byte << std::flush ;
    }
    catch (const ReadTimeout&)
    {
        std::cerr << "\nThe ReadByte() call has timed out." << std::endl ;
    };

    // Wait a brief period for more data to arrive.
    usleep(1000) ;

	DataBuffer read_buffer;

	while(true)
	{
		try {
			// Read as many bytes as are available during the timeout period.
			serialport.Read(read_buffer, 0, ms_timeout) ;
		} catch (const ReadTimeout&) {
			//std::string s1 = std::to_string(read_buffer.at(3)) + std::to_string(read_buffer.at(4));
			std::stringstream sstream;
			for (size_t i = 0 ; i < read_buffer.size() ; i++) {
				sstream << std::setfill ('0') << std::setw(sizeof(uint8_t)*2) << std::hex << static_cast<int> (read_buffer.at(i)) << std::flush;
			};

			std::string strstring = sstream.str();
			if(strstring.empty() == true || strstring.size() != 28|| strstring.find("eace") == std::string::npos) continue;

			if(serialDebug >= 2)
				cout << strstring << flush;
			
			//serial index: 8,9,10,11
			int midnum = stoi(strstring.substr(9,1), 0, 16) + stoi(strstring.substr(10,1), 0, 16);
			stringstream strmid;
			strmid << std::hex << midnum << flush;
			string snum = strstring.substr(8,1) + strmid.str() + strstring.substr(11,1);
			int nserial = stoi(snum, 0, 16);

			float temp_a = 0.0f, temp_b = 0.0f, temp_c = 0.0f, temp_d = 0.0f;
			temp_a = stoi(strstring.substr(12,4), 0, 16)/100.0;
			temp_b = stoi(strstring.substr(16,4), 0, 16)/100.0;
			temp_c = stoi(strstring.substr(20,4), 0, 16)/100.0;
			temp_d = stoi(strstring.substr(24,4), 0, 16)/100.0;

			//random number
			srand(time(nullptr));
			std::random_device rd;
			std::mt19937 gen(rd());
			std::uniform_real_distribution<> dis(0.01, 0.02);
			float fRvar = dis(gen);

			std::time_t datetime = std::time(nullptr);

			//printf("Random num: %f\n", fRvar);
			if(serialDebug >= 2)
				printf("\n Serial: %d, TempA: %f, TempB: %f, TempC: %f, Tempd: %f, Time:%s\n", nserial, temp_a+fRvar, temp_b+fRvar, temp_c, temp_d, 
									std::asctime(std::localtime(&datetime)));

			string strserial = to_string(nserial);
			RegMap regmap;
			for(auto iter = regmapfile.begin(); iter != regmapfile.end(); ++iter) {
				regmap = iter->second;
				string sdrvName(regmap.drvname);
				if(sdrvName.find(strserial) != string::npos)
				{
					string pvnameA = strserial+string("_TEMP_A");
					string pvnameB = strserial+string("_TEMP_B");
					if((setParamValue(pvnameA, temp_a+fRvar)) == 0) {
						callParamCallbacks();
					};
					if((setParamValue(pvnameB, temp_b+fRvar)) == 0) {
						callParamCallbacks();
					};
				};
			};

		};//end while
	};
}

void SerailAsynEpics::getOptionBitset(OptionBit &optionbit, epicsUInt32 option, epicsUInt32 shift, epicsUInt32 mask)
{
	optionbit = (mask == 0) ? (option >> shift) : (option >> shift) & mask;
}

epicsUInt32 SerailAsynEpics::getRShiftMask(epicsUInt32 option, epicsUInt32 shift, epicsUInt32 mask = 0)
{
	return (mask == 0) ? (option >> shift) : (option >> shift) & mask;
}

epicsUInt32 SerailAsynEpics::getLShiftMask(epicsUInt32 option, epicsUInt32 shift, epicsUInt32 mask)
{
	return (mask == 0) ? (option << shift) : (mask << shift) | option;
}

epicsUInt32 SerailAsynEpics::setBitOnOff(epicsUInt32 option, epicsUInt32 nbit, bool bit)
{
	epicsUInt32 retVal = (bit==1) ? option |= 1 << nbit : option &= ~(1 << nbit);
	return (retVal);
}

epicsUInt32 SerailAsynEpics::setExclusiveBit(epicsUInt32 option, epicsUInt32 nbit)
{
	return (option ^= 1 << nbit);
}

bool SerailAsynEpics::checkBit(epicsUInt32 option, epicsUInt32 nbit)
{
	return ((option >> nbit)&1);
}

void userProcess(void *drvPvt)
{
	SerailAsynEpics *pPvt = (SerailAsynEpics *)drvPvt;
	if(pPvt!=NULL) pPvt->UserProcess();
}

extern "C" {

/** EPICS iocsh callable function to call constructor for the testAsynPortDriver class.
  * \param[in] portName The name of the asyn port driver to be created.
  * \param[in] maxSizeSnapshot The maximum  number of sample in one snapshot
  * \param[in] maxNbSnapshot The number of snapshot buffered
 */
epicsShareFunc int SerialAsynEpicsConfigure(const char *portName, const char* registerfile, const char *serialPort)
{
    new SerailAsynEpics(portName, registerfile, serialPort);
    return(asynSuccess);
}

/* EPICS iocsh shell commands */
static const iocshArg initArg0 = { "portName"          ,	iocshArgString};
static const iocshArg initArg1 = { "register file name",	iocshArgString};
static const iocshArg initArg2 = { "serial port name"  ,	iocshArgString};
static const iocshArg * const initArgs[] = {&initArg0,
                                            &initArg1,
                                            &initArg2};

static const iocshFuncDef initFuncDef = {"SerialAsynEpicsConfigure",3,initArgs};
static void initCallFunc(const iocshArgBuf *args)
{
    SerialAsynEpicsConfigure(args[0].sval, args[1].sval, args[2].sval);
}

void serialAsynEpicsRegister(void)
{
    iocshRegister(&initFuncDef,initCallFunc);
}

epicsExportRegistrar(serialAsynEpicsRegister);
}//end extern "C"
