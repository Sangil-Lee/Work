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
#include "mbbiRecord.h"
#include "epicsExport.h"

/* Create the dset for devMbbiSoft */
static long init_record(mbbiRecord *prec);
static long read_mbbi(mbbiRecord *prec);

struct {
    long      number;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN read_mbbi;
} devMbbiSoft = {
    5,
    NULL,
    NULL,
    init_record,
    NULL,
    read_mbbi
};
epicsExportAddress(dset, devMbbiSoft);

static long init_record(mbbiRecord *prec)
{
    if (recGblInitConstantLink(&prec->inp, DBF_ENUM, &prec->val))
        prec->udf = FALSE;

    return 0;
}

static long read_mbbi(mbbiRecord *prec)
{
    if (!dbGetLink(&prec->inp, DBR_USHORT, &prec->val, 0, 0)) {
        prec->udf = FALSE;

        if (dbLinkIsConstant(&prec->tsel) &&
            prec->tse == epicsTimeEventDeviceTime)
            dbGetTimeStamp(&prec->inp, &prec->time);
    }
    return 2;
}
