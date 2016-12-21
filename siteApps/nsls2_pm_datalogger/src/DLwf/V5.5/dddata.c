#include "downloadwf.h"
#include <db_access.h>
int dd_readpvs();

/* We'll consider multiple triggers. We only have one AI trigger at present */

int dd_readstatus(dbrtype, status, ts, connected, allocated) 
long dbrtype;
int *status;
TS_STR ts;
int connected, allocated;
{ 
  int res;
  epicsTimeStamp stamp;
  struct dbr_time_short *tint;
  char *timeFormatStr = "%Y%m%d-%H:%M:%S.%06f";

  res = dd_readpvs_dbrtype(1, &(ddrecptr->trig_pv[0]), dbrtype, connected, allocated);
  if( res == 0 ) {
	tint = (struct dbr_time_short *)(ddrecptr->trig_pv[0].dbr_value);
	*status = tint->value;
/*
#ifdef DD_DEB
	fprintf(stderr,"Status PV: %s value: %d\n",ddrecptr->trig_pv[0].name, *status);
#endif
*/
	stamp = (epicsTimeStamp)tint->stamp;
	epicsTimeToStrftime(ts, TS_LENGTH, timeFormatStr,&stamp);
	strcpy(ddrecptr->trig_timestamp, ts);
	return 0;
  } else return -1;
}


int dd_resetstatus(connected)
int connected;
{
  int i,res;
  short values[MAX_RESETS];

#ifdef DD_DEB
  fprintf(stderr,"Reset PV: %s with value %d\n",ddrecptr->reset_pv[0].name,
	ddrecptr->reset_value[0]);
#endif
  for(i=0;i<ddrecptr->nresets;i++) {
	values[i] = ddrecptr->reset_value[i];
  }
  res = dd_writepvs_bo(ddrecptr->nresets, ddrecptr->reset_pv, values, connected);
#ifdef DD_DEB
  if( res == 0 ) fprintf(stderr,"Reset successful\n");
#endif
  return res;
}

