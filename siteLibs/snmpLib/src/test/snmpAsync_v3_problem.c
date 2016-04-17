/*
 * snmpAsync_v3.c
 * Mi Jeong Park, mijoy0909@ibs.re.kr
 * --------------------------------------------------------------------------------------------
 * NET-SNMP demo
 * This program demonstrates different ways to query a list of hosts
 * for a list of variables.
 * It would of course be faster just to send one query for all variables,
 * but the intention is to demonstrate the difference between synchronous
 * and asynchronous operation.
 * Niels Baggesen (Niels.Baggesen@uni-c.dk), 1999.
 * --------------------------------------------------------------------------------------------
 * log
 * --------------------------------------------------------------------------------------------
 * Wednesday, December 31 17:36:45 KST 2014 (v2c read only)
 * Monday, January  5 13:10:27 KST 2015 (v2c/3 read only)
 * Friday, January  9 16:45:32 KST 2015 (read only - sscanf -> snmprecord inp field) thanks to SL 
 * Monday, January 12 10:36:21 KST 2015 (read only - sscanf -> snmprecord host,comm,oids field)
 * Tuesday, January 13 17:51:42 KST 2015 (read only - snmprecord host,comm,oids field)
 * Thursday, January 15 15:49:37 KST 2015 modify something
 * Friday, January 16 16:53:23 KST 2015 snmpv3 read/write done! 
 * Monday, January 26 16:57:46 KST 2015 initialize
 * Thursday, February 12 17:23:36 KST 2015 psnmp->dpvt problem
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
#include "shareLib.h"
#include "iocsh.h"
#include "cantProceed.h"
#include "epicsStdio.h"
#include "epicsMutex.h"
#include "epicsEvent.h"
#include "epicsThread.h"
#include "callback.h"
#include "dbCommon.h"
#include "dbScan.h"

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

#define DEMO_USE_SNMP_VERSION_3

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
  char sval[6];
  char msg[24];
  char version[100];
  char hostaddr[100];
  char username[100];
  char authpass[100];
  char privpass[100];
} SNMP_INFO;


typedef struct devSNMPPvt {
  ELLNODE           devSNMPNode;
  IOSCANPVT         ioScanPvt;
  dbCommon          *prec;
  epicsMutexId      mutexId;
} devSNMPPvt;

void initialize(void *precord);
void snmp_get(void *precord);
void snmp_set(void *precord);
int return_data(int status, struct snmp_session *sp, struct snmp_pdu *pdu);
int asynch_response(int operation, struct snmp_session *sp, int reqid, struct snmp_pdu *pdu, void *magic);
long snmp_Init(void *precord);
long snmp_Read(void *precord);
long getIoIntInfo(int cmd, dbCommon *pr, IOSCANPVT *iopvt);
void active_hosts();


/* static long snmp_Write(struct snmpRecord *psnmp); */

/* epicsShareFunc int drvSNMPConnInit(const char *version, const char *hostaddr, const char *username, const char *authpass, const char*privpass); */
epicsShareFunc int drvSNMPConnInit(char *version, char *hostaddr, char *username, char *authpass, char*privpass);
static void devSNMPScanThread(void);

static struct snmpRecord *gpSnmp;
static SNMP_INFO *snmpinfo = NULL;
static SNMP_INFO *gsnmpInfo = NULL;      //leesi
static ELLLIST devSNMPList;
int hosts = 0;	/* hosts that we have not completed */



//drvSNMPConnINit IOC Shell command registration
static const iocshArg drvSNMPConnInitArg0 = {"version", iocshArgString};
static const iocshArg drvSNMPConnInitArg1 = {"hostaddr", iocshArgString};
static const iocshArg drvSNMPConnInitArg2 = {"username", iocshArgString};
static const iocshArg drvSNMPConnInitArg3 = {"authpass", iocshArgString};
static const iocshArg drvSNMPConnInitArg4 = {"privpass", iocshArgString};
static const iocshArg *drvSNMPConnInitArgs[] = {&drvSNMPConnInitArg0, &drvSNMPConnInitArg1, &drvSNMPConnInitArg2, &drvSNMPConnInitArg3, &drvSNMPConnInitArg4};
static const iocshFuncDef drvSNMPConnInitFuncDef = {"drvSNMPConnInit", 5, drvSNMPConnInitArgs};

static void drvSNMPConnInitCallFunc(const iocshArgBuf *args)
{
  drvSNMPConnInit(args[0].sval, args[1].sval, args[2].sval, args[3].sval, args[4].sval );
};

static void drvSNMPConnInitRegisterCommands(void)
{
  static int firstTime = 1;
  if(firstTime)
    {
      iocshRegister(&drvSNMPConnInitFuncDef, drvSNMPConnInitCallFunc);
      firstTime = 0;
    };
}
epicsExportRegistrar(drvSNMPConnInitRegisterCommands);

//drvSNMPConnInit

/* epicsShareFunc int drvSNMPConnInit(const char *version, const char *hostaddr, const char *username, const char *authpass, const char*privpass) */
epicsShareFunc int drvSNMPConnInit(char *version, char *hostaddr, char *username, char *authpass, char*privpass)
{
  printf("------------------------------------------------\n");
  printf("degug line\n");
  printf("------------------------------------------------\n");
  printf("version: %s, hostaddr: %s, username: %s, authpass: %s, privpass: %s\n", 
	 version, hostaddr, username, authpass, privpass);
  printf("------------------------------------------------\n");

  
  snmpinfo = (SNMP_INFO *)malloc(sizeof(SNMP_INFO));

  strcpy(&snmpinfo->version, version);
  strcpy(&snmpinfo->hostaddr, hostaddr);
  strcpy(&snmpinfo->username, username);
  strcpy(&snmpinfo->authpass, authpass);
  strcpy(&snmpinfo->privpass, privpass);
  strcpy(&snmpinfo->msg,"Test Message");

  gsnmpInfo = snmpinfo;
  memcpy(gsnmpInfo, snmpinfo, sizeof(SNMP_INFO));

  printf("version: %s, hostaddr: %s, username: %s, authpass: %s, privpass: %s\n",
  	 snmpinfo->version, snmpinfo->hostaddr, snmpinfo->username, snmpinfo->authpass, snmpinfo->privpass);

  ellInit(&devSNMPList);
  epicsThreadCreate("drvSNMPConnInit", epicsThreadPriorityHigh, epicsThreadGetStackSize(epicsThreadStackSmall),
		    (EPICSTHREADFUNC)devSNMPScanThread,NULL);

  return 0;
};

static void devSNMPScanThread(void)
{
  ELLLIST *pdevSNMPList = &devSNMPList;
  devSNMPPvt *pSNMPPvt;

  while(!pdevSNMPList->count) {
    epicsThreadSleep(0.5);
  }

  while(TRUE)
    {
      pSNMPPvt = (devSNMPPvt*) ellFirst(pdevSNMPList);
      do {
	epicsMutexLock(pSNMPPvt->mutexId);
	//critical section
	epicsMutexUnlock(pSNMPPvt->mutexId);
	scanIoRequest(pSNMPPvt->ioScanPvt);
      } while( (pSNMPPvt = (devSNMPPvt*) ellNext(&pSNMPPvt->devSNMPNode)) );
      epicsThreadSleep(0.1);
    }
}

int SNMPDebug = 0;
epicsExportAddress(int, SNMPDebug);

typedef struct devSNMPSoft{
  long number;
  DEVSUPFUN report;
  DEVSUPFUN init;
  DEVSUPFUN init_record;
  /* DEVSUPFUN get_ioint_info; */
  long (*get_ioint_info) (int, dbCommon*, IOSCANPVT*);
  DEVSUPFUN Read;
}devSNMPSoft;

devSNMPSoft devSNMPRead  = {5, NULL, NULL, snmp_Init, getIoIntInfo, snmp_Read};
//devSNMPSoft devSNMPWrite = {6, NULL, NULL, snmp_Init, NULL, snmp_Write};
/* devSNMPSoft devSNMPWrite = {5, NULL, NULL, snmp_Init, NULL, snmp_Write}; */

epicsExportAddress(dset, devSNMPRead);
/* epicsExportAddress(dset, devSNMPWrite); */

long getIoIntInfo(int cmd, dbCommon *pr, IOSCANPVT *iopvt)
{
  devSNMPPvt *pdev = (devSNMPPvt*)pr ->dpvt;
  *iopvt = pdev->ioScanPvt;
  return 0;
}

long snmp_Init(void *precord)
{            
  snmpRecord *psnmp = (snmpRecord *)precord;
  /* psnmp->dpvt = (SNMP_INFO*)gsnmpInfo; */

  printf("snmpInit()****** Message: %s, version: %s, ip: %s, name: %s, authpass: %s, privpass: %s\n",snmpinfo->msg, snmpinfo->version, snmpinfo->hostaddr, snmpinfo->username, snmpinfo->authpass, snmpinfo->privpass);

  if(recGblInitConstantLink(&psnmp->inp,DBF_DOUBLE,&psnmp->val))
    psnmp->udf = FALSE;


  devSNMPPvt *pdev = (devSNMPPvt *)malloc(sizeof (devSNMPPvt));
  if(pdev == NULL) return -1;

  pdev->mutexId = epicsMutexCreate();
  scanIoInit(&pdev->ioScanPvt);
  psnmp->dpvt = pdev;
  pdev->prec = (dbCommon*)precord;

  ellAdd(&devSNMPList, &(((devSNMPPvt*)psnmp->dpvt)->devSNMPNode));

  initialize(psnmp);
  return(0);
}

long snmp_Read(void *precord)
{
  snmpRecord *psnmp = (snmpRecord *)precord;
  long status;

  /* printf("SNMP Read\n"); */
  /* gpSnmp = psnmp; */
  /* snmp_get(psnmp); */

  status = dbGetLink(&(psnmp->inp),DBF_DOUBLE, &(psnmp->val),0,0);
  /*If return was succesful then set undefined false*/

  if(!status) psnmp->udf = FALSE;

  printf("SNMP Read\n");
  gpSnmp = psnmp;
  snmp_get(psnmp);

  return(0);
}

void initialize(void *precord)
{
  snmpRecord *psnmp = (snmpRecord *)precord;
  /* SNMP_INFO *gsnmpInfo = (SNMP_INFO*)psnmp->dpvt;           //leesi */
  static int snmpInited = FALSE;
  snmpinfo->oid_info.OidLen = MAX_OID_LEN;

  if (!snmpInited)
    {init_snmp("asyncapp");                               /* initialize library */
      snmpInited = TRUE;
    };

#ifdef DEMO_USE_SNMP_VERSION_3             //version 3
  snmp_sess_init(&(snmpinfo->ss));
  snmpinfo->ss.version = snmpinfo->version;
  snmpinfo->ss.peername = strdup(snmpinfo->hostaddr);
  snmpinfo->ss.securityName = (unsigned char*)strdup(snmpinfo->username);
  snmpinfo->ss.securityNameLen = strlen((char*)snmpinfo->ss.securityName);
  snmpinfo->ss.securityLevel = SNMP_SEC_LEVEL_AUTHPRIV;
  /* Authentication Protocol */
  /* MD5 */
  snmpinfo->ss.securityAuthProto = usmHMACMD5AuthProtocol;
  snmpinfo->ss.securityAuthProtoLen = sizeof(usmHMACMD5AuthProtocol)/sizeof(oid);
  snmpinfo->ss.securityAuthKeyLen = USM_AUTH_KU_LEN;

  /* Authentication keys generation */
  if (generate_Ku(snmpinfo->ss.securityAuthProto,
		  snmpinfo->ss.securityAuthProtoLen,
		  (u_char *) snmpinfo->authpass, strlen(snmpinfo->authpass),
		  snmpinfo->ss.securityAuthKey,
		  &snmpinfo->ss.securityAuthKeyLen) != SNMPERR_SUCCESS) {
    snmp_perror("Authentication Keys generation Error ");
    snmp_log(LOG_ERR, "Error generating Ku from authentication pass phrase. \n");
    exit(1);
  }

  /* Privacy protocol */
  /* DES */
  snmpinfo->ss.securityPrivProto = usmDESPrivProtocol;
  snmpinfo->ss.securityPrivProtoLen = USM_PRIV_PROTO_DES_LEN;
  snmpinfo->ss.securityPrivKeyLen = USM_PRIV_KU_LEN;

  /* Privacy keys generation */
  if (generate_Ku(snmpinfo->ss.securityAuthProto,
		  snmpinfo->ss.securityAuthProtoLen,
		  (u_char *) snmpinfo->privpass, strlen(snmpinfo->privpass),
		  snmpinfo->ss.securityPrivKey,
		  &snmpinfo->ss.securityPrivKeyLen) != SNMPERR_SUCCESS) {
    snmp_perror("Privacy Keys generation Error ");
    snmp_log(LOG_ERR, "Error generating Ku from privacity pass phrase. \n");
    exit(1);
  }
  snmpinfo->ss.callback = asynch_response;		/* default callback */
  snmpinfo->ss.callback_magic = snmpinfo;
#else //version 2c
  snmp_sess_init(&(snmpinfo->ss));			/* initialize session */
  snmpinfo->ss.version = snmpinfo->version;
  snmpinfo->ss.peername = strdup(snmpinfo->hostaddr);
  snmpinfo->ss.community = (unsigned char*)strdup(snmpinfo->username);
  snmpinfo->ss.community_len = strlen((char*)snmpinfo->ss.community);
  snmpinfo->ss.callback = asynch_response;		/* default callback */
  snmpinfo->ss.callback_magic = snmpinfo;
#endif

  if (!read_objid(psnmp->oids, snmpinfo->oid_info.Oid, &snmpinfo->oid_info.OidLen))      /* parse the oids */
    {
      printf("parse the oids %s\n",psnmp->oids);
      snmp_perror("read_objid");
      exit(1);
    }

  printf("initialize****** version: %s, ip: %s, name: %s, securityname: %s, authpass: %s, privpass: %s, secuauth: %s, secupriv: %s\n",snmpinfo->ss.version, snmpinfo->ss.peername, snmpinfo->username, snmpinfo->ss.securityName, snmpinfo->authpass, snmpinfo->privpass, snmpinfo->ss.securityAuthKey, snmpinfo->ss.securityPrivKey);
}

int return_data(int status, struct snmp_session *sp, struct snmp_pdu *pdu)
{
  char buf[1024];
  struct variable_list *vp;
  int ix;
  char *getdata = NULL;
 
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
#if 0
	//Data over flow...
	int nVal;
	memcpy((void *)&nVal, getdata, 4);
#else
	unsigned long int nVal;
	memcpy((void *)&nVal, getdata, sizeof(unsigned long int));
#endif
	gpSnmp->val = nVal;

	//debug print
	/* printf("data length(%d), unsigned long int size(%d), int(%d)\n",vp->val_len, sizeof(unsigned long int), sizeof(int)); */
	printf("snmpVal(%d), psnmp(%p)\n", nVal, gpSnmp);
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

#if 1
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

void snmp_get(void *precord)
{
  snmpRecord *psnmp = (snmpRecord *)precord;
  /* SNMP_INFO *gsnmpInfo = (SNMP_INFO*)psnmp->dpvt;       */

  printf("snmpget()****** Message:%s, version: %s, ip: %s, name: %s, securityname: %s, authpass: %s, privpass: %s, secuauth: %s, secupriv: %s\n", snmpinfo->msg, snmpinfo->ss.version, snmpinfo->ss.peername, snmpinfo->username, snmpinfo->ss.securityName, snmpinfo->authpass, snmpinfo->privpass, snmpinfo->ss.securityAuthKey, snmpinfo->ss.securityPrivKey);

  printf("oids: %s\n", psnmp->oids);

  if (!(snmpinfo->sess = snmp_open(&snmpinfo->ss))) {
    snmp_perror("snmp_open");
  };

  snmpinfo->getreq = snmp_pdu_create(SNMP_MSG_GET);	/* send the first GET */
  if (! snmpinfo->getreq) {
    snmp_close(snmpinfo->sess);                 /* cleanup */
  }

  snmp_add_null_var(snmpinfo->getreq, snmpinfo->oid_info.Oid, snmpinfo->oid_info.OidLen);

/* int snmp_async_send(netsnmp_session *, netsnmp_pdu *, netsnmp_callback, void *) */
/* int snmp_send(netsnmp_session *, netsnmp_pdu *) */


  /* if (snmp_async_send(snmpinfo->sess, snmpinfo->getreq, asynch_response, NULL))   */
  /*   { */
  /*     hosts++; */
  /*   } else { */
  /*   snmp_perror("snmp_get->async_Send Error "); */
  /*   snmp_free_pdu(snmpinfo->getreq); */
  /* } */


  if (snmp_send(snmpinfo->sess, snmpinfo->getreq))
    {
      hosts++;
    } else {
    snmp_perror("snmp_get->Send Error ");
    snmp_free_pdu(snmpinfo->getreq);
  }



  active_hosts();
  snmp_close(snmpinfo->sess);                 /* cleanup */
}
