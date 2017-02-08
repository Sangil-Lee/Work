/* ushortRecord.c */
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
#include "ushortRecord.h"
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
 
rset ushortRSET={
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
epicsExportAddress(rset,ushortRSET);

static void checkAlarms(ushortRecord *pushort);
static void monitor(ushortRecord *pushort);

static long init_record(void *precord,int pass)
{
    return(0);
}

static long process(void *precord)
{
	ushortRecord	*pushort = (ushortRecord *)precord;

	pushort->pact = TRUE;

	recGblGetTimeStamp(pushort);
	/* check for alarms */
	checkAlarms(pushort);
	/* check event list */
	monitor(pushort);
	/* process the forward scan link record */
        recGblFwdLink(pushort);

	pushort->pact=FALSE;
	return(0);
}

static long get_units(DBADDR *paddr, char *units)
{
    ushortRecord	*pushort=(ushortRecord *)paddr->precord;

    strncpy(units,pushort->egu,DB_UNITS_SIZE);
    return(0);
}

static long get_precision(DBADDR *paddr, long *precision)
{
    ushortRecord	*pushort=(ushortRecord *)paddr->precord;

    *precision = pushort->prec;
    if(paddr->pfield == (void *)&pushort->val) return(0);
    recGblGetPrec(paddr,precision);
    return(0);
}

static long get_graphic_double(DBADDR *paddr,struct dbr_grDouble *pgd)
{
    ushortRecord	*pushort=(ushortRecord *)paddr->precord;
    int		fieldIndex = dbGetFieldIndex(paddr);

    if(fieldIndex == ushortRecordVAL
    || fieldIndex == ushortRecordHIHI
    || fieldIndex == ushortRecordHIGH
    || fieldIndex == ushortRecordLOW
    || fieldIndex == ushortRecordLOLO
    || fieldIndex == ushortRecordHOPR
    || fieldIndex == ushortRecordLOPR) {
        pgd->upper_disp_limit = pushort->hopr;
        pgd->lower_disp_limit = pushort->lopr;
    } else recGblGetGraphicDouble(paddr,pgd);
    return(0);
}

static long get_control_double(DBADDR *paddr,struct dbr_ctrlDouble *pcd)
{
    ushortRecord	*pushort=(ushortRecord *)paddr->precord;
    int		fieldIndex = dbGetFieldIndex(paddr);

    if(fieldIndex == ushortRecordVAL
    || fieldIndex == ushortRecordHIHI
    || fieldIndex == ushortRecordHIGH
    || fieldIndex == ushortRecordLOW
    || fieldIndex == ushortRecordLOLO) {
	pcd->upper_ctrl_limit = pushort->hopr;
	pcd->lower_ctrl_limit = pushort->lopr;
    } else recGblGetControlDouble(paddr,pcd);
    return(0);
}

static long get_alarm_double(DBADDR *paddr,struct dbr_alDouble *pad)
{
    ushortRecord	*pushort=(ushortRecord *)paddr->precord;
    int		fieldIndex = dbGetFieldIndex(paddr);

    if(fieldIndex == ushortRecordVAL) {
	pad->upper_alarm_limit = pushort->hihi;
	pad->upper_warning_limit = pushort->high;
	pad->lower_warning_limit = pushort->low;
	pad->lower_alarm_limit = pushort->lolo;
    } else recGblGetAlarmDouble(paddr,pad);
    return(0);
}

static void checkAlarms(ushortRecord *pushort)
{
	double		val;
	float		hyst, lalm, hihi, high, low, lolo;
	unsigned short	hhsv, llsv, hsv, lsv;

	if(pushort->udf == TRUE ){
		recGblSetSevr(pushort,UDF_ALARM,INVALID_ALARM);
		return;
	}
	hihi = pushort->hihi; lolo = pushort->lolo; high = pushort->high; low = pushort->low;
	hhsv = pushort->hhsv; llsv = pushort->llsv; hsv = pushort->hsv; lsv = pushort->lsv;
	val = pushort->val; hyst = pushort->hyst; lalm = pushort->lalm;

	/* alarm condition hihi */
	if (hhsv && (val >= hihi || ((lalm==hihi) && (val >= hihi-hyst)))){
	        if (recGblSetSevr(pushort,HIHI_ALARM,pushort->hhsv)) pushort->lalm = hihi;
		return;
	}

	/* alarm condition lolo */
	if (llsv && (val <= lolo || ((lalm==lolo) && (val <= lolo+hyst)))){
	        if (recGblSetSevr(pushort,LOLO_ALARM,pushort->llsv)) pushort->lalm = lolo;
		return;
	}

	/* alarm condition high */
	if (hsv && (val >= high || ((lalm==high) && (val >= high-hyst)))){
	        if (recGblSetSevr(pushort,HIGH_ALARM,pushort->hsv)) pushort->lalm = high;
		return;
	}

	/* alarm condition low */
	if (lsv && (val <= low || ((lalm==low) && (val <= low+hyst)))){
	        if (recGblSetSevr(pushort,LOW_ALARM,pushort->lsv)) pushort->lalm = low;
		return;
	}

	/* we get here only if val is out of alarm by at least hyst */
	pushort->lalm = val;
	return;
}

static void monitor(ushortRecord *pushort)
{
	unsigned short	monitor_mask;
	double		delta;

        monitor_mask = recGblResetAlarms(pushort);
	/* check for value change */
	delta = pushort->mlst - pushort->val;
	if(delta<0.0) delta = -delta;
	if (delta > pushort->mdel) {
		/* post events for value change */
		monitor_mask |= DBE_VALUE;
		/* update last value monitored */
		pushort->mlst = pushort->val;
	}

	/* check for archive change */
	delta = pushort->alst - pushort->val;
	if(delta<0.0) delta = -delta;
	if (delta > pushort->adel) {
		/* post events on value field for archive change */
		monitor_mask |= DBE_LOG;
		/* update last archive value monitored */
		pushort->alst = pushort->val;
	}

	/* send out monitors connected to the value field */
	if (monitor_mask){
		db_post_events(pushort,&pushort->val,monitor_mask);
	}
	return;
}
