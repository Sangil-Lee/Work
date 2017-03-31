/*
 * async.c
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

/*
 * a list of variables to query for
 */
struct oid_info {
  char Name[81];
  oid Oid[MAX_OID_LEN];
  unsigned int OidLen;
};

typedef struct oid_info OID;
typedef struct snmp_session snmp_session;
typedef struct snmp_info
{
  OID  oid_info;
  snmp_session *sess;
  snmp_session ss;
  char sval[81];
  struct snmp_pdu *getreq;                           /* startup all hosts */
  struct snmp_pdu *setreq;                           /* startup all hosts */
  const char *authpass;
  const char *privpass;
  int count;
  char msg[24];
} SNMP_INFO;


typedef struct devSNMPPvt {
  ELLNODE           devSNMPNode;
  IOSCANPVT         ioScanPvt;
  dbCommon          *prec;
  epicsMutexId      mutexId;
} devSNMPPvt;


void initialize(void *precord);
void actve_hosts();
/* void snmp_get(void *precord); */
static long snmp_get(void *precord);
void snmp_set(void *precord);

int return_data(int status, struct snmp_session *sp, struct snmp_pdu *pdu);
int asynch_response(int operation, struct snmp_session *sp, int reqid, struct snmp_pdu *pdu, void *magic);

static long snmp_Init(void *precord);
static long snmp_Read(void *precord);
long getIoIntInfo(int cmd, dbCommon *pr, IOSCANPVT *iopvt);
static ELLLIST devSNMPList;
static void devSNMPScanThread(void);

epicsShareFunc int drvSNMPConnInit(const char *version, const char *hostaddr, const char *username, const char *authpass, const char*privpass);

static struct snmpRecord *gpSnmp;
//leesi
static SNMP_INFO *gsnmpInfo = NULL;
static SNMP_INFO *snmpinfo = NULL;
/* static snmp_session gss; */
int active_hosts = 0;			             /* hosts that we have not completed */

typedef struct devSNMPSoft{
  long number;
  DEVSUPFUN report;
  DEVSUPFUN init;
  DEVSUPFUN init_record;
  /* DEVSUPFUN get_ioint_info; */
  long (*get_ioint_info) (int, dbCommon*, IOSCANPVT*);
  DEVSUPFUN Read;
  DEVSUPFUN special_linconv;
}devSNMPSoft;

devSNMPSoft devSNMPRead  = {5, NULL, NULL, snmp_Init, getIoIntInfo, snmp_Read, NULL};
/* devSNMPSoft devSNMPRead  = {5, NULL, NULL, snmp_Init, NULL, snmp_Read}; */
epicsExportAddress(dset, devSNMPRead);

static const iocshArg drvSNMPConnInitArg0 = {"version", iocshArgString};
static const iocshArg drvSNMPConnInitArg1 = {"hostaddr", iocshArgString};
static const iocshArg drvSNMPConnInitArg2 = {"username", iocshArgString};
static const iocshArg drvSNMPConnInitArg3 = {"authpass", iocshArgString};
static const iocshArg drvSNMPConnInitArg4 = {"privpass", iocshArgString};
static const iocshArg *drvSNMPConnInitArgs[] = {&drvSNMPConnInitArg0, &drvSNMPConnInitArg1, &drvSNMPConnInitArg2, &drvSNMPConnInitArg3, &drvSNMPConnInitArg4};

static const iocshFuncDef drvSNMPConnInitFuncDef =
  {"drvSNMPConnInit", 5, drvSNMPConnInitArgs};

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
epicsShareFunc int drvSNMPConnInit(const char *version, const char *hostaddr, const char *username, const char *authpass, const char*privpass)
{
  printf("------------------------------------------------\n");
  printf("degug line\n");
  printf("------------------------------------------------\n");
  printf("version: %s, hostaddr: %s, username: %s, authpass: %s, privpass: %s\n", 
	 version, hostaddr, username, authpass, privpass);
  printf("------------------------------------------------\n");


  ellInit(&devSNMPList);
  epicsThreadCreate("drvSNMPConnInit", epicsThreadPriorityHigh, epicsThreadGetStackSize(epicsThreadStackSmall),
		    (EPICSTHREADFUNC)devSNMPScanThread,NULL);



  /* struct snmpRecord *psnmp; */
  //SNMP_INFO *snmpinfo = (SNMP_INFO *)malloc(sizeof(SNMP_INFO));
  snmpinfo = (SNMP_INFO *)malloc(sizeof(SNMP_INFO));
  snmpinfo->count = 0;
  static int snmpInited = FALSE;
  snmpinfo->oid_info.OidLen = MAX_OID_LEN;

  if (!snmpInited)
    {
      init_snmp("asynchapp");
      snmpInited = TRUE;
    };

  snmp_sess_init(&(snmpinfo->ss));
  snmpinfo->ss.version = strdup(version);
  snmpinfo->ss.peername = strdup(hostaddr);
  snmpinfo->ss.community = strdup(username);
  snmpinfo->ss.community_len = strlen((char*)snmpinfo->ss.community);

  snmpinfo->sess = &snmpinfo->ss;
  snmpinfo->ss.callback = asynch_response;		/* default callback */
  snmpinfo->ss.callback_magic = snmpinfo;



 if (!read_objid("PowerNet-MIB::sPDUOutletCtl.8", snmpinfo->oid_info.Oid, &snmpinfo->oid_info.OidLen))      /* parse the oids */
    {
      printf("parse the oids %s\n","PowerNet-MIB::sPDUOutletCtl.8");
      snmp_perror("read_objid");
      exit(1);
    } 

  if (!(snmpinfo->sess = snmp_open(&snmpinfo->ss))) {
    snmp_perror("snmp_open");
  }


 
  strcpy(&snmpinfo->msg,"Test Message");


  //leesi
  //gsnmpInfo = snmpinfo;
  //memcpy(gsnmpInfo, snmpinfo, sizeof(SNMP_INFO));
  /* memcpy(&gss,&snmpinfo->ss,sizeof(snmp_session)); */

  gsnmpInfo = snmpinfo;

  printf("Pointer-1(%p), SessionPointer(%p)\n",snmpinfo,&snmpinfo->ss);
  printf("version: %s(%d), ip: %s, name: %s(%d) \n", 
	 snmpinfo->ss.version,strlen(snmpinfo->ss.version), snmpinfo->ss.peername, snmpinfo->ss.community, snmpinfo->ss.community_len); 
  snmpinfo->count++;
  printf("------------------------------------------------\n");
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



/*
  static long snmp_Init(void *precord)
  {
  snmpRecord *psnmp = (snmpRecord *)precord;
  }
*/

static long snmp_Init(void *precord)
{            
  snmpRecord *psnmp = (snmpRecord *)precord;
  devSNMPSoft *pdset = (devSNMPSoft*)psnmp->dset;
  //leesi
  psnmp->dpvt = (SNMP_INFO *)snmpinfo;
  printf("snmpInit()******version: %s\n",snmpinfo->ss.version);
  printf("snmpInit()******Message: %s\n",snmpinfo->msg);
  printf("Count(%d),snmpInit()******version: %s, ip: %s, name: %s\n",snmpinfo->count, snmpinfo->ss.version,snmpinfo->ss.peername, snmpinfo->ss.community);
  //printf("snmpInit()******version: %s, ip: %s, name: %s\n",gss.version,gss.peername, gss.securityName);

  /* initialize(psnmp); */
  if(recGblInitConstantLink(&psnmp->inp,DBF_DOUBLE,&psnmp->val))
    psnmp->udf = FALSE;

  devSNMPPvt *pdev = (devSNMPPvt *)malloc(sizeof (devSNMPPvt));
  if(pdev == NULL) return -1;

  pdev->mutexId = epicsMutexCreate();
  scanIoInit(&pdev->ioScanPvt);
  psnmp->dpvt = pdev;

  ellAdd(&devSNMPList, &(((devSNMPPvt*)psnmp->dpvt)->devSNMPNode));

  return(0);
}

long getIoIntInfo(int cmd, dbCommon *pr, IOSCANPVT *iopvt)
{
  devSNMPPvt *pdev = (devSNMPPvt*)pr ->dpvt;

  *iopvt = pdev->ioScanPvt;
  return 0;
}


static long snmp_Read(void *precord)
{
  snmpRecord *psnmp = (snmpRecord *)precord;
  devSNMPSoft *pdset = (devSNMPSoft*)psnmp->dset;
  long status;

  printf("SNMP Read\n");
  gpSnmp = psnmp;
  snmp_get(psnmp);

  status = dbGetLink(&(psnmp->inp),DBF_DOUBLE, &(psnmp->val),0,0);
  /*If return was succesful then set undefined false*/
  if(!status) psnmp->udf = FALSE;

  return(0);
}

/* void initialize(void *precord) */
/* { */
/*   snmpRecord *psnmp = (snmpRecord *)precord; */
/*   SNMP_INFO *snmpinfo = (SNMP_INFO*)psnmp->dpvt;           //leesi */

/*  if (!read_objid("PowerNet-MIB::sPDUOutletCtl.8", snmpinfo->oid_info.Oid, &snmpinfo->oid_info.OidLen))      /\* parse the oids *\/ */
/*     { */
/*       printf("parse the oids %s\n","PowerNet-MIB::sPDUOutletCtl.8"); */
/*       snmp_perror("read_objid"); */
/*       exit(1); */
/*     }  */
/* } */




/* simple printing of returned data */
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
	//----------------------------------------------------------------------------
	//mjpark
	getdata = (char *)malloc(1 + vp->val_len);
	memcpy(getdata, vp->val.string, vp->val_len);
	getdata[vp->val_len] = '\0';
	int nVal;
	memcpy((void *)&nVal, getdata, 4);
	gpSnmp->val = nVal;

	//debug print
	printf("snmpVal(%d), psnmp(%p), data(%s)\n", nVal, gpSnmp, getdata); 
	//---------------------------------------------------------------------------
        fprintf(stdout, "%s: %s\n", sp->peername, buf);
	vp = vp->next_variable;
      }
      /* free(getdata); */
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
  active_hosts--;
  return 1;
}


void actve_hosts()
{
  /* loop while any active hosts */
  while (active_hosts) {
    /* printf("Active Host: %d\n", active_hosts); */
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

static long snmp_get(void *precord)
{
  snmpRecord *psnmp = (snmpRecord *)precord;
  SNMP_INFO *snmpinfo = (SNMP_INFO*)psnmp->dpvt;           //leesi

  snmpinfo->oid_info.OidLen = MAX_OID_LEN;

  /* if (!(snmpinfo->sess = snmp_open(&snmpinfo->ss))) { */
  /*   snmp_perror("snmp_open"); */
  /* } */

  printf("version: %s(%d), ip: %s, name: %s(%d)\n", 
	 snmpinfo->ss.version,strlen(snmpinfo->ss.version), snmpinfo->ss.peername, snmpinfo->ss.community, snmpinfo->ss.community_len); 


  snmpinfo->getreq = snmp_pdu_create(SNMP_MSG_GET);	/* send the first GET */
  snmp_add_null_var(snmpinfo->getreq, snmpinfo->oid_info.Oid, snmpinfo->oid_info.OidLen);

  if (snmp_send(snmpinfo->sess, snmpinfo->getreq))
    active_hosts++;
  else {
    snmp_perror("snmp_send");
    snmp_free_pdu(snmpinfo->getreq);
  }

  actve_hosts();

  /* cleanup */
  snmp_close(snmpinfo->sess);
}
