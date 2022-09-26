/***************************************************************
* Stream Device record interface for analog output records     *
*                                                              *
* (C) 1999 Dirk Zimoch (zimoch@delta.uni-dortmund.de)          *
* (C) 2005 Dirk Zimoch (dirk.zimoch@psi.ch)                    *
*                                                              *
* This is an EPICS record Interface for StreamDevice.          *
* Please refer to the HTML files in ../docs/ for a detailed    *
* documentation.                                               *
*                                                              *
* If you do any changes in this file, you are not allowed to   *
* redistribute it any more. If there is a bug or a missing     *
* feature, send me an email and/or your patch. If I accept     *
* your changes, they will go to the next release.              *
*                                                              *
* DISCLAIMER: If this software breaks something or harms       *
* someone, it's your problem.                                  *
*                                                              *
***************************************************************/
#include "NIDAQmx.h"

#include "aoRecord.h"
#include "devPXIe.h"

/*
#include "menuConvert.h"
#include "cvtTable.h"
#include "devStream.h"
*/

static TaskHandle gTHandle;
static long readData (dbCommon* record, void* arg)
{
    aoRecord *ao = (aoRecord *)record;
    return 0;

}

static long writeData(dbCommon *record, void* arg)
{
    aoRecord *ao = (aoRecord *)record;

    return 0;
}

static long initRecord(dbCommon *record)
{
    aoRecord *ao = (aoRecord *)record;

    DAQmxCreateTask("", &gTHandle);

    return 0;
}

struct {
    long number;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN write;
    DEVSUPFUN special_linconv;
} devaoPXIe = {
    6,
    pxieReport,
    pxieInit,
    initRecord,
    pxieGetIointInfo,
    pxieWrite,
    NULL
};

epicsExportAddress(dset,devaoPXIe);
