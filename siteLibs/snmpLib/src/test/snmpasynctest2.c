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
 * snmpasynctest2.c
 * Mi Jeong Park, mijoy0909@ibs.re.kr
 * ---------------------------------------------------------
 * log
 * ---------------------------------------------------------
 * Wednesday, December 31 17:36:45 KST 2014 (v2c read only)
 * Monday, January  5 13:10:27 KST 2015 (v2c/3 read only)
 * Friday, January  9 16:45:32 KST 2015 (read only - scanf epics inp field) thanks to SL 
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


/*Version 3-------------------------------------------*/
#define DEMO_USE_SNMP_VERSION_3
#ifdef DEMO_USE_SNMP_VERSION_3
const char *our_v3_passphrase = "qwertyuiop12345";
const char *priv_passphrase = "qwertyuiop12345";
#endif
/*----------------------------------------------------*/


/*
 * a list of variables to query for
 */
struct oid_info {
  //const char *Name;
  char Name[81];
  oid Oid[MAX_OID_LEN];
  unsigned int OidLen;
};

typedef struct oid_info OID;
typedef struct snmp_session snmp_session;

typedef struct snmp_info
{
  OID  oid;
  char ip[81];
  char community[81];
  snmp_session *sess;
} SNMP_INFO;


struct session {
  struct snmp_session *sess;	/* SNMP session data */
  //struct oid *snmpinfo;		/* How far in our poll are we */
  OID *current_oid;				/* How far in our poll are we */
} sessions;
//} sessions[sizeof(SNMP_INFO)/sizeof(SNMP_INFO[0])];

int active_hosts;			/* hosts that we have not completed */

//void asynchronous(SNMP_INFO *snmpinfo, struct link *pLink);
//void asynchronous(struct link *pLink);
void asynchronous(struct snmpRecord *psnmp);

int return_data(int status, struct snmp_session *sp, struct snmp_pdu *pdu);
int asynch_response(int operation, struct snmp_session *sp, int reqid, struct snmp_pdu *pdu, void *magic);

static long snmp_Init(struct snmpRecord *psnmp);
static long snmp_Read(struct snmpRecord *psnmp);

struct {
  long number;
  DEVSUPFUN report;
  DEVSUPFUN init;
  DEVSUPFUN init_record;
  DEVSUPFUN get_ioint_info;
  DEVSUPFUN read_ai;
  DEVSUPFUN special_linconv;
}devSNMPSoft={
  6,
  NULL,
  NULL,
  snmp_Init,
  NULL,
  snmp_Read,
  NULL
};
epicsExportAddress(dset, devSNMPSoft);

static struct snmpRecord *gpSnmp;

static void myCallback(CALLBACK *pcallback)
{ 
  snmpRecord *precord; 
  struct rset *prset; 

  callbackGetUser(precord, pcallback); 
  prset = (struct rset *)(precord->rset); 
  dbScanLock(precord); 
  (*prset->process)(precord); 
  dbScanUnlock(precord); 
} 

static long snmp_Init(struct snmpRecord *psnmp)
{            
  CALLBACK *pcallback;

  //leesi
  SNMP_INFO *snmpinfo = (SNMP_INFO *)malloc(sizeof(SNMP_INFO));
  psnmp->dpvt = (SNMP_INFO*)snmpinfo;

  switch (psnmp->inp.type) {
  case INST_IO:
    pcallback = (CALLBACK *)(calloc(1,sizeof(CALLBACK)));
    callbackSetCallback(myCallback, pcallback);
    callbackSetUser(psnmp, pcallback);
    psnmp->dpvt = (void *)pcallback;
    break;

  default :
    recGblRecordError(S_db_badField, (void *)psnmp,
		      "devSNMPSoft (init_record) Illegal INP field");
    return(S_db_badField);
  }

  psnmp->udf = FALSE;
  return 0;

}

static long snmp_Read(struct snmpRecord *psnmp)
{
  long status;
  /* CALLBACK *pcallback = (CALLBACK *)psnmp->dpvt; */

  if(psnmp->pact) {
    psnmp->val = 0; 
    psnmp -> udf = FALSE;
    return 2;
  }

  psnmp->pact = TRUE;
  gpSnmp = psnmp;

  //asynchronous(&psnmp->inp);
  asynchronous(psnmp);

  psnmp->pact = FALSE;

  /*   status = dbGetLink(&(psnmp->inp),DBF_DOUBLE, &(psnmp->val),0,0); */
  /*   if(!status) psnmp->udf = FALSE; */
  /*   return 0; */
  /* } */

  //status = recGblSetSevr(psnmp, READ_ALARM, INVALID_ALARM);
  //printf("Status(%d)\n", status);
  //if (status && (psnmp->stat ==READ_ALARM || psnmp->sevr == INVALID_ALARM))
  //recGblRecordError(-1, (void *)psnmp, "devSNMPSoft read error");

  return(status);
}


/*
 * simple printing of returned data
 */
int return_data(int status, struct snmp_session *sp, struct snmp_pdu *pdu)
{
  char buf[1024];

  struct variable_list *vp;
  int ix; 
  char *data;

  switch (status) {
  case STAT_SUCCESS:
    vp = pdu->variables;
    if (pdu->errstat == SNMP_ERR_NOERROR) {
      while (vp) {
	snprint_variable(buf, sizeof(buf), vp->name, vp->name_length, vp);

	/*mjpark------------------------------------------*/
	data = (char *)malloc(1 + vp->val_len);
	memcpy(data, vp->val.string, vp->val_len);
	data[vp->val_len] = '\0';

	int nVal;
	memcpy((void *)&nVal, data, 4);

	//debug print
	printf("snmpVal(%d), psnmp(%p)\n", nVal, gpSnmp);
	gpSnmp->val = nVal;

	/*------------------------------------------------*/
	//fprintf(stdout, "%s\n", buf);
	vp = vp->next_variable;
      }
      free(data);
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

/*
 * response handler
 */
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
  active_hosts--;
  return 1;
}

//void asynchronous(struct link *pLink)
//void asynchronous(SNMP_INFO *snmpinfo, struct link *pLink)
void asynchronous(struct snmpRecord *psnmp)
{
  struct session *hs;
  int n=0;

  static int snmpInited = FALSE;
  /* struct oid *op = oids; */

  //leesi
  SNMP_INFO *snmpinfo = (SNMP_INFO*)psnmp->dpvt;
  struct link *pLink = (struct link*)&psnmp->inp;

  n = sscanf(pLink->value.instio.string, "%s %s %s", snmpinfo->ip, snmpinfo->community, snmpinfo->oid.Name);

  /* startup all hosts */
  struct snmp_pdu *req;
  struct snmp_session sess;

  hs = &sessions;

  if (!snmpInited)
    {
      init_snmp("asynchapp");
      add_mibdir("/home/mijoy0909/snmp_local/mibs");
      init_mib();
      snmpInited = TRUE;
    };

  //printf("%s\n", pLink->value.instio.string);
  //++leesi
  //n = sscanf(pLink->value.instio.string, "%s %s %s", snmpinfo->ip, snmpinfo->community, Name);
  //printf("IP:%s, Community:%s, OIDName:%s\n", snmpinfo->ip, snmpinfo->community, snmpinfo->oid.Name);


  /*Version 3------------------------------------------------------------------------*/ 
  snmp_sess_init(&sess);
  sess.version = SNMP_VERSION_3;
  sess.peername = strdup(snmpinfo->ip);
  sess.securityName =  strdup(snmpinfo->community);
  sess.securityNameLen = strlen((char*)sess.securityName);
  sess.securityLevel = SNMP_SEC_LEVEL_AUTHPRIV;

  /* Authentication Protocol */
  /* MD5 */
  sess.securityAuthProto = usmHMACMD5AuthProtocol;
  sess.securityAuthProtoLen = sizeof(usmHMACMD5AuthProtocol)/sizeof(oid);
  sess.securityAuthKeyLen = USM_AUTH_KU_LEN;

  /* Authentication keys generation */
  if (generate_Ku(sess.securityAuthProto,
		  sess.securityAuthProtoLen,
		  (u_char *) our_v3_passphrase, strlen(our_v3_passphrase),
		  sess.securityAuthKey,
		  &sess.securityAuthKeyLen) != SNMPERR_SUCCESS) {
    snmp_perror("Error!!!!!!!!!!!!");
    snmp_log(LOG_ERR,
	     "Error generating Ku from authentication pass phrase. \n");
    exit(1);
  }

  /* Privacy protocol */
  /* DES */
  sess.securityPrivProto = usmDESPrivProtocol;
  sess.securityPrivProtoLen = USM_PRIV_PROTO_DES_LEN;
  sess.securityPrivKeyLen = USM_PRIV_KU_LEN;

  /* Privacy keys generation */
  if (generate_Ku(sess.securityAuthProto,
		  sess.securityAuthProtoLen,
		  (u_char *) priv_passphrase, strlen(priv_passphrase),
		  sess.securityPrivKey,
		  &sess.securityPrivKeyLen) != SNMPERR_SUCCESS) {
    printf ("Error generating Ku from privacity pass phrase. \n");
    exit(1);
  }

  sess.callback = asynch_response;		/* default callback */
  sess.callback_magic = hs;

  //snmpinfo->oid.Name = strdup(Name);
  snmpinfo->oid.OidLen = MAX_OID_LEN;
  /*---------------------------------------------------------------------------------*/
  /*Version 2c-----------------------------------------------------------------------*/ 
  /* snmp_sess_init(&sess);			/\* initialize session *\/ */
  /* sess.version = SNMP_VERSION_2c; */
  /* sess.peername = strdup(snmpinfo->ip); */
  /* sess.community = (unsigned char*)strdup(snmpinfo->community); */
  /* sess.community_len = strlen((char*)sess.community); */
  /* sess.callback = asynch_response;		/\* default callback *\/ */
  /* sess.callback_magic = hs; */
  /*---------------------------------------------------------------------------------*/

  /* struct oid *Name = Name; */

  /* int             read_objid(const char *, oid *, size_t *); */

  /* parse the oids */
  /* while (snmpinfo->oid.Name) { */
  /*   snmpinfo->oid.OidLen = sizeof(snmpinfo->oid.Oid)/sizeof(snmpinfo->oid.Oid[0]); */
  /*   /\* printf("parse     %s\n",snmpinfo->oid.Name); *\/ */

  //leesi
  //printf("IP:(%s), Community:(%s), OIDName:(%s)\n", snmpinfo->ip, snmpinfo->community, snmpinfo->oid.Name);
  if (!read_objid(snmpinfo->oid.Name, snmpinfo->oid.Oid, (size_t*)&(snmpinfo->oid.OidLen))) 
    {
      printf("parse     %s\n",snmpinfo->oid.Name);
      snmp_perror("read_objid");
      exit(1);
    }
      
  //printf("parse     %s\n",snmpinfo->oid.Name);


  /*   snmpinfo++; */
  /* } */

  /* parse the oids */
  /* while (op->Name) { */
  /*   op->OidLen = sizeof(op->Oid)/sizeof(op->Oid[0]); */
  /* if (!read_objid(snmpinfo->oid.Name, snmpinfo->oid.Oid, &snmpinfo->oid.OidLen)) { */
  /* 	snmp_perror("read_objid"); */
  /* 	exit(1); */
  /* } */
  /*   op++; */
  /* } */

  if (!(hs->sess = snmp_open(&sess))) {
    snmp_perror("snmp_open");
    /* continue; */
  };

  hs->current_oid = &snmpinfo->oid;

  req = snmp_pdu_create(SNMP_MSG_GET);	/* send the first GET */

  snmp_add_null_var(req, snmpinfo->oid.Oid, snmpinfo->oid.OidLen);
  //snmp_add_null_var(req, hs->current_oid->Oid, hs->current_oid->Oid.OidLen);

  if (snmp_send(hs->sess, req))
    active_hosts++;
  else {
    snmp_perror("snmp_send");
    snmp_free_pdu(req);
  }

  if (n != 3)
    {
      printf("devSNMPSoft (init_record) Bad INP field value");
    }


  /* loop while any active hosts */
  while (active_hosts) {
    printf("Active Host: %d\n", active_hosts);
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


  if (hs->sess) snmp_close(hs->sess);
  return;

  /* cleanup */

#if 0
  /* for (hp = hosts, hs = sessions; hp->community; hs++, hp++) { */
  //++leesi
  //for (snmpinfo, hs = sessions; snmpinfo->community; hs++, snmpinfo++) {
  for (hs = &sessions; snmpinfo->community; hs++, snmpinfo++) {
    if (snmpinfo->sess) snmp_close(snmpinfo->sess);
  }
#endif
}

