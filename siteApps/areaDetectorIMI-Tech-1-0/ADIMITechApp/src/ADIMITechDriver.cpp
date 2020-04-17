#include <cstdlib>
#include <cstring>
//#include <sstream>
#include <fstream>
#include <bitset>
#include <numeric>
#include <valarray>
#include <unistd.h>
#include <memory>

#include "debugPrint.h"
#include "ADIMITechDriver.h"


static int ADIMITechDebug = 2;
epicsExportAddress(int, ADIMITechDebug);

static void userImageGrabTask(void *drvPvt);


class NeptuneFrame {

public:
	NeptuneFrame(const char* camID);
	~NeptuneFrame();

	NErr OpenCamera();
	NErr StartImageGrab();
	NErr StopImageGrab();

	NErr GetFrameSize(int &width, int &height);

	static void cbGetImageFrame(NImageFrame *pData);
private:

	char*  mCamID;
	NCamHandle  _camHandle;

	void cameraList();

};


NeptuneFrame::NeptuneFrame(const char* camID):mCamID(0),_camHandle(0)
{
	mCamID = epicsStrDup(camID);
	cameraList();
};

NeptuneFrame::~NeptuneFrame()
{
	if(_camHandle)
	{
		NeptuneCloseCamera(_camHandle);
		_camHandle = 0;
	};

};

NErr NeptuneFrame::OpenCamera()
{
	return (NeptuneOpenCamera(mCamID,  &_camHandle));
}

NErr NeptuneFrame::GetFrameSize(int &width, int &height)
{
	NSizeInfo sInfo;
	NErr status = NeptuneGetSizeInfo(_camHandle, &sInfo);

	width = sInfo.width;
	height = sInfo.height;

	return status;
}

void NeptuneFrame::cbGetImageFrame(NImageFrame *pData)
{
	//bla..bla..

}

void NeptuneFrame::cameraList()
{
	int nNum = 0;
    NeptuneNumberOfCameras(&nNum);
    for(int i = 0 ; i < nNum ; i++)
    {
        char strID[64] = {0,};
        NeptuneGetDisplayName(i, strID);
		epicsPrintf("Camera List(%d): (%s) \n",i, strID);
    };
};


NErr NeptuneFrame::StartImageGrab()
{
	NErr status = NeptuneStartAcquisition(_camHandle, cbGetImageFrame);
	debug(DEBUG_INFO, "Error status:%d\n, status");
	return(status);
}

NErr NeptuneFrame::StopImageGrab()
{
	NErr status = NeptuneStopAcquisition(_camHandle);
	debug(DEBUG_INFO, "Error status:%d\n, status");
	return(status);
}

shared_ptr<NeptuneFrame> mpShrNeptune;


ADIMITechDriver::ADIMITechDriver(const char *portName, const char *cameraId, int maxBuffers, size_t maxMemory,
                     int priority, int stackSize, int maxPvAPIFrames)
    : ADDriver(portName, 1, 500, maxBuffers, maxMemory, 0, 0, /* No interfaces beyond those set in ADDriver.cpp */
               ASYN_CANBLOCK, 0, /* ASYN_CANBLOCK=1, ASYN_MULTIDEVICE=0, autoConnect=1 */ priority, stackSize),mCamID(0)
{

	mCamID = epicsStrDup(cameraId);

	asynStatus status = (asynStatus)(epicsThreadCreate("IMITechUserImageGrabTask",
					epicsThreadPriorityMedium, epicsThreadGetStackSize(epicsThreadStackMedium),
					(EPICSTHREADFUNC)::userImageGrabTask, this) == NULL);

	mpShrNeptune = make_shared<NeptuneFrame>(cameraId);

	if (status) {
		//printf("%s:%s: epicsThreadCreate failure\n", "ADIMITechDriver", "UserImageGrabTask");
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

	//status = NeptuneOpenCamera(mCamID,  &_camHandle);

	status = mpShrNeptune->OpenCamera();


    if(status != eNeptuneSuccess) {
        debug(DEBUG_INFO,"Error OpenCamera Error : %d\n", status);
        return;
    };

#if 0
    // get size info
    NSizeInfo sinfo;
    NeptuneGetSizeInfo(_camHandle, &sinfo);
    int m_iWidth = sinfo.width;
    int m_iHeight = sinfo.height;
#else
	int width = 0, height = 0;
	status = mpShrNeptune->GetFrameSize(width, height);
#endif


    // get pixelformat list
    int npixels = 0;
    NeptuneGetNumAvailablePixelFormats(_camHandle, &npixels);

#if 0
    for(int i = 0 ; i < npixels ; i++)
    {
        NeptuneGetAvailablePixelFormatsIdxToStr(m_cam, i, strTmp);
        ui->cmbPixelFormat->addItem(strTmp);
    }

    NeptuneGetPixelFormatStr(m_cam, strTmp);
    for(int i = 0 ; i < ui->cmbPixelFormat->count() ; i++)
    {
        str = ui->cmbPixelFormat->itemText(i);
        if(strcmp(str.toLatin1().data(), strTmp) == 0) {
            ui->cmbPixelFormat->setCurrentIndex(i);
            break;
        }
    };
#else

#endif


	while(true)
	{

	};


	if(_camHandle != NULL) NeptuneCloseCamera(_camHandle);
	_camHandle = NULL;

}

void userImageGrabTask(void *drvPvt)
{
	ADIMITechDriver *pDrv = (ADIMITechDriver *)drvPvt;

	pDrv->UserImageGrabTask();
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

extern "C" int ImiTechConfig(char *portName, /* Port name */
                               const char *cameraId,   /* Unique ID #, or IP address or IP name of this camera. */
                               int maxBuffers, size_t maxMemory,
                               int priority, int stackSize, int maxPvAPIFrames)
{
    new ADIMITechDriver(portName, cameraId, maxBuffers, maxMemory, priority, stackSize, maxPvAPIFrames);
    return(asynSuccess);
}   

/* Code for iocsh registration */
static const iocshArg ImiTechConfigArg0 = {"Port name", iocshArgString};
static const iocshArg ImiTechConfigArg1 = {"Camera Id (unique ID, IP address, or IP name", iocshArgString};
static const iocshArg ImiTechConfigArg2 = {"maxBuffers", iocshArgInt};
static const iocshArg ImiTechConfigArg3 = {"maxMemory", iocshArgInt};
static const iocshArg ImiTechConfigArg4 = {"priority", iocshArgInt};
static const iocshArg ImiTechConfigArg5 = {"stackSize", iocshArgInt};
static const iocshArg ImiTechConfigArg6 = {"maxPvAPIFrames", iocshArgInt};
static const iocshArg * const ImiTechConfigArgs[] = {&ImiTechConfigArg0,
                                                       &ImiTechConfigArg1,
                                                       &ImiTechConfigArg2,
                                                       &ImiTechConfigArg3,
                                                       &ImiTechConfigArg4,
                                                       &ImiTechConfigArg5,
                                                       &ImiTechConfigArg6};
static const iocshFuncDef configImiTech = {"ImiTechConfig", 7, ImiTechConfigArgs};
static void configImiTechCallFunc(const iocshArgBuf *args)
{
    ImiTechConfig(args[0].sval, args[1].sval, args[2].ival, 
                    args[3].ival, args[4].ival, args[5].ival, args[6].ival);
}


static void ImiTechRegister(void)
{
    iocshRegister(&configImiTech, configImiTechCallFunc);
}

extern "C" {
epicsExportRegistrar(ImiTechRegister);
}
