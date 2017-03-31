/* ubyteRecord.c */
/* Example record support module */
  
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
#define GEN_SIZE_OFFSET
#include "ubyteRecord.h"
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
 
rset ubyteRSET={
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
epicsExportAddress(rset,ubyteRSET);

static void checkAlarms(ubyteRecord *pubyte);
static void monitor(ubyteRecord *pubyte);

static long init_record(void *precord,int pass)
{
    return(0);
}

static long process(void *precord)
{
	ubyteRecord	*pubyte = (ubyteRecord *)precord;

	pubyte->pact = TRUE;

	recGblGetTimeStamp(pubyte);
	/* check for alarms */
	checkAlarms(pubyte);
	/* check event list */
	monitor(pubyte);
	/* process the forward scan link record */
        recGblFwdLink(pubyte);

	pubyte->pact=FALSE;
	return(0);
}

static long get_units(DBADDR *paddr, char *units)
{
    ubyteRecord	*pubyte=(ubyteRecord *)paddr->precord;

    strncpy(units,pubyte->egu,DB_UNITS_SIZE);
    return(0);
}

static long get_precision(DBADDR *paddr, long *precision)
{
    ubyteRecord	*pubyte=(ubyteRecord *)paddr->precord;

    *precision = pubyte->prec;
    if(paddr->pfield == (void *)&pubyte->val) return(0);
    recGblGetPrec(paddr,precision);
    return(0);
}

static long get_graphic_double(DBADDR *paddr,struct dbr_grDouble *pgd)
{
    ubyteRecord	*pubyte=(ubyteRecord *)paddr->precord;
    int		fieldIndex = dbGetFieldIndex(paddr);

    if(fieldIndex == ubyteRecordVAL
    || fieldIndex == ubyteRecordHIHI
    || fieldIndex == ubyteRecordHIGH
    || fieldIndex == ubyteRecordLOW
    || fieldIndex == ubyteRecordLOLO
    || fieldIndex == ubyteRecordHOPR
    || fieldIndex == ubyteRecordLOPR) {
        pgd->upper_disp_limit = pubyte->hopr;
        pgd->lower_disp_limit = pubyte->lopr;
    } else recGblGetGraphicDouble(paddr,pgd);
    return(0);
}

static long get_control_double(DBADDR *paddr,struct dbr_ctrlDouble *pcd)
{
    ubyteRecord	*pubyte=(ubyteRecord *)paddr->precord;
    int		fieldIndex = dbGetFieldIndex(paddr);

    if(fieldIndex == ubyteRecordVAL
    || fieldIndex == ubyteRecordHIHI
    || fieldIndex == ubyteRecordHIGH
    || fieldIndex == ubyteRecordLOW
    || fieldIndex == ubyteRecordLOLO) {
	pcd->upper_ctrl_limit = pubyte->hopr;
	pcd->lower_ctrl_limit = pubyte->lopr;
    } else recGblGetControlDouble(paddr,pcd);
    return(0);
}

static long get_alarm_double(DBADDR *paddr,struct dbr_alDouble *pad)
{
    ubyteRecord	*pubyte=(ubyteRecord *)paddr->precord;
    int		fieldIndex = dbGetFieldIndex(paddr);

    if(fieldIndex == ubyteRecordVAL) {
	pad->upper_alarm_limit = pubyte->hihi;
	pad->upper_warning_limit = pubyte->high;
	pad->lower_warning_limit = pubyte->low;
	pad->lower_alarm_limit = pubyte->lolo;
    } else recGblGetAlarmDouble(paddr,pad);
    return(0);
}

static void checkAlarms(ubyteRecord *pubyte)
{
	double		val;
	float		hyst, lalm, hihi, high, low, lolo;
	unsigned short	hhsv, llsv, hsv, lsv;

	if(pubyte->udf == TRUE ){
		recGblSetSevr(pubyte,UDF_ALARM,INVALID_ALARM);
		return;
	}
	hihi = pubyte->hihi; lolo = pubyte->lolo; high = pubyte->high; low = pubyte->low;
	hhsv = pubyte->hhsv; llsv = pubyte->llsv; hsv = pubyte->hsv; lsv = pubyte->lsv;
	val = pubyte->val; hyst = pubyte->hyst; lalm = pubyte->lalm;

	/* alarm condition hihi */
	if (hhsv && (val >= hihi || ((lalm==hihi) && (val >= hihi-hyst)))){
	        if (recGblSetSevr(pubyte,HIHI_ALARM,pubyte->hhsv)) pubyte->lalm = hihi;
		return;
	}

	/* alarm condition lolo */
	if (llsv && (val <= lolo || ((lalm==lolo) && (val <= lolo+hyst)))){
	        if (recGblSetSevr(pubyte,LOLO_ALARM,pubyte->llsv)) pubyte->lalm = lolo;
		return;
	}

	/* alarm condition high */
	if (hsv && (val >= high || ((lalm==high) && (val >= high-hyst)))){
	        if (recGblSetSevr(pubyte,HIGH_ALARM,pubyte->hsv)) pubyte->lalm = high;
		return;
	}

	/* alarm condition low */
	if (lsv && (val <= low || ((lalm==low) && (val <= low+hyst)))){
	        if (recGblSetSevr(pubyte,LOW_ALARM,pubyte->lsv)) pubyte->lalm = low;
		return;
	}

	/* we get here only if val is out of alarm by at least hyst */
	pubyte->lalm = val;
	return;
}

static void monitor(ubyteRecord *pubyte)
{
	unsigned short	monitor_mask;
	double		delta;

        monitor_mask = recGblResetAlarms(pubyte);
	/* check for value change */
	delta = pubyte->mlst - pubyte->val;
	if(delta<0.0) delta = -delta;
	if (delta > pubyte->mdel) {
		/* post events for value change */
		monitor_mask |= DBE_VALUE;
		/* update last value monitored */
		pubyte->mlst = pubyte->val;
	}

	/* check for archive change */
	delta = pubyte->alst - pubyte->val;
	if(delta<0.0) delta = -delta;
	if (delta > pubyte->adel) {
		/* post events on value field for archive change */
		monitor_mask |= DBE_LOG;
		/* update last archive value monitored */
		pubyte->alst = pubyte->val;
	}

	/* send out monitors connected to the value field */
	if (monitor_mask){
		db_post_events(pubyte,&pubyte->val,monitor_mask);
	}
	return;
}
