#include "downloadwf.h"

/*
#define CA_DEB 1
*/

int dd_createchan(pv)
DD_PV *pv;
{
  int res;
  char sline[132];

  if( pv->created ) return 0;

#ifdef CA_DEB
  fprintf(stderr,"***********  Enter dd_createchan for %s\n", pv->name);
#endif

  pv->err = 0;
  res = ca_create_channel(pv->name,NULL,0,ddrec.priority,&(pv->chid));
  if(res != ECA_NORMAL) {
	pv->created = 0;
	pv->err = 1;
	sprintf(sline,"%s: %s",pv->name,ca_message(res));
	dd_log(2,1,sline);
	res = -1;
	fprintf(stderr,"%s: %s\n",pv->name,ca_message(res));
	exit(1);
  } else {
	pv->created = 1;
	res = 0;
  }
#ifdef CA_DEB
  fprintf(stderr,"***********  Exit dd_createchan with %d\n", res);
#endif

  return res;
}


int dd_connectpv(pv)
DD_PV *pv;
{
  int res;
  char sline[132];

  if( pv->connected ) return 0;

#ifdef CA_DEB
  fprintf(stderr,"***********  Enter dd_connectpv with pv %s\n",pv->name);
#endif

  if( !pv->created ) {
	dd_createchan(pv);
  }

  pv->err = 0;
  res = ca_pend_io(ddrec.connect_timeout);
  if( res != ECA_NORMAL ) {
	pv->connected = 0;
	pv->err = 1;
	sprintf(sline,"Connection timeout: %s",ca_message(res));
	dd_log(2,1,sline);
	res = -1;
  } else {
	pv->connected = 1;
	res = pv->err = 0;
  }
  pv->nelems = ca_element_count(pv->chid);
  pv->dbrtype = ca_field_type(pv->chid);

#ifdef CA_DEB
  fprintf(stderr,"     PV name %s\n", pv->name);
  fprintf(stderr,"     nlelems %d, dbrtype %d\n", pv->nelems, (int)(pv->dbrtype));
#endif

#ifdef CA_DEB
  fprintf(stderr,"***********  Exit dd_connectpv with code %d\n",res);
#endif

  return res;
}


int dd_connectpvs(npvs,pvs)
int npvs;
DD_PV *pvs;
{
  int i,res=0;
  char sline[132];
  DD_PV *pv;

#ifdef CA_DEB
  fprintf(stderr,"Enter dd_connectpvs with %d pvs: %s\n",npvs,pvs->name);
#endif
  if( npvs == 1 ) {
	return dd_connectpv(pvs);
  }
  pv = pvs;
  for(i=0;i<npvs;i++) {
	if( pv->created == 0 )
		dd_createchan(pv);
	++pv;
  }

  res = ca_pend_io(ddrec.connect_timeout);
  if( res != ECA_NORMAL ) {
	pv = pvs;
	for(i=0;i<npvs;i++) {
		pv->connected = 0;
		pv->err = 1;
		++pv;
	}
	sprintf(sline,"Connection timeout: %s",ca_message(res));
	dd_log(2,1,sline);
	res = -1;
  } else {
	pv = pvs;
	for(i=0;i<npvs;i++) {
		pv->connected = 1;
		pv->err = 0;
		++pv;
	}
	res = 0;
  }
  pv = pvs;
  for(i=0;i<npvs;i++) {
	pv->nelems = ca_element_count(pv->chid);
	pv->dbrtype = ca_field_type(pv->chid);
	++pv;
  }
#ifdef CA_DEB
  fprintf(stderr,"Exit dd_connectpvs with %d\n",res);
#endif
  return res;
}


int dd_readpvs( npvs, pvs, connected, allocated )
int npvs;
DD_PV *pvs;
int connected;
int allocated;
{
  int i;
  int res;
  int nconns;
  char sline[132];
  DD_PV *ppv;

#ifdef CA_DEB
  fprintf(stderr,"************ Enter dd_readpvs: %s\n", pvs->name);
#endif
  if( !connected ) {
	res =  dd_connectpvs(npvs,pvs);
  }

  nconns = 0;
  ppv = pvs;
  for (i = 0; i < npvs; i++) {
	if( ca_state(ppv->chid) == cs_conn ) {
		ppv->value_size = dbr_size_n(ppv->dbrtype, ppv->nelems);
#ifdef CA_DEB
		fprintf(stderr,"    Number of elem, DBR type, size: %d %d %d\n",ppv->nelems,
			ppv->dbrtype, ppv->value_size);
#endif
		if( !allocated ) {
			ppv->dbr_value = realloc(ppv->dbr_value, ppv->value_size);
			if( ppv->dbr_value == NULL ) {
				ppv->err = 1;
				sprintf(sline,"dd_readpvs: Memory allocation error");
				dd_log(2,1,sline);
				return -1;
			}
			ppv->allocated = 1;
		}
		++nconns;
		ppv->connected = 1;
		res = ca_array_get(ppv->dbrtype, ppv->nelems, ppv->chid, ppv->dbr_value);
	} else {
		ppv->err = 1;
	}
	++ppv;
  }
  if( nconns == 0 ) {
	sprintf(sline, "No PVs connected");
	dd_log(2,1,sline);
	return -1;
  }

  res = ca_pend_io(5*ddrec.read_timeout);
  if (res == ECA_TIMEOUT) {
	sprintf(sline,"dd_readpvs: %s",ca_message(res));
	dd_log(2,1,sline);
	return -1;
  }

  return 0;
}


int dd_readpvs_dbrtype( npvs, pvs, dbrtype, connected, allocated )
int npvs;
DD_PV *pvs;
long dbrtype;
int connected;
int allocated;
{
  int i;
  int res;
  int nconns;
  char sline[132];
  DD_PV *ppv;

  if( !connected ) {
	res =  dd_connectpvs(npvs,pvs);
  }

  nconns = 0;
  ppv = pvs;
  for (i = 0; i < npvs; i++) {
	if( ca_state(ppv->chid) == cs_conn ) {
		ppv->value_size = dbr_size_n(dbrtype, ppv->nelems);
		if( !allocated ) {
			ppv->dbr_value = realloc(ppv->dbr_value, ppv->value_size);
			if( ppv->dbr_value == NULL ) {
				ppv->err = 1;
				sprintf(sline,"dd_readpv: Memory allocation error");
				dd_log(2,1,sline);
				return -1;
			}
			ppv->allocated = 1;
		}
		++nconns;
		ppv->connected = 1;
		res = ca_array_get(dbrtype, ppv->nelems, ppv->chid, ppv->dbr_value);
	} else {
		ppv->err = 1;
	}
	++ppv;
  }
  if( nconns == 0 ) {
	sprintf(sline, "No PVs connected");
	dd_log(2,1,sline);
	return -1;
  }

  res = ca_pend_io(5*ddrec.read_timeout);
  if (res == ECA_TIMEOUT) {
	sprintf(sline,"dd_pvread: %s",ca_message(res));
	dd_log(2,1,sline);
	return -1;
  }

  return 0;
}


int dd_writepvs_bo( npvs, pvs, values, connected )
int npvs;
DD_PV *pvs;
short *values;
int connected;
{
  int i, res, nconns;
  char sline[128];
  DD_PV *ppv;

#ifdef CA_DEB
  fprintf(stderr,"************ Enter dd_writepvs_bo: %s\n", pvs->name);
  fprintf(stderr,"             connected: %d\n",connected);
#endif
  if( !connected ) {
	res =  dd_connectpvs(npvs,pvs);
  }

  nconns = 0;
  ppv = pvs;
  for (i = 0; i < npvs; i++) {
	if( ca_state(ppv->chid) == cs_conn ) {
		++nconns;
		ppv->connected = 1;
		res = ca_put(DBR_SHORT, ppv->chid, values+i);
	} else {
		ppv->err = 1;
	}
	++ppv;
  }

  if( nconns == 0 ) {
	sprintf(sline, "No PVs connected");
	dd_log(2,1,sline);
	return -1;
  }

  res = ca_pend_io(5*ddrec.read_timeout);
  if (res == ECA_TIMEOUT) {
	sprintf(sline,"dd_writepvs_bo: %s",ca_message(res));
	dd_log(2,1,sline);
	return -1;
  }
  return 0;
}

