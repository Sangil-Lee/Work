#include <cstdlib>
#include <cstring>
//#include <sstream>
#include <fstream>
#include <bitset>
#include <numeric>
#include <valarray>
#include <unistd.h>
#include <memory>

#include "ADIMITechDriver.h"
#include "debugPrint.h"


static int ADIMITechDebug = 2;
epicsExportAddress(int, ADIMITechDebug);

static void userImageGrabTask(void *drvPvt);
//static void callBackImageGrab(NImageFrame *pData);

namespace DataFrame
{
	class DataBuffer
	{
		public:
			DataBuffer():mpFrameBuf(0),mframesize(0)
				{};

			~DataBuffer()
			{
				if(mpFrameBuf)
					delete [] mpFrameBuf;

			};

			int SetFrameSize(unsigned int framesize)
			{
				if(mpFrameBuf) return(-1);

				mpFrameBuf = new unsigned char[framesize];
				mframesize = framesize;

				return(0);
			};

			int FrameCopy(NImageFrame *pImage, size_t imgsize)
			{
				memcpy(mpFrameBuf, pImage, imgsize);
			};

			unsigned int GetFrameSize() {return mframesize;};
			unsigned char* GetFrameBuffer() {return mpFrameBuf;};

		private:
			unsigned char*	mpFrameBuf;
			unsigned int mframesize;
			NImageFrame *mpImage;

			int mWidth;
			int mHeight;

	};
};

shared_ptr<DataFrame::DataBuffer> mpDataFrame;


ADIMITechDriver::ADIMITechDriver(const char *portName, const char*filename, const char *cameraId, int maxBuffers, size_t maxMemory,
                     int priority, int stackSize, int maxPvAPIFrames)
    : ADDriver(portName, 1, 500, maxBuffers, maxMemory, 0, 0, /* No interfaces beyond those set in ADDriver.cpp */
               ASYN_CANBLOCK, 0, /* ASYN_CANBLOCK=1, ASYN_MULTIDEVICE=0, autoConnect=1 */ priority, stackSize),mCamStrID(0),fileName(filename)
{

	mCamStrID = epicsStrDup(cameraId);

	mpDataFrame = make_shared<DataFrame::DataBuffer>();

	registerParamListFromFile(fileName);

	if(ADIMITechDebug) {
		int camNum = 0;
		NeptuneNumberOfCameras(&camNum);

		for(int i = 0; i < camNum; i++)
		{
			char camStrID[64] = {0};
			NeptuneGetDisplayName(i, camStrID);
			debug(DEBUG_INFO,"CamStrID List:(%s)\n", camStrID);
		};

	};

	asynStatus status = (asynStatus)(epicsThreadCreate("IMITechUserImageGrabTask",
					epicsThreadPriorityMedium, epicsThreadGetStackSize(epicsThreadStackMedium),
					(EPICSTHREADFUNC)::userImageGrabTask, this) == NULL);

	if (status) {
		debug(DEBUG_INFO,"%s:%s: epicsThreadCreate failure\n", "ADIMITechDriver", "UserImageGrabTask");
		return;
	};
}

ADIMITechDriver::~ADIMITechDriver()
{
}

void ADIMITechDriver::UserImageGrabTask()
{
	NErr status = eNeptuneSuccess;
	NCamHandle  _camHandle;

	status = NeptuneOpenCamera(mCamStrID,  &_camHandle);

    if(status != eNeptuneSuccess) {
		debug(DEBUG_INFO, "Error OpenCamera Error:%s \n", mCamStrID);
        return;
    };

    // get size info
    NSizeInfo sinfo;
    NeptuneGetSizeInfo(_camHandle, &sinfo);
    int m_iWidth = sinfo.width;
    int m_iHeight = sinfo.height;

    // get pixelformat list
    int npixels = 0;
    NeptuneGetNumAvailablePixelFormats(_camHandle, &npixels);

	char strTmp[128] = {0};

#if 1
    for(int i = 0 ; i < npixels ; i++) {
        NeptuneGetAvailablePixelFormatsIdxToStr(_camHandle, i, strTmp);
    };

    NeptuneGetPixelFormatStr(_camHandle, strTmp);
	//setParam

	//gain
	int value = 0;
	bool bAuto = false;

	NeptuneGetGain(_camHandle, &value, &bAuto);
	//setParam

	//shutter
	unsigned int shutterVal = 0;
	NeptuneGetShutter(_camHandle, &shutterVal, &bAuto);

	//setParam

	//trigger
	NeptuneGetTriggerEnable(_camHandle, &bAuto);
	//setParam

	NeptuneGetTriggerSourceStr(_camHandle, strTmp);
	//setParam


	NeptuneGetTriggerParameter(_camHandle, &value);

	//getParam

	NeptuneSetBayerConversion(_camHandle, eNeptuneBayerNearest);
	NeptuneSetBayerConversion(_camHandle, eNeptuneBayerNone);

	while(true)
	{
		NeptuneStartAcquisition(_camHandle, ADIMITechDriver::callBackImageGrab);
	};
#endif


	if(_camHandle != NULL) NeptuneCloseCamera(_camHandle);
	_camHandle = NULL;

}

void userImageGrabTask(void *drvPvt)
{
	ADIMITechDriver *pDrv = (ADIMITechDriver *)drvPvt;
	pDrv->UserImageGrabTask();
}

void ADIMITechDriver::callBackImageGrab(NImageFrame *pData)
{


}


void ADIMITechDriver::registerParamListFromFile(string &filename)
{
	ifstream sfile (filename.c_str());

	if(sfile.fail()) return;
	
	RegMap regmap;
	string strToken;
	char str[200];
	char *pch;

	regmap.index = 0;

	while(!sfile.eof())
	{
		getline(sfile, strToken);
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

asynParamType ADIMITechDriver::getAsynParamType(const char *paramstring)
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

asynStatus ADIMITechDriver::createParamNMap(RegMap &regmap)
{
    asynStatus status = asynSuccess;
   	createParam(regmap.drvname, regmap.paramtype, &regmap.index);

	regmaptable[regmap.index] = regmapfile[regmap.drvname] = regmap;

#if 0
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

	if(regmap.index == 0 && ADIMITechDebug)
	{
		printf("%s\n", "==========================================================================================");
		printf("%4s, %-34s(%s), %10s, %10s, %-10s\n", "index", "drvname","options", "paramtype", "slotchannel", "drvlink");
		printf("%s\n", "------------------------------------------------------------------------------------------");
	}

	if(ADIMITechDebug)
		printf("%5d, %-34s(%s), %10d, %10s(len:%u), %-10s(len:%d)\n", regmap.index, regmap.drvname, strOption.c_str(),regmap.paramtype,regmap.slotchannel, 
				(int)strlen(regmap.slotchannel),regmap.drvlink,(int)strlen(regmap.drvlink));
#endif

	return (status);
}

extern "C" int ImiTechConfig(const char *portName, /* Port name */
							 const char *filename, /*Register file name*/
                             const char *cameraId, /*IMC-7050G-28029679(Model&Serial), Camera name of IMITech 7050G model  . */
                               int maxBuffers, size_t maxMemory, int priority, int stackSize, int maxPvAPIFrames)
{
    new ADIMITechDriver(portName, filename, cameraId, maxBuffers, maxMemory, priority, stackSize, maxPvAPIFrames);
    return(asynSuccess);
}   

/* Code for iocsh registration */
static const iocshArg ImiTechConfigArg0 = {"Port name", iocshArgString};
static const iocshArg ImiTechConfigArg1 = {"Regiter file name", iocshArgString};
static const iocshArg ImiTechConfigArg2 = {"Camera Id (unique ID, IP address, or IP name", iocshArgString};
static const iocshArg ImiTechConfigArg3 = {"maxBuffers", iocshArgInt};
static const iocshArg ImiTechConfigArg4 = {"maxMemory", iocshArgInt};
static const iocshArg ImiTechConfigArg5 = {"priority", iocshArgInt};
static const iocshArg ImiTechConfigArg6 = {"stackSize", iocshArgInt};
static const iocshArg ImiTechConfigArg7 = {"maxPvAPIFrames", iocshArgInt};
static const iocshArg * const ImiTechConfigArgs[] = {&ImiTechConfigArg0,
                                                       &ImiTechConfigArg1,
                                                       &ImiTechConfigArg2,
                                                       &ImiTechConfigArg3,
                                                       &ImiTechConfigArg4,
                                                       &ImiTechConfigArg5,
                                                       &ImiTechConfigArg6,
                                                       &ImiTechConfigArg7};

static const iocshFuncDef configImiTech = {"ImiTechConfig", 8, ImiTechConfigArgs};
static void configImiTechCallFunc(const iocshArgBuf *args)
{
    ImiTechConfig(args[0].sval, args[1].sval, args[2].sval, args[3].ival, 
                    args[4].ival, args[5].ival, args[6].ival, args[7].ival);
}


static void ImiTechRegister(void)
{
    iocshRegister(&configImiTech, configImiTechCallFunc);
}

extern "C" {
epicsExportRegistrar(ImiTechRegister);
}
