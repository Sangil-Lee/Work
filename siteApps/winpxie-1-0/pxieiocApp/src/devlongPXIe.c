
#include "longinRecord.h"
#include "longoutRecord.h"
#include "devPXIe.h"
#include <registryFunction.h>
#include <epicsExport.h>
#include <dbCommon.h>
#include <iocsh.h>
#include <NIDAQmx.h>


static int nidaqDebug = 1;

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
    long status;

    status = dbGetLink(&(pRec->inp),DBF_LONG, &(pRec->val),0,0);
    /*If return was succesful then set undefined false*/
    printf("Value:%u\n", pRec->val);
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
    if(!status) pRec->udf = FALSE;
    return(0);
}

epicsExportAddress(dset,devPXI6514Write);



/** EPICS iocsh callable function to call constructor for the testAsynPortDriver class.
  * \param[in] portName The name of the asyn port driver to be created.
  * \param[in] maxSizeSnapshot The maximum  number of sample in one snapshot
  * \param[in] maxNbSnapshot The number of snapshot buffered
 */
epicsShareFunc int nidaqAsynEpicsConfigure(const char *portName, const char* registerfile, const char *deviceName)
{
    TaskHandle  g_taskHandle;
    int status = DAQmxCreateTask("", &g_taskHandle);

    char devNames[60];
	int32 sts= DAQmxGetSysDevNames(devNames, sizeof(devNames));
	printf("Device Names: %s\n", devNames);

}


/* EPICS iocsh shell commands */
static const iocshArg initArg0 = { "portName"          ,iocshArgString};
static const iocshArg initArg1 = { "register file name",iocshArgString};
static const iocshArg initArg2 = { "device name"       ,iocshArgString};
static const iocshArg * const initArgs[] = {&initArg0,
                                            &initArg1,
                                            &initArg2};

static const iocshFuncDef initFuncDef = {"nidaqAsynEpicsConfigure",3,initArgs};
static void initCallFunc(const iocshArgBuf *args)
{
    nidaqAsynEpicsConfigure(args[0].sval, args[1].sval, args[2].sval);
}

void nidaqAsynEpicsRegister(void)
{
    iocshRegister(&initFuncDef,initCallFunc);
}

epicsExportAddress(int, nidaqDebug);
epicsExportRegistrar(nidaqAsynEpicsRegister);
