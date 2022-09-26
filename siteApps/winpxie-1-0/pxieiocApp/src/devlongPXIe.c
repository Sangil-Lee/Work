
#include "longinRecord.h"
#include "longoutRecord.h"
#include "devPXIe.h"
#include <registryFunction.h>
#include <epicsExport.h>
#include <dbCommon.h>
#include <iocsh.h>
#include <NIDAQmx.h>
#include <string.h>


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
    initRecord,
    NULL,
    readPXI6514
};

epicsExportAddress(dset,devPXI6514Read);


long initRecord(struct longinRecord *pRec)
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
    printf("Value:%u\n", pRec->val);

	status = DAQmxReadDigitalU32(g_taskHandle,1, 10.0, DAQmx_Val_GroupByChannel, &data, 1, &read, NULL);
	printf("Data acquired: 0x%X\n",(unsigned)data);
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
    initRecordW,
    NULL,
    writePXI6514
};

long initRecordW(struct longoutRecord *pRec)
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

	uInt32  data = pRec->val;
	int32	written;

	/*********************************************/
	// DAQmx Write Code
	/*********************************************/
	status = DAQmxWriteDigitalU32(g_taskHandle,1,1,10.0,DAQmx_Val_GroupByChannel,&data,&written,NULL);

    if(!status) pRec->udf = FALSE;
    return(0);
}

epicsExportAddress(dset,devPXI6514Write);



/** EPICS iocsh callable function to call constructor for the testAsynPortDriver class.
  * \param[in] portName The name of the asyn port driver to be created.
  * \param[in] maxSizeSnapshot The maximum  number of sample in one snapshot
  * \param[in] maxNbSnapshot The number of snapshot buffered
 */
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
	DAQmxStartTask(g_taskHandle);
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

static const iocshArg chArg0 = { "chportName", iocshArgString};
static const iocshArg * const chArgs[] = {&chArg0};
static const iocshFuncDef niDICreateFuncDef = {"nidaqDICreateChannel", 1, chArgs};

static void niDICreateFunc(const iocshArgBuf *args)
{
	nidaqDICreateChannel(args[0].sval);
}

void nidaqAsynEpicsRegister(void)
{
    iocshRegister(&initFuncDef,initCallFunc);
    iocshRegister(&niDICreateFuncDef,niDICreateFunc);
}

epicsExportAddress(int, nidaqDebug);
epicsExportRegistrar(nidaqAsynEpicsRegister);
