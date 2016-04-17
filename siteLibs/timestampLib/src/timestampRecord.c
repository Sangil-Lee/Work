#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dbDefs.h"
#include "errlog.h"
#include "alarm.h"
#include "cvtTable.h"
#include "dbAccess.h"
#include "dbScan.h"
#include "dbEvent.h"
#include "dbFldTypes.h"
#include "devSup.h"
#include "recSup.h"
#include "recGbl.h"
#include "menuConvert.h"

#define GEN_SIZE_OFFSET
#include	"timestampRecord.h"
#undef  GEN_SIZE_OFFSET

#include "epicsExport.h"

/* create RSET - record support entry table */
#define report NULL
#define initialize NULL
static long init_record();
static long process();
static void monitor();
#define special NULL
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

rset timestampRSET={
	RSETNUMBER,
	report,
	initialize,
	init_record,
	process,
	special,
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
	get_alarm_double };

epicsExportAddress(rset, timestampRSET);



typedef struct {   /* timestamp dset */
    long         number;
    DEVSUPFUN    dev_report;
    DEVSUPFUN    init;
    DEVSUPFUN    init_record;  /* returns: (-1,0) => (failure, sucess) */
    DEVSUPFUN    get_ioint_info;
    DEVSUPFUN    read_timestamp;
} timestampdset;

    

typedef struct {
    int year;
    int month;
    int day;
    int hour;
    int min;
    int sec;
    double sec_nsec;
} timeStr;

static char month[12][4]=
 {"JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};


static long init_record(struct timestampRecord *ptimestamp, int pass) 
{
    timestampdset *pdset = (timestampdset*)ptimestamp->dset;

    if(!pass) return 0;
    if(!pdset) return 0;

    if(pdset->number <5 || !pdset->read_timestamp) {
        recGblRecordError(S_dev_missingSup, (void*)ptimestamp, "timestamp: init_record");
        return S_dev_missingSup;
    }

    if(pdset->init_record) {
        long status = (*pdset->init_record)(ptimestamp);
        return status;
    }


    return 0;
}

static long process(struct timestampRecord *ptimestamp)
{
    timestampdset *pdset = (timestampdset*)ptimestamp->dset;
    char buf[40];
    timeStr vtime;
    epicsTimeStamp *etime = &ptimestamp->time;
    long status = 0;

    ptimestamp->udf  = FALSE;
    ptimestamp->pact = TRUE;

    if (!pdset) recGblGetTimeStamp(ptimestamp);
    else {
        if(!pdset->read_timestamp) {
            ptimestamp->pact = TRUE;
            recGblRecordError(S_dev_missingSup, (void*)ptimestamp, "read_timestamp");
            return S_dev_missingSup;
        }
        else {
            status = (*pdset->read_timestamp)(ptimestamp);
        }
    }

    ptimestamp->rval = (double) etime->secPastEpoch + 1.E-9 * (double) etime->nsec;

    epicsTimeToStrftime(buf,40,"%Y/%m/%d %H:%M:%S.%09f", etime);
    sscanf(buf, "%d/%d/%d %d:%d:%lf", &vtime.year, &vtime.month, &vtime.day, &vtime.hour, &vtime.min, &vtime.sec_nsec);
    vtime.sec = (int) vtime.sec_nsec;
      
    switch(ptimestamp->tst) {
        case timestampTST_YY_MM_DD_HH_MM_SS:    /* YY/MM/DD HH:MM:SS */
            sprintf(ptimestamp->val, "%.2d/%.2d/%.2d %.2d:%.2d:%.2d",
                    vtime.year, vtime.month, vtime.day, vtime.hour, vtime.min, vtime.sec);
            break;
        case timestampTST_MM_DD_YY_HH_MM_SS:    /* MM/DD/YY HH:MM:SS */
            sprintf(ptimestamp->val, "%.2d/%.2d/%.2d %.2d:%.2d:%.2d",
                    vtime.month, vtime.day, vtime.year, vtime.hour, vtime.min, vtime.sec);
            break;
        break;
        case timestampTST_MM_DD_HH_MM_SS_YY:    /* Mon DD HH:MM:SS YY */
            sprintf(ptimestamp->val, "%s %.2d %.2d:%.2d:%.2d %.2d",
                    month[vtime.month-1], vtime.day, vtime.hour, vtime.min, vtime.sec, vtime.year);
            break;
        break;
        case timestampTST_MM_DD_HH_MM_SS:       /* Mon DD HH:MM:SS */
            sprintf(ptimestamp->val, "%s %.2d %.2d:%.2d:%.2d",
                    month[vtime.month-1], vtime.day, vtime.hour, vtime.min, vtime.sec);
            break;
        case timestampTST_HH_MM_SS:             /* HH:MM:SS */
            sprintf(ptimestamp->val, "%.2d:%.2d:%.2d",
                    vtime.hour, vtime.min, vtime.sec);
            break;
        case timestampTST_HH_MM:                /* HH:MM */
            sprintf(ptimestamp->val, "%.2d:%.2d",
                    vtime.hour, vtime.min);
            break;
        case timestampTST_DD_MM_YY_HH_MM_SS:    /* DD/MM/YY HH:MM:SS */
            sprintf(ptimestamp->val, "%.2d/%.2d/%.2d %.2d:%.2d:%.2d",
                    vtime.day, vtime.month, vtime.year, vtime.hour, vtime.min, vtime.sec);
            break;
        case timestampTST_DD_MM_HH_MM_SS_YY:    /* DD Mon HH:MM:SS YY */
            sprintf(ptimestamp->val, "%.2d %s %.2d:%.2d:%.2d %.2d",
                    vtime.day, month[vtime.month-1], vtime.hour, vtime.min, vtime.sec, vtime.year);
            break;
        case timestampTST_VMS:                  /* DD-Mon-YYYY HH:MM:SS */
            sprintf(ptimestamp->val, "%.2d-%s-%.4d %.2d:%.2d:%.2d",
                    vtime.day, month[vtime.month-1], vtime.year, vtime.hour, vtime.min, vtime.sec);
            break;
        case timestampTST_MM_DD_YYYY:           /* Mon DD, YYYY HH:MM:SS.ns */
            sprintf(ptimestamp->val, "%s %.2d, %.4d %.2d:%.2d:%.2d.%.9d",
                    month[vtime.month-1], vtime.day, vtime.year, vtime.hour, vtime.min, vtime.sec,
                    etime->nsec);
            break;
        case timestampTST_MM_DD_YY:            /* MM/DD/YY HH:MM:SS.ns */
            sprintf(ptimestamp->val, "%.2d/%.2d/%.2d %.2d:%.2d:%.2d.%.9d",
                    vtime.month, vtime.day, vtime.year, vtime.hour, vtime.min, vtime.sec, etime->nsec);
            break;
        case timestampTST_YYYY_MM_DD_HH_MM_SS:
        default :  /* YYYY/MM/DD HH:MM:SS.ns */
            strcpy(ptimestamp->val, buf);
            break;
    }
    /* check event list */
    monitor(ptimestamp);

    /* process the forward scan link record */
    recGblFwdLink(ptimestamp);
    ptimestamp->pact=FALSE;
    return status;
}


static void monitor(struct timestampRecord *ptimestamp)
{
    unsigned short  monitor_mask;
  
    monitor_mask = recGblResetAlarms(ptimestamp);
    monitor_mask |=DBE_VALUE|DBE_LOG;
    if(strncmp(ptimestamp->oval,ptimestamp->val,sizeof(ptimestamp->val))) {
        db_post_events(ptimestamp,&(ptimestamp->val[0]), monitor_mask);
        db_post_events(ptimestamp,&ptimestamp->rval,monitor_mask);
        strncpy(ptimestamp->oval,ptimestamp->val,sizeof(ptimestamp->val));
    }
    return;
}
