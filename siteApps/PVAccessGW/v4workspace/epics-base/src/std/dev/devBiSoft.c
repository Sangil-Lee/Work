/*************************************************************************\
* Copyright (c) 2008 UChicago Argonne LLC, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/

/* $Revision-Id$
 *
 *      Original Authors: Bob Dalesio and Marty Kraimer
 *      Date: 6-1-90
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "alarm.h"
#include "dbDefs.h"
#include "dbAccess.h"
#include "recGbl.h"
#include "devSup.h"
#include "biRecord.h"
#include "epicsExport.h"

/* Create the dset for devBiSoft */
static long init_record(biRecord *prec);
static long read_bi(biRecord *prec);

struct {
    long      number;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN read_bi;
} devBiSoft = {
    5,
    NULL,
    NULL,
    init_record,
    NULL,
    read_bi
};
epicsExportAddress(dset, devBiSoft);

static long init_record(biRecord *prec)
{
    if (recGblInitConstantLink(&prec->inp, DBF_ENUM, &prec->val))
        prec->udf = FALSE;
    return 0;
}

static long read_bi(biRecord *prec)
{
    if (!dbGetLink(&prec->inp, DBR_USHORT, &prec->val, 0, 0)) {
        prec->udf = FALSE;
        if (dbLinkIsConstant(&prec->tsel) &&
            prec->tse == epicsTimeEventDeviceTime)
            dbGetTimeStamp(&prec->inp, &prec->time);
    }
    return 2;
}
