/* icmpRecord.c */
/* Example record support module */
  
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "epicsMath.h"
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
#include "icmpRecord.h"
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
#define get_units  NULL
#define get_precision  NULL
#define get_enum_str NULL
#define get_enum_strs NULL
#define put_enum_str NULL
#define get_graphic_double NULL
#define get_control_double NULL
#define get_alarm_double NULL
 
rset icmpRSET={
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
epicsExportAddress(rset,icmpRSET);

typedef struct icmpset {
	long		number;
	DEVSUPFUN	dev_report;
	DEVSUPFUN	init;
	DEVSUPFUN	init_record; /*returns: (-1,0)=>(failure,success)*/
	DEVSUPFUN	get_ioint_info;
	DEVSUPFUN	read_icmp;
}icmpdset;

static void checkAlarms(icmpRecord *prec);
static void monitor(icmpRecord *prec);

static long init_record(void *precord,int pass)
{
    icmpRecord	*prec = (icmpRecord *)precord;
    icmpdset	*pdset;
    long	status;

    if (pass==0) return(0);

    if(!(pdset = (icmpdset *)(prec->dset))) {
	recGblRecordError(S_dev_noDSET,(void *)prec,"icmp: init_record");
	return(S_dev_noDSET);
    }
    /* must have read_icmp function defined */
    if( (pdset->number < 5) || (pdset->read_icmp == NULL) ) {
	recGblRecordError(S_dev_missingSup,(void *)prec,"icmp: init_record");
	return(S_dev_missingSup);
    }

    if( pdset->init_record ) {
	if((status=(*pdset->init_record)(prec))) return(status);
    }
    return(0);
}
static void checkAlarms(icmpRecord *prec)
{
	double		val;
	float		hyst, lalm, hihi, high, low, lolo;
	unsigned short	hhsv, llsv, hsv, lsv;

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

static long process(void *precord)
{
	icmpRecord	*prec = (icmpRecord *)precord;
	icmpdset		*pdset = (icmpdset *)(prec->dset);
	long		 status;
	unsigned char    pact=prec->pact;

	if( (pdset==NULL) || (pdset->read_icmp==NULL) ) {
		prec->pact=TRUE;
		recGblRecordError(S_dev_missingSup,(void *)prec,"read_icmp");
		return(S_dev_missingSup);
	}

	/* pact must not be set until after calling device support */
	status=(*pdset->read_icmp)(prec);
	/* check if device support set pact */
	if ( !pact && prec->pact ) return(0);
	prec->pact = TRUE;

	recGblGetTimeStamp(prec);
	/* check for alarms */
	checkAlarms(prec);
	/* check event list */
	monitor(prec);
	/* process the forward scan link record */
        recGblFwdLink(prec);

	prec->pact=FALSE;
	return(status);
}


static void monitor(icmpRecord *prec)
{
	unsigned short	monitor_mask;
	double		delta;

    monitor_mask = recGblResetAlarms(prec);

	/* send out monitors connected to the value field */
	if (monitor_mask){
		db_post_events(prec,&prec->val,monitor_mask);
	}
	return;
}
