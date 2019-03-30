/* ulongRecord.c */
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
#include "ulongRecord.h"
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
 
rset ulongRSET={
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
epicsExportAddress(rset,ulongRSET);

static void checkAlarms(ulongRecord *pulong);
static void monitor(ulongRecord *pulong);

static long init_record(void *precord,int pass)
{
    return(0);
}

static long process(void *precord)
{
	ulongRecord	*pulong = (ulongRecord *)precord;

	pulong->pact = TRUE;

	recGblGetTimeStamp(pulong);
	/* check for alarms */
	checkAlarms(pulong);
	/* check event list */
	monitor(pulong);
	/* process the forward scan link record */
        recGblFwdLink(pulong);

	pulong->pact=FALSE;
	return(0);
}

static long get_units(DBADDR *paddr, char *units)
{
    ulongRecord	*pulong=(ulongRecord *)paddr->precord;

    strncpy(units,pulong->egu,DB_UNITS_SIZE);
    return(0);
}

static long get_precision(DBADDR *paddr, long *precision)
{
    ulongRecord	*pulong=(ulongRecord *)paddr->precord;

    *precision = pulong->prec;
    if(paddr->pfield == (void *)&pulong->val) return(0);
    recGblGetPrec(paddr,precision);
    return(0);
}

static long get_graphic_double(DBADDR *paddr,struct dbr_grDouble *pgd)
{
    ulongRecord	*pulong=(ulongRecord *)paddr->precord;
    int		fieldIndex = dbGetFieldIndex(paddr);

    if(fieldIndex == ulongRecordVAL
    || fieldIndex == ulongRecordHIHI
    || fieldIndex == ulongRecordHIGH
    || fieldIndex == ulongRecordLOW
    || fieldIndex == ulongRecordLOLO
    || fieldIndex == ulongRecordHOPR
    || fieldIndex == ulongRecordLOPR) {
        pgd->upper_disp_limit = pulong->hopr;
        pgd->lower_disp_limit = pulong->lopr;
    } else recGblGetGraphicDouble(paddr,pgd);
    return(0);
}

static long get_control_double(DBADDR *paddr,struct dbr_ctrlDouble *pcd)
{
    ulongRecord	*pulong=(ulongRecord *)paddr->precord;
    int		fieldIndex = dbGetFieldIndex(paddr);

    if(fieldIndex == ulongRecordVAL
    || fieldIndex == ulongRecordHIHI
    || fieldIndex == ulongRecordHIGH
    || fieldIndex == ulongRecordLOW
    || fieldIndex == ulongRecordLOLO) {
	pcd->upper_ctrl_limit = pulong->hopr;
	pcd->lower_ctrl_limit = pulong->lopr;
    } else recGblGetControlDouble(paddr,pcd);
    return(0);
}

static long get_alarm_double(DBADDR *paddr,struct dbr_alDouble *pad)
{
    ulongRecord	*pulong=(ulongRecord *)paddr->precord;
    int		fieldIndex = dbGetFieldIndex(paddr);

    if(fieldIndex == ulongRecordVAL) {
	pad->upper_alarm_limit = pulong->hihi;
	pad->upper_warning_limit = pulong->high;
	pad->lower_warning_limit = pulong->low;
	pad->lower_alarm_limit = pulong->lolo;
    } else recGblGetAlarmDouble(paddr,pad);
    return(0);
}

static void checkAlarms(ulongRecord *pulong)
{
	double		val;
	float		hyst, lalm, hihi, high, low, lolo;
	unsigned long	hhsv, llsv, hsv, lsv;

	if(pulong->udf == TRUE ){
		recGblSetSevr(pulong,UDF_ALARM,INVALID_ALARM);
		return;
	}
	hihi = pulong->hihi; lolo = pulong->lolo; high = pulong->high; low = pulong->low;
	hhsv = pulong->hhsv; llsv = pulong->llsv; hsv = pulong->hsv; lsv = pulong->lsv;
	val = pulong->val; hyst = pulong->hyst; lalm = pulong->lalm;

	/* alarm condition hihi */
	if (hhsv && (val >= hihi || ((lalm==hihi) && (val >= hihi-hyst)))){
	        if (recGblSetSevr(pulong,HIHI_ALARM,pulong->hhsv)) pulong->lalm = hihi;
		return;
	}

	/* alarm condition lolo */
	if (llsv && (val <= lolo || ((lalm==lolo) && (val <= lolo+hyst)))){
	        if (recGblSetSevr(pulong,LOLO_ALARM,pulong->llsv)) pulong->lalm = lolo;
		return;
	}

	/* alarm condition high */
	if (hsv && (val >= high || ((lalm==high) && (val >= high-hyst)))){
	        if (recGblSetSevr(pulong,HIGH_ALARM,pulong->hsv)) pulong->lalm = high;
		return;
	}

	/* alarm condition low */
	if (lsv && (val <= low || ((lalm==low) && (val <= low+hyst)))){
	        if (recGblSetSevr(pulong,LOW_ALARM,pulong->lsv)) pulong->lalm = low;
		return;
	}

	/* we get here only if val is out of alarm by at least hyst */
	pulong->lalm = val;
	return;
}

static void monitor(ulongRecord *pulong)
{
	unsigned long	monitor_mask;
	double		delta;

        monitor_mask = recGblResetAlarms(pulong);
	/* check for value change */
	delta = pulong->mlst - pulong->val;
	if(delta<0.0) delta = -delta;
	if (delta > pulong->mdel) {
		/* post events for value change */
		monitor_mask |= DBE_VALUE;
		/* update last value monitored */
		pulong->mlst = pulong->val;
	}

	/* check for archive change */
	delta = pulong->alst - pulong->val;
	if(delta<0.0) delta = -delta;
	if (delta > pulong->adel) {
		/* post events on value field for archive change */
		monitor_mask |= DBE_LOG;
		/* update last archive value monitored */
		pulong->alst = pulong->val;
	}

	/* send out monitors connected to the value field */
	if (monitor_mask){
		db_post_events(pulong,&pulong->val,monitor_mask);
	}
	return;
}
