/*
 * NET-SNMP demo
 *
 * This program demonstrates different ways to query a list of hosts
 * for a list of variables.
 *
 * It would of course be faster just to send one query for all variables,
 * but the intention is to demonstrate the difference between synchronous
 * and asynchronous operation.
 *
 * Niels Baggesen (Niels.Baggesen@uni-c.dk), 1999.
 */

/*
 * snmpDevAsync.c
 * Mi Jeong Park, mijoy0909@ibs.re.kr
 * ---------------------------------------------------------
 * log
 * ---------------------------------------------------------
 * Wednesday, December 31 17:36:45 KST 2014 (v2c read only)
 * Monday, January  5 13:10:27 KST 2015 (v2c/3 read only)
 * Friday, January  9 16:45:32 KST 2015 (read only - sscanf -> snmprecord inp field) thanks to SL 
 * Monday, January 12 10:36:21 KST 2015 (read only - sscanf -> snmprecord host,comm,oids field)
 * Tuesday, January 13 17:51:42 KST 2015 (read only - snmprecord host,comm,oids field)
 * Thursday, January 15 15:49:37 KST 2015 modify something
 * Friday, January 16 16:53:23 KST 2015 snmpv3 read/write done! 
 * Tuesday, February  3 09:48:42 KST 2015
 * Wednesday, February  4 17:15:55 KST 2015 snmpv3 read/write done!! thanks to SL
 * Tuesday, February 17 09:27:48 KST 2015 snmpstr ing.....
 * Monday, February 23 16:43:29 KST 2015 using auth, priv, vers field read/write ok!
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <alarm.h>
#include <cvtTable.h>
#include <dbDefs.h>
#include <dbAccess.h>
#include <recGbl.h>
#include <recSup.h>
#include <devSup.h>
#include <callback.h>
#include <link.h>
#include <snmpRecord.h>
#include <epicsExport.h>
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

/*
 * a list of variables to query for
 */
struct oid_info {
  char Name[81];
  oid Oid[MAX_OID_LEN];
  unsigned int OidLen;
};

typedef struct oid_info OID;
typedef struct snmp_info
{
  OID  oid_info;
  struct snmp_session *sess;
  struct snmp_session ss;
  struct snmp_pdu *getreq;                           /* startup all hosts */
  struct snmp_pdu *setreq;                           /* startup all hosts */
  char sval[12];
  char vers[40];
  char type;
} SNMP_INFO;

//psnmp_int
void initialize(void *precord);
void get_snmp(void *precord);
int set_snmp(void *precord, const char *sval);
int return_data(int status, struct snmp_session *sp, struct snmp_pdu *pdu);
int asynch_response(int operation, struct snmp_session *sp, int reqid, struct snmp_pdu *pdu, void *magic);
long snmp_Init(void *precord);
long snmp_Write(void *precord);
long snmp_Read(void *precord);
void active_hosts();

//psnmp_str
/* long snmp_strInit(void *precord); */
/* long snmp_strRead(void *precord); */

/* static SNMP_INFO *snmpinfo = NULL; */
/* static SNMP_INFO *gsnmpinfo = NULL; */

static struct snmpRecord *gpSnmp = NULL;


/* static struct snmpstrRecord *gpSnmpstr = NULL; */
int hosts = 0;		                           	 /* hosts that we have not completed */

typedef struct devSNMPSoft{
  long number;
  DEVSUPFUN report;
  DEVSUPFUN init;
  DEVSUPFUN init_record;
  DEVSUPFUN get_ioint_info;
  DEVSUPFUN read_write_record;
  DEVSUPFUN special_linconv;
}devSNMPSoft;

devSNMPSoft devSNMPRead = {6, NULL, NULL, snmp_Init, NULL, snmp_Read, NULL};
devSNMPSoft devSNMPWrite = {6, NULL, NULL, snmp_Init, NULL, snmp_Write, NULL};
epicsExportAddress(dset, devSNMPRead);
epicsExportAddress(dset, devSNMPWrite);

/* ************************************************************************************************************** */

long snmp_Init(void *precord)
{            
  snmpRecord *psnmp = (snmpRecord *)precord;
  SNMP_INFO *snmpinfo = (SNMP_INFO *)malloc(sizeof(SNMP_INFO));       //leesi
  psnmp->dpvt = (SNMP_INFO*)snmpinfo;

  switch (psnmp->inp.type) {
  case INST_IO:
    break;
  default :
    recGblRecordError(S_db_badField, (void *)psnmp,
  		      "devSNMPSoft (init_record) Illegal INP field");
    return(S_db_badField);
  }
  /* printf("Host:%s, Communit:%s, OID:%s\n", psnmp->host, psnmp->comm, psnmp->oids); */
  initialize(psnmp);
  psnmp->udf = FALSE;
    
  return 0;
}

long snmp_Write(void *precord)
{
  snmpRecord *psnmp = (snmpRecord *)precord;
  SNMP_INFO *snmpinfo = (SNMP_INFO*)psnmp->dpvt;           //leesi
  long status;

  if(psnmp->pact) {
    psnmp->val = 0;
    psnmp->udf = FALSE;
    return 2;
  }

  if(psnmp->rval == psnmp->oval) return 2;   //leesi
  
  psnmp->pact = TRUE;
  gpSnmp = psnmp;

  sprintf(snmpinfo->sval, "%d", psnmp->rval); //leesi
  if(set_snmp(psnmp, snmpinfo->sval) == 0)    //leesi
    {
      psnmp->oval = psnmp->rval;
    }

  psnmp->pact = FALSE;

  return(status);
}

long snmp_Read(void *precord)
{
  snmpRecord *psnmp = (snmpRecord *)precord;
  long status;

  if(psnmp->pact) {
    psnmp->val = 0;
    psnmp->udf = FALSE;
    return 2;
  }
  psnmp->pact = TRUE;
  gpSnmp = psnmp;
  get_snmp(psnmp);

  psnmp->pact = FALSE;

  return(status);
}

/* ************************************************************************************************************** */

void initialize(void *precord)
{
  snmpRecord *psnmp = (snmpRecord *)precord;
  SNMP_INFO *snmpinfo = (SNMP_INFO*)psnmp->dpvt;

  static int snmpInited = FALSE;
  snmpinfo->oid_info.OidLen = MAX_OID_LEN;
  if (!snmpInited)
    {
      init_snmp("snmpDevAsync.c");                    /* initialize library */
      snmpInited = TRUE;
    };

  if (strlen(psnmp->vers) == 14) {                    //VERSION 3
    snmp_sess_init(&(snmpinfo->ss));                  /* initialize session */    
    snmpinfo->ss.version = SNMP_VERSION_3;
    snmpinfo->ss.peername = strdup(psnmp->host);
    snmpinfo->ss.securityName =  strdup(psnmp->comm);
    snmpinfo->ss.securityNameLen = strlen((char*)snmpinfo->ss.securityName);
    snmpinfo->ss.securityLevel = SNMP_SEC_LEVEL_AUTHPRIV;

    /* Authentication Protocol *//* MD5 */
    snmpinfo->ss.securityAuthProto = usmHMACMD5AuthProtocol;
    snmpinfo->ss.securityAuthProtoLen = sizeof(usmHMACMD5AuthProtocol)/sizeof(oid);
    snmpinfo->ss.securityAuthKeyLen = USM_AUTH_KU_LEN;

    /* Authentication keys generation */
    if (generate_Ku(snmpinfo->ss.securityAuthProto,
		    snmpinfo->ss.securityAuthProtoLen,
		    (u_char *) strdup(psnmp->auth), strlen(psnmp->auth),
		    snmpinfo->ss.securityAuthKey,
		    &snmpinfo->ss.securityAuthKeyLen) != SNMPERR_SUCCESS) {
      snmp_perror("Error!!!!!!!!!!!!");
      snmp_log(LOG_ERR,
	       "Error generating Ku from authentication pass phrase. \n");
      exit(1);
    }

    /* Privacy protocol *//* DES */
    snmpinfo->ss.securityPrivProto = usmDESPrivProtocol;
    snmpinfo->ss.securityPrivProtoLen = USM_PRIV_PROTO_DES_LEN;
    snmpinfo->ss.securityPrivKeyLen = USM_PRIV_KU_LEN;

    /* Privacy keys generation */
    if (generate_Ku(snmpinfo->ss.securityAuthProto,
		    snmpinfo->ss.securityAuthProtoLen,
		    (u_char *) strdup(psnmp->priv), strlen(psnmp->priv),
		    snmpinfo->ss.securityPrivKey,
		    &snmpinfo->ss.securityPrivKeyLen) != SNMPERR_SUCCESS) {
      printf ("Error generating Ku from privacity pass phrase. \n");
      exit(1);
    }
    snmpinfo->ss.callback = asynch_response;		/* default callback */
    snmpinfo->ss.callback_magic = snmpinfo;
  } else {                                              //VERSION 2c
    snmp_sess_init(&(snmpinfo->ss));			/* initialize session */
    snmpinfo->ss.version = SNMP_VERSION_2c;
    snmpinfo->ss.peername = strdup(psnmp->host);
    snmpinfo->ss.community = (unsigned char*)strdup(psnmp->comm);
    snmpinfo->ss.community_len = strlen((char*)snmpinfo->ss.community);
    snmpinfo->ss.callback = asynch_response;		/* default callback */
    snmpinfo->ss.callback_magic = snmpinfo;
  }

  if (!read_objid(psnmp->oids, snmpinfo->oid_info.Oid, &snmpinfo->oid_info.OidLen))      /* parse the oids */
    {
      printf("parse the oids %s\n",psnmp->oids);
      snmp_perror("read_objid");
      exit(1);
    }
}

int return_data(int status, struct snmp_session *sp, struct snmp_pdu *pdu)               /* simple printing of returned data */
{
  char buf[1024];
  struct variable_list *vp;
  int ix;
  char *getdata = NULL;
  unsigned long int nVal;
  /* unsigned char sVal; */
  /* int ipVal; */

  switch (status) {
  case STAT_SUCCESS:
    vp = pdu->variables;
    if (pdu->errstat == SNMP_ERR_NOERROR) {
      while (vp) {
	snprint_variable(buf, sizeof(buf), vp->name, vp->name_length, vp);
	/*mjpark------------------------------------------*/
	getdata = (char *)malloc(1 + vp->val_len);
	memcpy(getdata, vp->val.string, vp->val_len);
	getdata[vp->val_len] = '\0';

	switch(vp->type)
	  {
	  case ASN_OCTET_STR:
	    /* { */
	    /* memcpy((void *)&sVal, getdata, sizeof(unsigned char)); */
	    /* gpSnmpstr->val = sVal; */
	    /* break; */
	    /* } */
	  case ASN_GAUGE:
	  case ASN_COUNTER:
	  case ASN_TIMETICKS:
	  case ASN_INTEGER:
	    {
	      memcpy((void *)&nVal, getdata, sizeof(unsigned long int));
	      gpSnmp->val = nVal;
	      break;
	    }
	  case ASN_IPADDRESS:
	    {
	      /* struct in_addr st_addr; */
    	      /* memcpy((void *)&ipVal, getdata, 4); */
	      /* st_addr.s_addr = ipVal; */
	      /* gpSnmpstr->val = inet_ntoa(st_addr); */
	      break;
	    }
	  }
	//debug print
	//printf("data length(%d), unsigned long int size(%d), int(%d)\n",vp->val_len, sizeof(unsigned long int), sizeof(int));
	/* printf("snmpVal(%d), psnmp(%p)\n", nVal, gpSnmp); */
	/*------------------------------------------------*/
	vp = vp->next_variable;
      }
      free(getdata);
    }
    else {
      for (ix = 1; vp && ix != pdu->errindex; vp = vp->next_variable, ix++);
      if (vp)
	snprint_objid(buf, sizeof(buf), vp->name, vp->name_length);
      else
	strcpy(buf, "(none)");
      fprintf(stdout, "%s: %s: %s\n", sp->peername, buf, snmp_errstring(pdu->errstat));
    }
    return 1;
  case STAT_TIMEOUT:
    fprintf(stdout, "%s: Timeout\n", sp->peername);
    return 0;
  
  case STAT_ERROR:
    snmp_perror(sp->peername);
    return 0;
  }
  return 0;
}

/* response handler */
int asynch_response(int operation, struct snmp_session *sp, int reqid, struct snmp_pdu *pdu, void *magic)
{
  SNMP_INFO *snmpinfo = (SNMP_INFO*)magic;
#if 0
  static int count = 0;
  printf("Response:%d\n", count++);
#endif

  if ((operation == NETSNMP_CALLBACK_OP_RECEIVED_MESSAGE)) {
    return_data(STAT_SUCCESS, snmpinfo->sess, pdu);
  } else {
    return_data(STAT_TIMEOUT, snmpinfo->sess, pdu);
  }
  /* something went wrong (or end of variables)
   * this host not active any more
   */
  hosts--;
  return 1;
}

void active_hosts()
{
  /* loop while any active hosts */
  while (hosts) {
    /* printf("Active Host: %d\n", hosts); */
    int fds = 0, block = 1;
    fd_set fdset;
    struct timeval timeout;

    FD_ZERO(&fdset);
    snmp_select_info(&fds, &fdset, &timeout, &block);
    fds = select(fds, &fdset, NULL, NULL, block ? NULL : &timeout);
    if (fds < 0) {
      perror("select failed");
      exit(1);
    }
    if (fds)
      snmp_read(&fdset);
    else
      snmp_timeout();
  }
}

void get_snmp(void *precord)
{
  snmpRecord *psnmp = (snmpRecord *)precord;
  SNMP_INFO *snmpinfo = (SNMP_INFO*)psnmp->dpvt;

  if (!(snmpinfo->sess = snmp_open(&snmpinfo->ss))) {
    snmp_perror("snmp_open");
  };

  snmpinfo->getreq = snmp_pdu_create(SNMP_MSG_GET);	/* send the first GET */
  if (! snmpinfo->getreq) {
    snmp_close(snmpinfo->sess);                 /* cleanup */
  }

  snmp_add_null_var(snmpinfo->getreq, snmpinfo->oid_info.Oid, snmpinfo->oid_info.OidLen);

  if (snmp_send(snmpinfo->sess, snmpinfo->getreq))
    hosts++;
  else {
    snmp_perror("snmp_setsend");
    snmp_free_pdu(snmpinfo->getreq);
  }

  active_hosts();
  snmp_close(snmpinfo->sess);                 /* cleanup */
}

int set_snmp(void *precord, const char *sval)
{
  snmpRecord *psnmp = (snmpRecord *)precord;
  SNMP_INFO *snmpinfo = (SNMP_INFO*)psnmp->dpvt;

  char type = 'i';

  if (!(snmpinfo->sess = snmp_open(&snmpinfo->ss))) {
    snmp_perror("snmp_open");
    return -1;
  };

  snmpinfo->setreq = snmp_pdu_create(SNMP_MSG_SET);	/* send the first GET */
  if (! snmpinfo->setreq) {
    snmp_close(snmpinfo->sess);                 /* cleanup */
    return -1;
  }

  snmp_add_var(snmpinfo->setreq, snmpinfo->oid_info.Oid, snmpinfo->oid_info.OidLen, type, sval);       //snmp_add_var(netsnmp_pdu *, const oid *, size_t, char, const char *)

  if (snmp_send(snmpinfo->sess, snmpinfo->setreq))
    hosts++;
  else {
    snmp_perror("snmp_getsend");
    snmp_free_pdu(snmpinfo->setreq);
    return -1;
  }

  active_hosts();
  snmp_close(snmpinfo->sess);                 /* cleanup */
  return 0;
}


