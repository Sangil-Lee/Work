/*************************************************************************\
* Copyright (c) 2008 UChicago Argonne LLC, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/

/* Revision-Id: anj@aps.anl.gov-20101005192737-disfz3vs0f3fiixd */

/* recStringin.c - Record Support Routines for Stringin records */
/*
 *      Author: 	Janet Anderson
 *      Date:   	4/23/91
 */


#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "dbDefs.h"
#include "epicsPrint.h"
#include "alarm.h"
#include "dbAccess.h"
#include "dbEvent.h"
#include "dbFldTypes.h"
#include "devSup.h"
#include "errMdef.h"
#include "recSup.h"
#include "recGbl.h"
#include "menuYesNo.h"
#define GEN_SIZE_OFFSET
#include "bigstringinRecord.h"
#undef  GEN_SIZE_OFFSET
#include "epicsExport.h"

/* Create RSET - Record Support Entry Table*/
#define report NULL
#define initialize NULL
static long init_record(bigstringinRecord *, int);
static long process(bigstringinRecord *);
#define special NULL
#define get_value NULL
#define cvt_dbaddr NULL
#define get_array_info NULL
#define put_array_info NULL
#define get_units NULL
#define get_precision NULL
#define get_enum_str NULL
#define get_enum_strs NULL
#define put_enum_str NULL
#define get_graphic_double NULL
#define get_control_double NULL
#define get_alarm_double NULL

rset bigstringinRSET={
	RSETNUMBER,
	report,
	initialize,
	init_record,
	process,
	special,
	get_value,
	cvt_dbaddr,
	get_array_info,
	put_array_info,
	get_units,
	get_precision,
	get_enum_str,
	get_enum_strs,
	put_enum_str,
	get_graphic_double,
	get_control_double,
	get_alarm_double
};
epicsExportAddress(rset,bigstringinRSET);



static long init_record(bigstringinRecord *prec, int pass)
{
    STATIC_ASSERT(sizeof(prec->oval)==sizeof(prec->val));

    if (pass==0) return(0);

    strcpy(prec->oval,prec->val);
    return(0);
}

static long process(bigstringinRecord *prec)
{
    unsigned short  monitor_mask;
    monitor_mask = recGblResetAlarms(prec);
    prec->pact = TRUE;
    recGblGetTimeStamp(prec);
    /* process the forward scan link record */
    recGblFwdLink(prec);
    monitor_mask |= DBE_VALUE|DBE_LOG;
    db_post_events(prec,&(prec->val[0]),monitor_mask);
    prec->pact=FALSE;
    return(0);
}
