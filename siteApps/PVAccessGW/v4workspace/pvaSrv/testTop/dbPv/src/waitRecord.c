/* waitRecord.c */
/* Record waits while processing */
  
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "alarm.h"
#include "dbAccess.h"
#include "recGbl.h"
#include "dbEvent.h"
#include "dbDefs.h"
#include "dbAccess.h"
#include "devSup.h"
#include "errMdef.h"
#include "recSup.h"
#include "special.h"
#include "callback.h"
#define GEN_SIZE_OFFSET
#include "waitRecord.h"
#undef  GEN_SIZE_OFFSET
#include "epicsExport.h"

/* Create RSET - Record Support Entry Table */
#define report NULL
#define initialize NULL
static long init_record();
static long process();
#define special NULL
#define get_value NULL
#define cvt_dbaddr NULL
#define get_array_info NULL
#define put_array_info NULL
static long get_units();
static long get_precision();
#define get_enum_str NULL
#define get_enum_strs NULL
#define put_enum_str NULL
static long get_graphic_double();
static long get_control_double();
static long get_alarm_double();
 
rset waitRSET={
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
epicsExportAddress(rset,waitRSET);

static void checkAlarms(waitRecord *prec);
static void monitor(waitRecord *prec);

static long init_record(void *precord, int pass)
{
    waitRecord *prec = (waitRecord *)precord;
    if (!prec->dpvt) prec->dpvt = calloc(1, sizeof(CALLBACK));
    return(0);
}

static long process(void *precord)
{
    waitRecord *prec = (waitRecord *)precord;

    if (!prec->pact) {
        prec->pact = TRUE;
        callbackRequestProcessCallbackDelayed((CALLBACK *)prec->dpvt,
                    prec->prio, prec, (double)prec->val);
        return 0;
    }

    recGblGetTimeStamp(prec);
	/* check for alarms */
    checkAlarms(prec);
	/* check event list */
    monitor(prec);
	/* process the forward scan link record */
    recGblFwdLink(prec);

    prec->pact=FALSE;
	return(0);
}

static long get_units(DBADDR *paddr, char *units)
{
    waitRecord *prec=(waitRecord *)paddr->precord;

    strncpy(units,prec->egu,DB_UNITS_SIZE);
    return(0);
}

static long get_precision(DBADDR *paddr, long *precision)
{
    waitRecord *prec=(waitRecord *)paddr->precord;

    *precision = prec->prec;
    if(paddr->pfield == (void *)&prec->val) return(0);
    recGblGetPrec(paddr,precision);
    return(0);
}

static long get_graphic_double(DBADDR *paddr,struct dbr_grDouble *pgd)
{
    waitRecord *prec=(waitRecord *)paddr->precord;
    int fieldIndex = dbGetFieldIndex(paddr);

    if(fieldIndex == waitRecordVAL
    || fieldIndex == waitRecordHIHI
    || fieldIndex == waitRecordHIGH
    || fieldIndex == waitRecordLOW
    || fieldIndex == waitRecordLOLO
    || fieldIndex == waitRecordHOPR
    || fieldIndex == waitRecordLOPR) {
        pgd->upper_disp_limit = prec->hopr;
        pgd->lower_disp_limit = prec->lopr;
    } else recGblGetGraphicDouble(paddr,pgd);
    return(0);
}

static long get_control_double(DBADDR *paddr,struct dbr_ctrlDouble *pcd)
{
    waitRecord *prec=(waitRecord *)paddr->precord;
    int fieldIndex = dbGetFieldIndex(paddr);

    if(fieldIndex == waitRecordVAL
    || fieldIndex == waitRecordHIHI
    || fieldIndex == waitRecordHIGH
    || fieldIndex == waitRecordLOW
    || fieldIndex == waitRecordLOLO) {
    pcd->upper_ctrl_limit = prec->hopr;
    pcd->lower_ctrl_limit = prec->lopr;
    } else recGblGetControlDouble(paddr,pcd);
    return(0);
}

static long get_alarm_double(DBADDR *paddr,struct dbr_alDouble *pad)
{
    waitRecord *prec=(waitRecord *)paddr->precord;
    int	 fieldIndex = dbGetFieldIndex(paddr);

    if(fieldIndex == waitRecordVAL) {
    pad->upper_alarm_limit = prec->hihi;
    pad->upper_warning_limit = prec->high;
    pad->lower_warning_limit = prec->low;
    pad->lower_alarm_limit = prec->lolo;
    } else recGblGetAlarmDouble(paddr,pad);
    return(0);
}

static void checkAlarms(waitRecord *prec)
{
    double val;
    float hyst, lalm, hihi, high, low, lolo;
    unsigned short hhsv, llsv, hsv, lsv;

    if(prec->udf == TRUE ){
        recGblSetSevr(prec,UDF_ALARM,INVALID_ALARM);
		return;
	}
    hihi = prec->hihi; lolo = prec->lolo; high = prec->high; low = prec->low;
    hhsv = prec->hhsv; llsv = prec->llsv; hsv = prec->hsv; lsv = prec->lsv;
    val = prec->val; hyst = prec->hyst; lalm = prec->lalm;

	/* alarm condition hihi */
	if (hhsv && (val >= hihi || ((lalm==hihi) && (val >= hihi-hyst)))){
            if (recGblSetSevr(prec,HIHI_ALARM,prec->hhsv)) prec->lalm = hihi;
		return;
	}

	/* alarm condition lolo */
	if (llsv && (val <= lolo || ((lalm==lolo) && (val <= lolo+hyst)))){
            if (recGblSetSevr(prec,LOLO_ALARM,prec->llsv)) prec->lalm = lolo;
		return;
	}

	/* alarm condition high */
	if (hsv && (val >= high || ((lalm==high) && (val >= high-hyst)))){
            if (recGblSetSevr(prec,HIGH_ALARM,prec->hsv)) prec->lalm = high;
		return;
	}

	/* alarm condition low */
	if (lsv && (val <= low || ((lalm==low) && (val <= low+hyst)))){
            if (recGblSetSevr(prec,LOW_ALARM,prec->lsv)) prec->lalm = low;
		return;
	}

	/* we get here only if val is out of alarm by at least hyst */
    prec->lalm = val;
	return;
}

static void monitor(waitRecord *prec)
{
    unsigned short monitor_mask;
    double delta;

    monitor_mask = recGblResetAlarms(prec);
	/* check for value change */
    delta = prec->mlst - prec->val;
	if(delta<0.0) delta = -delta;
    if (delta > prec->mdel) {
		/* post events for value change */
		monitor_mask |= DBE_VALUE;
		/* update last value monitored */
        prec->mlst = prec->val;
	}

	/* check for archive change */
    delta = prec->alst - prec->val;
	if(delta<0.0) delta = -delta;
    if (delta > prec->adel) {
		/* post events on value field for archive change */
		monitor_mask |= DBE_LOG;
		/* update last archive value monitored */
        prec->alst = prec->val;
	}

	/* send out monitors connected to the value field */
	if (monitor_mask){
        db_post_events(prec,&prec->val,monitor_mask);
	}
	return;
}
