#include <string.h>

#include "registryFunction.h"
#include "epicsExport.h"
#include "dbCommon.h"
#include "iocsh.h"
#include "NIDAQmx.h"

#include "longinRecord.h"
#include "longoutRecord.h"
#include "aoRecord.h"
#include "devPXIe.h"

static int nidaqDebug = 1;
static TaskHandle  g_taskHandle;

static char gDevName[24];

struct {
    long number;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN read;
} devPXI6514Read = {
    5,
    NULL,
    NULL,
    initRecordDI,
    NULL,
    readPXI6514
};

epicsExportAddress(dset,devPXI6514Read);


long initRecordDI(struct longinRecord *pRec)
{
    if(recGblInitConstantLink(&pRec->inp,DBF_LONG,&pRec->val))
         pRec->udf = FALSE;
    return(0);
}

long readPXI6514(struct longinRecord *pRec)
{
    long 	status;
    uInt32	data;
    int32	read;

    status = dbGetLink(&(pRec->inp),DBF_LONG, &(pRec->val),0,0);
    /*If return was succesful then set undefined false*/

    status = DAQmxReadDigitalU32(g_taskHandle,1, 10.0, DAQmx_Val_GroupByChannel, &data, 1, &read, NULL);

    pRec->val = data;

    if(!status) pRec->udf = FALSE;
    return(0);
}

struct {
    long number;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN write;
} devPXI6514Write = {
    5,
    NULL,
    NULL,
    initRecordDO,
    NULL,
    writePXI6514
};

long initRecordDO(struct longoutRecord *pRec)
{
    if(recGblInitConstantLink(&pRec->out,DBF_LONG,&pRec->val))
         pRec->udf = FALSE;
    return(0);
}


long writePXI6514(struct longoutRecord *pRec)
{
    long status;

    status = dbGetLink(&(pRec->out),DBF_LONG, &(pRec->val),0,0);
    /*If return was succesful then set undefined false*/

    uInt32  data = (uInt32)pRec->val;
    int32	written;

    /*********************************************/
    // DAQmx Write Code
    /*********************************************/
    status = DAQmxWriteDigitalU32(g_taskHandle,1,1,10.0,DAQmx_Val_GroupByChannel,&data,&written,NULL);

    if(!status) pRec->udf = FALSE;
    return(0);
}

epicsExportAddress(dset,devPXI6514Write);

struct {
    long number;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN write;
} devPXI4322Write = {
    5,
    NULL,
    NULL,
    initRecordPXIAO,
    NULL,
    writePXIAO
};

long initRecordPXIAO(struct aoRecord *pRec)
{
    if(recGblInitConstantLink(&pRec->out,DBF_DOUBLE,&pRec->val))
		pRec->udf = TRUE;
    return(1);
}


long writePXIAO(struct aoRecord *pRec)
{
    long status;

    status = dbGetLink(&(pRec->out),DBF_DOUBLE, &(pRec->val),0,0);
    /*If return was succesful then set undefined false*/

    /*********************************************/
    // DAQmx Write Code
    /*********************************************/
    float64  data = pRec->val;
    status = DAQmxWriteAnalogF64(g_taskHandle,1,1,10.0,DAQmx_Val_GroupByChannel,&data,NULL,NULL);

    if(!status) pRec->udf = FALSE;
    return(0);
}

epicsExportAddress(dset,devPXI4322Write);


/** EPICS iocsh callable function to call constructor for the testAsynPortDriver class.
  * \param[in] portName The name of the asyn port driver to be created.
  * \param[in] maxSizeSnapshot The maximum  number of sample in one snapshot
  * \param[in] maxNbSnapshot The number of snapshot buffered
 */
//============================>
epicsShareFunc int nidaqDIConfigure(const char *portName, const char *deviceName)
{
    int status = DAQmxCreateTask("", &g_taskHandle);

    char devNames[60];
	status = DAQmxGetSysDevNames(devNames, sizeof(devNames));
	printf("Device Names: %s\n", devNames);
	strcpy(gDevName, deviceName);
	return status;
}

epicsShareFunc int nidaqDICreateChannel(const char *channelName)
{
	char devChName[40];	
	sprintf(devChName, "%s/%s", gDevName, channelName); 

	int status = DAQmxCreateDIChan(g_taskHandle, devChName, "", DAQmx_Val_ChanForAllLines);
	return(status);
}

epicsShareFunc int nidaqDOCreateChannel(const char *channelName)
{
	char devChName[40];	
	sprintf(devChName, "%s/%s", gDevName, channelName); 

	int status = DAQmxCreateDOChan(g_taskHandle, devChName, "", DAQmx_Val_ChanForAllLines);
	return(status);
}

epicsShareFunc int nidaqAOCreateChannel(const char *channelName)
{
	char devChName[40];	
	sprintf(devChName, "%s/%s", gDevName, channelName); 

    	int status = DAQmxCreateAOVoltageChan(g_taskHandle,devChName,"",-10.0,10.0,DAQmx_Val_Volts,"");
	return(status);
}

/* EPICS iocsh shell commands */
static const iocshArg initArg0 = { "portName"          ,iocshArgString};
static const iocshArg initArg1 = { "device name"       ,iocshArgString};

static const iocshArg * const initArgs[] = {&initArg0, &initArg1};

static const iocshFuncDef initFuncDef = {"nidaqDIConfigure",2,initArgs};

static void initCallFunc(const iocshArgBuf *args)
{
    nidaqDIConfigure(args[0].sval, args[1].sval);
}

static const iocshArg diArg0 = { "chportName", iocshArgString};
static const iocshArg * const diArgs[] = {&diArg0};
static const iocshFuncDef niDICreateFuncDef = {"nidaqDICreateChannel", 1, diArgs};

static void niDICreateFunc(const iocshArgBuf *args)
{
	nidaqDICreateChannel(args[0].sval);
}

static const iocshArg doArg0 = { "chportName", iocshArgString};
static const iocshArg * const doArgs[] = {&doArg0};
static const iocshFuncDef niDOCreateFuncDef = {"nidaqDOCreateChannel", 1, doArgs};

static void niDOCreateFunc(const iocshArgBuf *args)
{
	nidaqDOCreateChannel(args[0].sval);
}

static const iocshArg aoArg0 = { "chportName", iocshArgString};
static const iocshArg * const aoArgs[] = {&aoArg0};
static const iocshFuncDef niAOCreateFuncDef = {"nidaqAOCreateChannel", 1, aoArgs};

static void niAOCreateFunc(const iocshArgBuf *args)
{
	nidaqAOCreateChannel(args[0].sval);
}

static const iocshFuncDef niTaskStartFuncDef = {"nidaqTaskStart", 0};

static void niTaskStartFunc()
{
	DAQmxStartTask(g_taskHandle);
}

void nidaqAsynEpicsRegister(void)
{
    iocshRegister(&initFuncDef,initCallFunc);
    iocshRegister(&niDICreateFuncDef,niDICreateFunc);
    iocshRegister(&niDOCreateFuncDef,niDOCreateFunc);
    iocshRegister(&niAOCreateFuncDef,niAOCreateFunc);
    iocshRegister(&niTaskStartFuncDef, niTaskStartFunc);
}

epicsExportAddress(int, nidaqDebug);
epicsExportRegistrar(nidaqAsynEpicsRegister);
