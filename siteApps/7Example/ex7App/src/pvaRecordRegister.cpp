/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2013.07.24
 */


/* Author: Marty Kraimer */

#include <iocsh.h>
#include <pv/pvDatabase.h>
#include <pv/pvStructureCopy.h>
#include <pv/channelProviderLocal.h>

// The following must be the last include for code database uses
#include <epicsExport.h>
#define epicsExportSharedSymbols
#include "pvaRecord.h"

using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;
using namespace epics::pvarecord::database;

static const iocshFuncDef PVARecordFuncDef = {"PVARecord", 0};

static void PVARecordCallFunc(const iocshArgBuf *args)
{
    PVARecord::create();
}

static void PVARecordRegister(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&PVARecordFuncDef, PVARecordCallFunc);
    }
}

extern "C" {
    epicsExportRegistrar(PVARecordRegister);
}
