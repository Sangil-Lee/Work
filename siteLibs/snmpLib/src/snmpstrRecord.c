/* snmpstrRecord.c */
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
#include "snmpstrRecord.h"
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

#define SNMPSTR 0

#if SNMPSTR
static long get_graphic_double();
static long get_control_double();
static long get_alarm_double();
rset snmpstrRSET={
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
#else
rset snmpstrRSET={
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
	NULL,
	NULL,
	NULL
};
#endif
epicsExportAddress(rset,snmpstrRSET);

typedef struct snmpstrset { /* snmpstr input dset */
	long		number;
	DEVSUPFUN	dev_report;
	DEVSUPFUN	init;
	DEVSUPFUN	init_record; /*returns: (-1,0)=>(failure,success)*/
	DEVSUPFUN	get_ioint_info;
	DEVSUPFUN	read_snmpstr;
}snmpstrdset;

static void checkAlarms(snmpstrRecord *psnmpstr);
static void monitor(snmpstrRecord *psnmpstr);

static long init_record(void *precord,int pass)
{
    snmpstrRecord	*psnmpstr = (snmpstrRecord *)precord;
    snmpstrdset	*pdset;
    long	status;

    if (pass==0) return(0);

    if(!(pdset = (snmpstrdset *)(psnmpstr->dset))) {
	recGblRecordError(S_dev_noDSET,(void *)psnmpstr,"snmpstr: init_record");
	return(S_dev_noDSET);
    }
    /* must have read_snmpstr function defined */
    if( (pdset->number < 5) || (pdset->read_snmpstr == NULL) ) {
	recGblRecordError(S_dev_missingSup,(void *)psnmpstr,"snmpstr: init_record");
	return(S_dev_missingSup);
    }

    if( pdset->init_record ) {
	if((status=(*pdset->init_record)(psnmpstr))) return(status);
    }
    return(0);
}

static long process(void *precord)
{
	snmpstrRecord	*psnmpstr = (snmpstrRecord *)precord;
	snmpstrdset		*pdset = (snmpstrdset *)(psnmpstr->dset);
	long		 status;
	unsigned char    pact=psnmpstr->pact;

	if( (pdset==NULL) || (pdset->read_snmpstr==NULL) ) {
		psnmpstr->pact=TRUE;
		recGblRecordError(S_dev_missingSup,(void *)psnmpstr,"read_snmpstr");
		return(S_dev_missingSup);
	}

	/* pact must not be set until after calling device support */
	status=(*pdset->read_snmpstr)(psnmpstr);
	/* check if device support set pact */
	if ( !pact && psnmpstr->pact ) return(0);
	psnmpstr->pact = TRUE;

	recGblGetTimeStamp(psnmpstr);

	/* check for alarms */
	//checkAlarms(psnmpstr);

	/* check event list */
	monitor(psnmpstr);
	/* process the forward scan link record */
        recGblFwdLink(psnmpstr);

	psnmpstr->pact=FALSE;
	return(status);
}

static long get_units(DBADDR *paddr, char *units)
{
    snmpstrRecord	*psnmpstr=(snmpstrRecord *)paddr->precord;

    strncpy(units,psnmpstr->egu,DB_UNITS_SIZE);
    return(0);
}

static long get_precision(DBADDR *paddr, long *precision)
{
    snmpstrRecord	*psnmpstr=(snmpstrRecord *)paddr->precord;

    *precision = psnmpstr->prec;
    if(paddr->pfield == (void *)&psnmpstr->val) return(0);
    recGblGetPrec(paddr,precision);
    return(0);
}

#if SNMPSTR
static long get_graphic_double(DBADDR *paddr,struct dbr_grDouble *pgd)
{
    snmpstrRecord	*psnmpstr=(snmpstrRecord *)paddr->precord;
    int		fieldIndex = dbGetFieldIndex(paddr);

    if(fieldIndex == snmpstrRecordVAL
    || fieldIndex == snmpstrRecordHIHI
    || fieldIndex == snmpstrRecordHIGH
    || fieldIndex == snmpstrRecordLOW
    || fieldIndex == snmpstrRecordLOLO
    || fieldIndex == snmpstrRecordHOPR
    || fieldIndex == snmpstrRecordLOPR) {
        pgd->upper_disp_limit = psnmpstr->hopr;
        pgd->lower_disp_limit = psnmpstr->lopr;
    } else recGblGetGraphicDouble(paddr,pgd);
    return(0);
}

static long get_control_double(DBADDR *paddr,struct dbr_ctrlDouble *pcd)
{
    snmpstrRecord	*psnmpstr=(snmpstrRecord *)paddr->precord;
    int		fieldIndex = dbGetFieldIndex(paddr);

    if(fieldIndex == snmpstrRecordVAL
    || fieldIndex == snmpstrRecordHIHI
    || fieldIndex == snmpstrRecordHIGH
    || fieldIndex == snmpstrRecordLOW
    || fieldIndex == snmpstrRecordLOLO) {
	pcd->upper_ctrl_limit = psnmpstr->hopr;
	pcd->lower_ctrl_limit = psnmpstr->lopr;
    } else recGblGetControlDouble(paddr,pcd);
    return(0);
}

static long get_alarm_double(DBADDR *paddr,struct dbr_alDouble *pad)
{
    snmpstrRecord	*psnmpstr=(snmpstrRecord *)paddr->precord;
    int		fieldIndex = dbGetFieldIndex(paddr);

    if(fieldIndex == snmpstrRecordVAL) {
	pad->upper_alarm_limit = psnmpstr->hihi;
	pad->upper_warning_limit = psnmpstr->high;
	pad->lower_warning_limit = psnmpstr->low;
	pad->lower_alarm_limit = psnmpstr->lolo;
    } else recGblGetAlarmDouble(paddr,pad);
    return(0);
}

static void checkAlarms(snmpstrRecord *psnmpstr)
{
	double		val;
	float		hyst, lalm, hihi, high, low, lolo;
	unsigned short	hhsv, llsv, hsv, lsv;

	if(psnmpstr->udf == TRUE ){
		recGblSetSevr(psnmpstr,UDF_ALARM,INVALID_ALARM);
		return;
	}
	hihi = psnmpstr->hihi; lolo = psnmpstr->lolo; high = psnmpstr->high; low = psnmpstr->low;
	hhsv = psnmpstr->hhsv; llsv = psnmpstr->llsv; hsv = psnmpstr->hsv; lsv = psnmpstr->lsv;
	val = psnmpstr->val; hyst = psnmpstr->hyst; lalm = psnmpstr->lalm;

	/* alarm condition hihi */
	if (hhsv && (val >= hihi || ((lalm==hihi) && (val >= hihi-hyst)))){
	        if (recGblSetSevr(psnmpstr,HIHI_ALARM,psnmpstr->hhsv)) psnmpstr->lalm = hihi;
		return;
	}

	/* alarm condition lolo */
	if (llsv && (val <= lolo || ((lalm==lolo) && (val <= lolo+hyst)))){
	        if (recGblSetSevr(psnmpstr,LOLO_ALARM,psnmpstr->llsv)) psnmpstr->lalm = lolo;
		return;
	}

	/* alarm condition high */
	if (hsv && (val >= high || ((lalm==high) && (val >= high-hyst)))){
	        if (recGblSetSevr(psnmpstr,HIGH_ALARM,psnmpstr->hsv)) psnmpstr->lalm = high;
		return;
	}

	/* alarm condition low */
	if (lsv && (val <= low || ((lalm==low) && (val <= low+hyst)))){
	        if (recGblSetSevr(psnmpstr,LOW_ALARM,psnmpstr->lsv)) psnmpstr->lalm = low;
		return;
	}

	/* we get here only if val is out of alarm by at least hyst */
	psnmpstr->lalm = val;
	return;
}
#endif

static void monitor(snmpstrRecord *psnmpstr)
{
	unsigned short	monitor_mask = 0;
	double		delta = 0.0 ;
	// better to use fabs() instead of if condition.
	// jhlee
        monitor_mask = recGblResetAlarms(psnmpstr);
	/* check for value change */
	/* delta = psnmpstr->mlst - psnmpstr->val; */
	if(delta<0.0) delta = -delta;
	if (delta > psnmpstr->mdel) {
		/* post events for value change */
		monitor_mask |= DBE_VALUE;
		/* update last value monitored */
		/* psnmpstr->mlst = psnmpstr->val; */
	}

	/* check for archive change */
	/* delta = psnmpstr->alst - psnmpstr->val; */
	if(delta<0.0) delta = -delta;
	if (delta > psnmpstr->adel) {
		/* post events on value field for archive change */
		monitor_mask |= DBE_LOG;
		/* update last archive value monitored */
		/* psnmpstr->alst = psnmpstr->val; */
	}

	/* send out monitors connected to the value field */
	if (monitor_mask){
		db_post_events(psnmpstr,&psnmpstr->val,monitor_mask);
	}
	return;
}
