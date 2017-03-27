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
 * snmpasynctestv1.c
 * Mi Jeong Park, mijoy0909@ibs.re.kr
 * ---------------------------------------------------------
 * log
 * ---------------------------------------------------------
 * Wednesday, December 31 17:36:45 KST 2014 (v2c read only)
 * Monday, January  5 13:10:27 KST 2015 (v2c/3 read only)
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
 * a list of hosts to query
 */
struct host {
  const char *ip;
  const char *name;
} hosts[] = {
  { "10.1.5.142", "mijoy" },
  { NULL }
};

/*
 * a list of variables to query for
 */
struct oid {
  const char *Name;
  oid Oid[MAX_OID_LEN];
  int OidLen;
} oids[] = {
  { "PowerNet-MIB::sPDUOutletCtl.8" },
  { NULL }
};

void initialize ();
void asynchronous();
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
  switch (psnmp->inp.type) {
  case (CONSTANT) :
    pcallback = (CALLBACK *)(malloc(1,sizeof(CALLBACK)));
    callbackSetCallback(myCallback, pcallback);
    callbackSetUser(psnmp, pcallback);
    psnmp->dpvt = (void *)pcallback;
    break;

  default :
    recGblRecordError(S_db_badField, (void *)psnmp,
		      "devSNMPSoft (init_record) Illegal INP field");
    return(S_db_badField);
  }

  if(recGblInitConstantLink(&psnmp->inp,DBF_DOUBLE,&psnmp->val))
    psnmp->udf = FALSE;

  initialize();
  return 0;
}

static long snmp_Read(struct snmpRecord *psnmp)
{
  long status;
  CALLBACK *pcallback = (CALLBACK *)psnmp->dpvt;

  if(psnmp->pact) {
    psnmp->val = 0; 
    psnmp -> udf = FALSE;
    return 2;
  }

  psnmp->pact = TRUE;

  gpSnmp = psnmp;
  asynchronous();

  psnmp->pact = FALSE;

  status = dbGetLink(&(psnmp->inp),DBF_DOUBLE, &(psnmp->val),0,0);
  /*If return was succesful then set undefined false*/
  if(!status) psnmp->udf = FALSE;
  return 0;
}


/*
 * initialize
 */
void initialize(void)
{
  struct oid *op = oids;
  
  /* initialize library */
  init_snmp("asynchapp");

  /* parse the oids */
  while (op->Name) {
    op->OidLen = sizeof(op->Oid)/sizeof(op->Oid[0]);
    if (!read_objid(op->Name, op->Oid, &op->OidLen)) {
      snmp_perror("read_objid");
      exit(1);
    }
    op++;
  }
}

/*
 * simple printing of returned data
 */
int return_data(int status, struct snmp_session *sp, struct snmp_pdu *pdu)
{
  char buf[1024];
  struct variable_list *vp;
  int ix; 

  switch (status) {
  case STAT_SUCCESS:
    vp = pdu->variables;
    if (pdu->errstat == SNMP_ERR_NOERROR) {
      while (vp) {
	snprint_variable(buf, sizeof(buf), vp->name, vp->name_length, vp);

	/*mjpark------------------------------------------*/
	char *string = (char *)malloc(1 + vp->val_len);
	memcpy(string, vp->val.string, vp->val_len);
	string[vp->val_len] = '\0';

	int nVal;
	memcpy((void *)&nVal, string, 4);

	//debug print
	//printf("snmpVal(%d), psnmp(%p)\n", nVal, gpSnmp);
	gpSnmp->val = nVal;

	/*------------------------------------------------*/
	//fprintf(stdout, "%s\n", buf);
	vp = vp->next_variable;
      }
    }
    else {
      for (ix = 1; vp && ix != pdu->errindex; vp = vp->next_variable, ix++)
	;
      if (vp) snprint_objid(buf, sizeof(buf), vp->name, vp->name_length);
      else strcpy(buf, "(none)");
      fprintf(stdout, "%s: %s: %s\n",
	      sp->peername, buf, snmp_errstring(pdu->errstat));
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
 * poll all hosts in parallel
 */
struct session {
  struct snmp_session *sess;		/* SNMP session data */
  struct oid *current_oid;		/* How far in our poll are we */
} sessions[sizeof(hosts)/sizeof(hosts[0])];

int active_hosts;			/* hosts that we have not completed */

/*
 * response handler
 */
int asynch_response(int operation, struct snmp_session *sp, int reqid, struct snmp_pdu *pdu, void *magic)
{
  struct session *host = (struct session *)magic;
  struct snmp_pdu *req;

  //struct snmpRecord *psnmp;

  if (operation == NETSNMP_CALLBACK_OP_RECEIVED_MESSAGE) {
    if (return_data(STAT_SUCCESS, host->sess, pdu)) {
      host->current_oid++;			/* send next GET (if any) */
      if (host->current_oid->Name) {

	req = snmp_pdu_create(SNMP_MSG_GET);
	snmp_add_null_var(req, host->current_oid->Oid, host->current_oid->OidLen);

	if (snmp_send(host->sess, req))
	  return 1;
	else {
	  snmp_perror("snmp_send");
	  snmp_free_pdu(req);
	}
      }
    }
  }
  else
    return_data(STAT_TIMEOUT, host->sess, pdu);

  /* something went wrong (or end of variables) 
   * this host not active any more
   */
  active_hosts--;
  return 1;
}

void asynchronous(void)
{
  struct session *hs;
  struct host *hp;
  //struct snmpRecord *psnmp;

  /* startup all hosts */
  for (hs = sessions, hp = hosts; hp->name; hs++, hp++) {
    struct snmp_pdu *req;
    struct snmp_session sess;

    /*Version 3------------------------------------------------------------------------*/ 
    snmp_sess_init(&sess);
    sess.version = SNMP_VERSION_3;
    sess.peername = strdup(hp->ip);
    sess.securityName = strdup(hp->name);
    sess.securityNameLen = strlen(sess.securityName);
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
    /*---------------------------------------------------------------------------------*/

    /*Version 2c-----------------------------------------------------------------------*/ 
        /* snmp_sess_init(&sess);			/\* initialize session *\/ */
        /* sess.version = SNMP_VERSION_2c; */
        /* sess.peername = strdup(hp->ip); */
        /* sess.community = strdup(hp->name); */

        /* sess.community_len = strlen(sess.community); */
        /* sess.callback = asynch_response;		/\* default callback *\/ */
        /* sess.callback_magic = hs; */
    /*---------------------------------------------------------------------------------*/

    if (!(hs->sess = snmp_open(&sess))) {
      snmp_perror("snmp_open");
      continue;
    }
    hs->current_oid = oids;

    req = snmp_pdu_create(SNMP_MSG_GET);	/* send the first GET */
    snmp_add_null_var(req, hs->current_oid->Oid, hs->current_oid->OidLen);

    if (snmp_send(hs->sess, req))
      active_hosts++;
    else {
      snmp_perror("snmp_send");
      snmp_free_pdu(req);
    }
  }

  /* loop while any active hosts */

  while (active_hosts) {
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

  /* cleanup */

  for (hp = hosts, hs = sessions; hp->name; hs++, hp++) {
    if (hs->sess) snmp_close(hs->sess);
  }
}





