/* byteRecord.c */
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
#include "byteRecord.h"
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
 
rset byterRSET={
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
epicsExportAddress(rset,byterRSET);

static void checkAlarms(byterRecord *pbyte);
static void monitor(byterRecord *pbyte);

static long init_record(void *precord,int pass)
{
    return(0);
}

static long process(void *precord)
{
    byterRecord	*pbyte = (byterRecord *)precord;

	pbyte->pact = TRUE;

	recGblGetTimeStamp(pbyte);
	/* check for alarms */
	checkAlarms(pbyte);
	/* check event list */
	monitor(pbyte);
	/* process the forward scan link record */
        recGblFwdLink(pbyte);

	pbyte->pact=FALSE;
	return(0);
}

static long get_units(DBADDR *paddr, char *units)
{
    byterRecord	*pbyte=(byterRecord *)paddr->precord;

    strncpy(units,pbyte->egu,DB_UNITS_SIZE);
    return(0);
}

static long get_precision(DBADDR *paddr, long *precision)
{
    byterRecord	*pbyte=(byterRecord *)paddr->precord;

    *precision = pbyte->prec;
    if(paddr->pfield == (void *)&pbyte->val) return(0);
    recGblGetPrec(paddr,precision);
    return(0);
}

static long get_graphic_double(DBADDR *paddr,struct dbr_grDouble *pgd)
{
    byterRecord	*pbyte=(byterRecord *)paddr->precord;
    int		fieldIndex = dbGetFieldIndex(paddr);

    if(fieldIndex == byterRecordVAL
    || fieldIndex == byterRecordHIHI
    || fieldIndex == byterRecordHIGH
    || fieldIndex == byterRecordLOW
    || fieldIndex == byterRecordLOLO
    || fieldIndex == byterRecordHOPR
    || fieldIndex == byterRecordLOPR) {
        pgd->upper_disp_limit = pbyte->hopr;
        pgd->lower_disp_limit = pbyte->lopr;
    } else recGblGetGraphicDouble(paddr,pgd);
    return(0);
}

static long get_control_double(DBADDR *paddr,struct dbr_ctrlDouble *pcd)
{
    byterRecord	*pbyte=(byterRecord *)paddr->precord;
    int		fieldIndex = dbGetFieldIndex(paddr);

    if(fieldIndex == byterRecordVAL
    || fieldIndex == byterRecordHIHI
    || fieldIndex == byterRecordHIGH
    || fieldIndex == byterRecordLOW
    || fieldIndex == byterRecordLOLO) {
	pcd->upper_ctrl_limit = pbyte->hopr;
	pcd->lower_ctrl_limit = pbyte->lopr;
    } else recGblGetControlDouble(paddr,pcd);
    return(0);
}

static long get_alarm_double(DBADDR *paddr,struct dbr_alDouble *pad)
{
    byterRecord	*pbyte=(byterRecord *)paddr->precord;
    int		fieldIndex = dbGetFieldIndex(paddr);

    if(fieldIndex == byterRecordVAL) {
	pad->upper_alarm_limit = pbyte->hihi;
	pad->upper_warning_limit = pbyte->high;
	pad->lower_warning_limit = pbyte->low;
	pad->lower_alarm_limit = pbyte->lolo;
    } else recGblGetAlarmDouble(paddr,pad);
    return(0);
}

static void checkAlarms(byterRecord *pbyte)
{
	double		val;
	float		hyst, lalm, hihi, high, low, lolo;
	unsigned short	hhsv, llsv, hsv, lsv;

	if(pbyte->udf == TRUE ){
		recGblSetSevr(pbyte,UDF_ALARM,INVALID_ALARM);
		return;
	}
	hihi = pbyte->hihi; lolo = pbyte->lolo; high = pbyte->high; low = pbyte->low;
	hhsv = pbyte->hhsv; llsv = pbyte->llsv; hsv = pbyte->hsv; lsv = pbyte->lsv;
	val = pbyte->val; hyst = pbyte->hyst; lalm = pbyte->lalm;

	/* alarm condition hihi */
	if (hhsv && (val >= hihi || ((lalm==hihi) && (val >= hihi-hyst)))){
	        if (recGblSetSevr(pbyte,HIHI_ALARM,pbyte->hhsv)) pbyte->lalm = hihi;
		return;
	}

	/* alarm condition lolo */
	if (llsv && (val <= lolo || ((lalm==lolo) && (val <= lolo+hyst)))){
	        if (recGblSetSevr(pbyte,LOLO_ALARM,pbyte->llsv)) pbyte->lalm = lolo;
		return;
	}

	/* alarm condition high */
	if (hsv && (val >= high || ((lalm==high) && (val >= high-hyst)))){
	        if (recGblSetSevr(pbyte,HIGH_ALARM,pbyte->hsv)) pbyte->lalm = high;
		return;
	}

	/* alarm condition low */
	if (lsv && (val <= low || ((lalm==low) && (val <= low+hyst)))){
	        if (recGblSetSevr(pbyte,LOW_ALARM,pbyte->lsv)) pbyte->lalm = low;
		return;
	}

	/* we get here only if val is out of alarm by at least hyst */
	pbyte->lalm = val;
	return;
}

static void monitor(byterRecord *pbyte)
{
	unsigned short	monitor_mask;
	double		delta;

        monitor_mask = recGblResetAlarms(pbyte);
	/* check for value change */
	delta = pbyte->mlst - pbyte->val;
	if(delta<0.0) delta = -delta;
	if (delta > pbyte->mdel) {
		/* post events for value change */
		monitor_mask |= DBE_VALUE;
		/* update last value monitored */
		pbyte->mlst = pbyte->val;
	}

	/* check for archive change */
	delta = pbyte->alst - pbyte->val;
	if(delta<0.0) delta = -delta;
	if (delta > pbyte->adel) {
		/* post events on value field for archive change */
		monitor_mask |= DBE_LOG;
		/* update last archive value monitored */
		pbyte->alst = pbyte->val;
	}

	/* send out monitors connected to the value field */
	if (monitor_mask){
		db_post_events(pbyte,&pbyte->val,monitor_mask);
	}
	return;
}
