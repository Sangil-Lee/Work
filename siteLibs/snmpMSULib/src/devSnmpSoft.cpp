/*
  devSnmp.cpp

  EPICS device support for SNMP

  by J.Priller  NSCL/FRIB
*/

#ifdef SHOW_ERROR_999
// undefine items below NOT set given record type val field on read error
#define AI_ON_READ_ERROR_VALUE  -999.999
#define LI_ON_READ_ERROR_VALUE  -999
#define SI_ON_READ_ERROR_VALUE  "INVALID"
// (we do NOT return weird values when output records have error, so that
//  weird values aren't stored and then re-applied by IOC bumpless reboot)
#endif

// needed for PRIxx and SCNxx format specifiers
#define __STDC_FORMAT_MACROS
#include <inttypes.h>
#ifndef SCNu64
  #error SCNu64 not defined - check includes/defs
#endif

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include <devSnmp.h>

#include <alarm.h>
#include <cvtTable.h>
#include <dbDefs.h>
#include <dbAccess.h>
#include <recGbl.h>
#include <recSup.h>
#include <link.h>
#include <macLib.h>
#include <epicsStdlib.h>
#include <epicsExport.h>
#include <epicsExit.h>
#include <initHooks.h>
#include <menuCalc.h>

//comment out.
//#include <snmpRecord.h>
#include <epicsStdioRedirect.h>
#include <snmp.h>



#define epicsOk    (0)
#define epicsError (-1)

#define MANAGER_BAR  "============================================================"
#define HOST_BAR     "========================================"
#define GROUP_BAR    "----------------------------------------"
#define OIDPV_BAR    "----------------------------"

// special flags for devSnmp_pv
#define SPECIAL_FLAG_PACK          0x0001
#define SPECIAL_FLAG_HEXBITS       0x0002
#define SPECIAL_FLAG_REVERSE_BITS  0x0004
typedef struct {
  char flagChar;
  unsigned long flagValue;
} specialFlag;
static specialFlag specialFlagArray[] = {
  { 'p', SPECIAL_FLAG_PACK         },
  { 'h', SPECIAL_FLAG_HEXBITS      },
  { 'r', SPECIAL_FLAG_REVERSE_BITS },
  { 0,   0                         }
};

// global variables
static devSnmp_manager *pManager = NULL;
static bool didEpicsInit = false;
static bool doingEpicsExit = false;

// updated by loop in sendTask, for use by other objects where
// timing that doesn't require much precision
static epicsTimeStamp globalLastTick;

// internal function prototypes
static bool checkInit(void);
static char *dup_string(char *str);
static char *snmpVersionString(long version);
static int snmpSessionCallback(int op, SNMP_SESSION *sp, int reqId, SNMP_PDU *pdu, void *magic);
static char *snmpStrStr(char *str, char *mask);
static int snmpStrCmp(char *s0, char *s1);
static int snmpReadTask(devSnmp_manager *pMgr);
static int snmpSendTask(devSnmp_manager *pMgr);


//--------------------------------------------------------------------
// parameters settable via devSnmpSetParam()
//--------------------------------------------------------------------

// debugging messages level
static int snmpDebugLevel = 0;

// how stale oid data can be before flagged as bad, in msec
// (to guard against manager's readTask crashing or hanging up)
static int snmpDataStaleTimeoutMSec = 20000;

// maximum number of consecutive times snmp_oid_compare() can fail
// before devSnmp_oid signals read error
// (helps with Wiener/ISEGS that sometimes return variable list different
//  from that requested)
static int snmpMaxOidCompFailures = 10;

// maximum poll weight of lowest-weight device in a group to indicate
// a poll of devices in the group should be made
static int snmpMaxTopPollWeight = 20;

// poll weight that flags device should NOT be polled
// (at least not until their weight decreases)
static int snmpDoNotPollWeight = 1000;

// how often to poll Passive records (such as output records)
static int snmpPassivePollMSec = 2000;

// how long to skip readback/record->val compare for output records after
// a setting to the record has been made (prevents value 'flicker')
static int snmpSetSkipReadbackMSec = 4000;

// how old in milliseconds the oldest 'get' transaction queued for a
// host can be before 'set' transactions no longer have priority over it
static int snmpReadStarvationMSec = 1000;

// thread sleep time for read and send task loops
static int snmpThreadSleepMSec = 20;

// retries and timeout for SNMP base session
// (timeout is in microseconds)
static int snmpSessionRetries = 5;
static int snmpSessionTimeout = 10000000;  // = 1 second


typedef struct {
  const char *paramName;
  int *valuePointer;
  void (*changeFunc)(void);
} setParamItem;

static void debugLevelChange(void);
static void sessionRetriesChange(void);
static void sessionTimeoutChange(void);

static setParamItem setParamTable[] = {
  { "DebugLevel",           &snmpDebugLevel,           debugLevelChange     },
  { "DataStaleTimeoutMSec", &snmpDataStaleTimeoutMSec, NULL                 },
  { "MaxOidCompFailures",   &snmpMaxOidCompFailures,   NULL                 },
  { "MaxTopPollWeight",     &snmpMaxTopPollWeight,     NULL                 },
  { "DoNotPollWeight",      &snmpDoNotPollWeight,      NULL                 },
  { "PassivePollMSec",      &snmpPassivePollMSec,      NULL                 },
  { "SetSkipReadbackMSec",  &snmpSetSkipReadbackMSec,  NULL                 },
  { "ReadStarvationMSec",   &snmpReadStarvationMSec,   NULL                 },
  { "ThreadSleepMSec",      &snmpThreadSleepMSec,      NULL                 },
  { "SessionRetries",       &snmpSessionRetries,       sessionRetriesChange },
  { "SessionTimeout",       &snmpSessionTimeout,       sessionTimeoutChange },
  { NULL,                   NULL,                      NULL                 }
};

//--------------------------------------------------------------------

#ifndef VALIDHEX
  #define VALIDHEX(c) (((c >= '0') && (c <= '9')) || ((c >= 'A') && (c <= 'F')) || ((c >= 'a') && (c <= 'f')))
#endif
#ifndef ISSPACE
  #define ISSPACE(c) ((c == ' ') || (c == '\t'))
#endif

//--------------------------------------------------------------------
// internal routines
//--------------------------------------------------------------------
static char *tnow(void)
{
  static char _TimeStr[64];
  snmpTimeObject T;
  T.start(NULL);
  T.toDateTimeString(_TimeStr);
  return(_TimeStr);
}
//----------------------------------------------------------------------
static void copy_string(char *target, int maxsize, char *source)
{
  int len = strlen(source);
  if (len < maxsize)
    strcpy(target,source);
  else {
    strncpy(target,source,maxsize-1);
    target[maxsize-1] = 0;
  }
}
//----------------------------------------------------------------------
static int snmpScanLong(char *str, long *dest, int base = 10)
{
  char *endp;
  long num = strtol(str,&endp,base);
  if (endp == str) return(0);
  (*dest) = num;
  return(1);
}
//--------------------------------------------------------------------
static int differentEnough(double d1, double d2)
{
  if (d1 == d2) return(false);
  if (d1 == 0.0) return( fabs(d2) > 1.0e-9);
  if (d2 == 0.0) return( fabs(d1) > 1.0e-9);
  double pd = fabs((d2 - d1) / d1);
  return( pd > 0.00001 );
}
//----------------------------------------------------------------------
static int local_isBigEndian(void)
{
  static int _bigEndianState = -1;

  if (_bigEndianState >= 0) return( _bigEndianState);
  long X = 1;
  unsigned char *pUC = (unsigned char *) &X;
  _bigEndianState = (pUC[0] == 0) ? 1 : 0;
  return(_bigEndianState);
}
//----------------------------------------------------------------------
static int local_isNegativeZero(double v)
{
  static int chkidx = -1;

  if (v != 0.0) return(false);
  if (chkidx < 0) chkidx = local_isBigEndian() ? 0 : sizeof(double)-1;
  unsigned char *pUC = (unsigned char *) &v;
  return( (pUC[chkidx] & 0x0080) ? true : false );
}
//----------------------------------------------------------------------
static unsigned long specialValueForChar(char c)
{
  for (int ii = 0; specialFlagArray[ii].flagChar != 0; ii++) {
    if (specialFlagArray[ii].flagChar == c) return(specialFlagArray[ii].flagValue);
  }
  return(0);
}
//----------------------------------------------------------------------
static bool snmpParseInOut
     (char           *inOutText,
      configDataOid  *base,
      configDataPV *extra)
{
  memset(base,0,sizeof(configDataOid));
  memset(extra,0,sizeof(configDataPV));
  int n = sscanf(inOutText, "%s %s %s %s %d %s",
                 base->host,
                 base->community,
                 base->oidStr,
                 extra->mask,
                 &extra->data_len,
                 extra->specialStr);
  if (n < 5) {
    printf("devSnmp: error parsing '%s'\n",inOutText);
    printf("         format is 'host community oidname mask maxdatalength [set_format/special]'\n");
    return(false);
  }

  // parse 'special'
  extra->set_type = 's';     // default
  extra->special_flags = 0;  // default
  if (n >= 6) {
    // first char is set type
    char *pc = extra->specialStr;
    extra->set_type = (*pc);
    pc++;
    while ((*pc) != 0) {
      unsigned long mval = specialValueForChar(*pc);
      if (! mval) {
        printf("devSnmp: error parsing '%s'\n",inOutText);
        printf("         unknown special flag '%c''\n",*pc);
        return(false);
      }
      extra->special_flags |= mval;
      pc++;
    }
  }

  // okay if make it this far
  return(true);
}
//----------------------------------------------------------------------
static void snmpAtExit(void *arg)
{
  // set exiting flag
  // and give processes making calls into pManager a little time to complete
  doingEpicsExit = true;
  epicsThreadSleep(0.2);

  if (pManager) {
    if (snmpDebugLevel) printf("devSnmp : deleting manager object\n");
    delete pManager;
    pManager = NULL;
  }
}
//--------------------------------------------------------------------
static void snmpInitHook(initHookState state)
{
  switch (state) {
    case initHookAtEnd:
      if (pManager->start() != epicsOk) {
        printf("devSnmp ERROR: could not start manager object\n");
      }
      break;

    default:
      break;
  }
}
//----------------------------------------------------------------------
static void debugLevelChange(void)
{
  if (snmpDebugLevel < 0) snmpDebugLevel = 0;
  if (snmpDebugLevel == 0)
    printf("devSnmp: debug level is now OFF\n");
  else
    printf("devSnmp: debug level level is now %d\n",snmpDebugLevel);
}
//----------------------------------------------------------------------
static void sessionRetriesChange(void)
{
  if (pManager) pManager->sessionRetriesChange();
}
//----------------------------------------------------------------------
static void sessionTimeoutChange(void)
{
  if (pManager) pManager->sessionTimeoutChange();
}
//----------------------------------------------------------------------
static bool checkInit(void)
{
  if (doingEpicsExit) return(false);

  if (! didEpicsInit) {
    // set last-tick
    epicsTimeGetCurrent(&globalLastTick);

    // register at-exit call
    int stat = epicsAtExit(snmpAtExit,NULL);
    if (stat != epicsOk) {
      printf("devSnmp ERROR: epicsAtExit returned %d\n",stat);
      return(false);
    }
    // also register init hook
    initHookRegister(snmpInitHook);

    didEpicsInit = true;
  }

  if (! pManager) {
    pManager = new devSnmp_manager();
    if (! pManager) return(false);
  }

  return(true);
}
//--------------------------------------------------------------------
static char *dup_string(char *str)
{
  char *retval = new char[strlen(str)+1];
  strcpy(retval,str);
  return(retval);
}
//--------------------------------------------------------------------
static char *snmpVersionString(long version)
{
  switch (version) {
    case SNMP_VERSION_1:  return((char *) "SNMP_VERSION_1");
    case SNMP_VERSION_2c: return((char *) "SNMP_VERSION_2c");
    case SNMP_VERSION_3:  return((char *) "SNMP_VERSION_3");
    default:              return((char *) "unknown");
  }
}
//--------------------------------------------------------------------
static int snmpReadTask(devSnmp_manager *pMgr)
{
  return(pMgr->readTask());
}
//--------------------------------------------------------------------
static int snmpSendTask(devSnmp_manager *pMgr)
{
  return(pMgr->sendTask());
}
//--------------------------------------------------------------------
static int snmpSessionCallback(int op, SNMP_SESSION *sp, int reqId, SNMP_PDU *pdu, void *magic)
{
  // get pointer to devSnmp_session callback is for, ignore if is NULL
  devSnmp_session *pSession = (devSnmp_session *) magic;
  if (! pSession) return(epicsOk);

  // tell session to process itself, return its status
  return( pSession->replyProcessing(op,sp,reqId,pdu) );
}
//--------------------------------------------------------------------
static char *snmpStrStr(char *str, char *mask)
{
  int i, x;
  char *tmpStr = NULL;

  if ((str != NULL) && (mask != NULL)) {

    for (i = 0; *(str + i) != '\0'; i++) {
      if (*(str + i) == *mask) {
        tmpStr = str + i;
        // let '_' in mask match any char
        for (x = 0; (*(str + i) == *(mask + x) || (*(mask + x) == '_'));) {
          ++i; ++x;
          if (*(mask + x) == '\0') {
            return(tmpStr);
          } else
            continue;
        }
      }
    }
  }
  return(NULL);
}
//--------------------------------------------------------------------
static int snmpStrCmp(char *s0, char *s1)
{
  if ((s0 == NULL) || (s1 == NULL)) return(1);
  char *ts0 = s0, *ts1 = s1;
  for (; *ts0 != ':' && *ts0 && *ts1; ts0++, ts1++) {
    if (*ts0 != *ts1)
      return(1);
  }
  return(0);
}
//--------------------------------------------------------------------
static int binListItem_compare(void *p1, void *p2)
{
  readBinListItem *pL1 = (readBinListItem *) p1;
  readBinListItem *pL2 = (readBinListItem *) p2;

  int u1 = pL1->topPollWeight;
  int u2 = pL2->topPollWeight;

  if (u1 > u2)
    return(1);
  else if (u2 > u1)
    return(-1);
  else
    return(0);
}
//--------------------------------------------------------------------
static int list_context_compare(void *p1, void *p2)
{
  snmpPointerList *pL1 = (snmpPointerList *) p1;
  snmpPointerList *pL2 = (snmpPointerList *) p2;

  unsigned long u1 = (unsigned long) pL1->context();
  unsigned long u2 = (unsigned long) pL2->context();

  if (u1 > u2)
    return(-1);
  else if (u2 > u1)
    return(1);
  else
    return(0);
}
//--------------------------------------------------------------------
static int devSnmp_host_compare(void *p1, void *p2)
{
  devSnmp_host *pG1 = (devSnmp_host *) p1;
  devSnmp_host *pG2 = (devSnmp_host *) p2;

  return( strcmp(pG1->hostName(),pG2->hostName()) );
}
//--------------------------------------------------------------------
static int devSnmp_group_compare(void *p1, void *p2)
{
  devSnmp_group *pG1 = (devSnmp_group *) p1;
  devSnmp_group *pG2 = (devSnmp_group *) p2;

  int ret = strcmp(pG1->hostName(),pG2->hostName());
  if (ret != 0) return(ret);
  return( strcmp(pG1->communityName(),pG2->communityName()) );
}
//--------------------------------------------------------------------
static int devSnmp_pv_compare(void *p1, void *p2)
{
  devSnmp_pv *pD1 = (devSnmp_pv *) p1;
  devSnmp_pv *pD2 = (devSnmp_pv *) p2;

  return( strcmp(pD1->recordName(),pD2->recordName()) );
}
//--------------------------------------------------------------------
static int devSnmp_oidobj_compare(void *p1, void *p2)
{
  devSnmp_oid *pD1 = (devSnmp_oid *) p1;
  devSnmp_oid *pD2 = (devSnmp_oid *) p2;

  return( strcmp(pD1->oidName(),pD2->oidName()) );
}
//--------------------------------------------------------------------
static void analyzeMismatch(netsnmp_variable_list *varlist, devSnmp_oid **oidlist)
//
// Analyze mismatch of vars delivered versus vars requested
//
// For debugging issue with Wiener/ISEG crates where list of variables
// returned does not match list of variables requested
//
{
  typedef struct {
    oid *oidPtr;
    int  oidLen;
  } varItem;

  // build arrays for comparison
  int varCount = 0;
  varItem varArray[128];
  varItem oidArray[128];
  netsnmp_variable_list *var = varlist;
  while (var) {
    devSnmp_oid *pOID = oidlist[varCount];
    varArray[varCount].oidPtr = var->name;
    varArray[varCount].oidLen = var->name_length;
    oidArray[varCount].oidPtr = pOID->getOid()->Oid;
    oidArray[varCount].oidLen = pOID->getOid()->OidLen;
    varCount++;
    var = var->next_variable;
  }

  // compare list of vars returned to oids requested
  char misMask[128];
  misMask[0] = 0;
  for (int ii = 0; ii < varCount; ii++) {
    if (snmp_oid_compare(varArray[ii].oidPtr,varArray[ii].oidLen,
                         oidArray[ii].oidPtr,oidArray[ii].oidLen) != 0)
      strcat(misMask,"M");
    else
      strcat(misMask,"-");
  }

  // count oids requested not in vars returned (missing)
  int oidsMissing = 0;
  for (int ii = 0; ii < varCount; ii++) {
    bool found = false;
    for (int jj = 0; jj < varCount; jj++) {
      if (snmp_oid_compare(oidArray[ii].oidPtr,oidArray[ii].oidLen,
                           varArray[jj].oidPtr,varArray[jj].oidLen) == 0) {
        found = true;
        break;
      }
    }
    if (! found) oidsMissing++;
  }

  // count vars returned not in oids requested (extra)
  int oidsExtra = 0;
  for (int ii = 0; ii < varCount; ii++) {
    bool found = false;
    for (int jj = 0; jj < varCount; jj++) {
      if (snmp_oid_compare(varArray[ii].oidPtr,varArray[ii].oidLen,
                           oidArray[jj].oidPtr,oidArray[jj].oidLen) == 0) {
        found = true;
        break;
      }
    }
    if (! found) oidsExtra++;
  }

  printf("%s  %s missing:%d extra:%d\n",tnow(),misMask,oidsMissing,oidsExtra);
}
//--------------------------------------------------------------------
// class snmpPointerList
//--------------------------------------------------------------------
snmpPointerList::snmpPointerList(int initialCount, int extendCount)
{
  delta     = extendCount;
  currSize  = initialCount;
  currCount = 0;
  _context  = NULL;

  pPtrList = new void *[currSize];
  if (pPtrList) {
    for (int ii = 0; ii < currSize; ii++) pPtrList[ii] = NULL;
  }
}
//--------------------------------------------------------------------
snmpPointerList::~snmpPointerList(void)
{
  // we don't know how to call destructors for pointers we're storing
  // so we hope our owner has taken care of clearing us out, we just
  // dispose of the list of pointers we allocated
  if (pPtrList) {
    delete [] pPtrList;
    pPtrList = NULL;
  }
}
//--------------------------------------------------------------------
void *snmpPointerList::context(void)
{
  return(_context);
}
//--------------------------------------------------------------------
void snmpPointerList::setContext(void *newcontext)
{
  _context = newcontext;
}
//--------------------------------------------------------------------
void snmpPointerList::resize(int newSize)
{
  void **newList = new void *[newSize];
  for (int ii = 0; ii < newSize; ii++) {
    newList[ii] = (ii < currCount) ? pPtrList[ii] : NULL;
  }
  currSize = newSize;
  delete [] pPtrList;
  pPtrList = newList;
}
//--------------------------------------------------------------------
void snmpPointerList::append(void *ptr)
{
  if (currCount >= currSize) {
    // dynamic array is full, need to extend it
    resize(currSize + delta);
  }

  // store pointer
  pPtrList[currCount] = ptr;
  currCount++;
}
//--------------------------------------------------------------------
int snmpPointerList::count(void)
{
  return(currCount);
}
//--------------------------------------------------------------------
void **snmpPointerList::rawArray(void)
{
  return(pPtrList);
}
//--------------------------------------------------------------------
void *snmpPointerList::itemAt(int idx)
{
  void *retval = ((idx < 0) || (idx >= currCount)) ? NULL : pPtrList[idx];
  return(retval);
}
//--------------------------------------------------------------------
void *snmpPointerList::removeItemAt(int idx)
{
  void *retval;
  if ((idx < 0) || (idx >= currCount))
    retval = NULL;
  else {
    retval = pPtrList[idx];
    for (int ii = idx; ii < currCount-1; ii++) {
      pPtrList[ii] = pPtrList[ii+1];
    }
    pPtrList[currCount-1] = NULL;
    currCount--;
  }
  return(retval);
}
//--------------------------------------------------------------------
void snmpPointerList::sort(POINTERLIST_COMPARE_PROC proc)
// performs a good old boring shell sort (fairly fast and is easy to code...)
{
  // no compare procedure, don't sort
  if (! proc) return;

  int gap = currCount / 2;
  do {
    for (int ii = 0; ii < (currCount - gap); ii++) {
      int X = ii;
      do {
        if (proc(pPtrList[X],pPtrList[X+gap]) <= 0) break;
        void *tmp = pPtrList[X];
        pPtrList[X] = pPtrList[X+gap];
        pPtrList[X+gap] = tmp;
        X -= gap;
      } while (X >= 0);
    }
    gap = gap / 2;
  } while (gap != 0);
}
//--------------------------------------------------------------------
// class snmpTimeObject
//--------------------------------------------------------------------
snmpTimeObject::snmpTimeObject(void)
{
  clear();  // start out zero-ed
}
//--------------------------------------------------------------------
snmpTimeObject::~snmpTimeObject(void)
{
  // nothing to do here
}
//--------------------------------------------------------------------
void snmpTimeObject::start(epicsTimeStamp *pnow)
{
  if (pnow)
    memcpy(&lastStarted,pnow,sizeof(epicsTimeStamp));
  else
    epicsTimeGetCurrent(&lastStarted);
}
//--------------------------------------------------------------------
bool snmpTimeObject::started(void)
{
  return( (lastStarted.secPastEpoch == 0) ? false : true );
}
//--------------------------------------------------------------------
void snmpTimeObject::clear(void)
{
  lastStarted.secPastEpoch = 0;
  lastStarted.nsec = 0;
}
//--------------------------------------------------------------------
void snmpTimeObject::toDateTimeString(char *str)
{
  struct tm tb;
  unsigned long nsec;

  if (! started()) {
    strcpy(str,"n/a");
  } else {
    epicsTimeToTM(&tb,&nsec,&lastStarted);
    tb.tm_mon++;
    tb.tm_year += 1900;
    sprintf(str,"%04d/%02d/%02d %02d:%02d:%02d",
            tb.tm_year,tb.tm_mon,tb.tm_mday,
            tb.tm_hour,tb.tm_min,tb.tm_sec);
  }
}
//--------------------------------------------------------------------
double snmpTimeObject::elapsedSeconds(epicsTimeStamp *pnow)
{
  return( ((double) elapsedMilliseconds(pnow)) / 1000.0 );
}
//--------------------------------------------------------------------
long snmpTimeObject::elapsedMilliseconds(epicsTimeStamp *pnow)
{
  // we do the math here directly, as calling epicsTimeDiffInSeconds
  // has extra overhead and we aren't worried about roll-over

  epicsTimeStamp now;
  if (! pnow) {
    epicsTimeGetCurrent(&now);
    pnow = &now;
  }

  // calculate seconds diff
  // prevent msec diff from overflowing a 'long'
  long secPart = pnow->secPastEpoch - lastStarted.secPastEpoch;
  if (secPart >  2147480) return(2147480000);
  if (secPart < -2147480) return(-2147480000);

  // calculate nanoseconds diff
  long nsecPart = (pnow->nsec >= lastStarted.nsec) ?
                   pnow->nsec - lastStarted.nsec
                : (0xFFFFFFFFul - lastStarted.nsec) + pnow->nsec;

  return( secPart*1000 + nsecPart/1000000 );
}
//--------------------------------------------------------------------
// class snmpWeightCollection
//--------------------------------------------------------------------
snmpWeightCollection::snmpWeightCollection(void)
{
  mutex    = epicsMutexCreate();
  oidList  = new snmpPointerList();
  binList  = new snmpPointerList();
  binIdx   = 0;
  binCount = 0;
  binArray = NULL;
  inBuild  = false;
  redoBins = true;
}
//----------------------------------------------------------------------
snmpWeightCollection::~snmpWeightCollection(void)
{
  if (binList) {
    deleteBins();
    delete binList;
    binList = NULL;
  }

  if (oidList) {
    delete oidList;
    oidList = NULL;
  }

  if (mutex) {
    epicsMutexDestroy(mutex);
    mutex = 0;
  }
}
//----------------------------------------------------------------------
void snmpWeightCollection::binChange(void)
{
  epicsMutexLock(mutex);

  // flag that we need to re-bin before next transaction
  redoBins = true;

  epicsMutexUnlock(mutex);
}
//----------------------------------------------------------------------
void snmpWeightCollection::addOID(devSnmp_oid *pOid)
{
  epicsMutexLock(mutex);

  // add to oid list
  if (oidList) oidList->append(pOid);

  // flag that we need to re-bin before next transaction
  redoBins = true;

  epicsMutexUnlock(mutex);
}
//----------------------------------------------------------------------
void snmpWeightCollection::deleteBins(void)
// only called inside of transactionBuildStart..End
// mutex will be held
// -OR- is also called in object destructor
{
  if (binList) {
    int count = binList->count();
    for (int ii = count-1; ii >= 0; ii--) {
      readBinListItem *pBin = (readBinListItem *) binList->removeItemAt(ii);
      if (! pBin) continue;
      if (pBin->pList) delete pBin->pList;
      delete pBin;
    }
  }
}
//----------------------------------------------------------------------
void snmpWeightCollection::createBins(void)
// only called inside of transactionBuildStart..End
// mutex will be held
{
  // clear existing bins
  deleteBins();

  // roll through oids
  int oidCount = oidList->count();
  devSnmp_oid **oidArray = (devSnmp_oid **) oidList->rawArray();
  for (int ii = 0; ii < oidCount; ii++) {
    devSnmp_oid *pOid = oidArray[ii];
    if (! pOid) continue;
    // locate bin for this oid
    readBinListItem *pBin = findBin(pOid->getPollMSec());
    if (! pBin) {
      // no matching bin yet, create one
      pBin = new readBinListItem;
      memset(pBin,0,sizeof(readBinListItem));
      pBin->pollMSec = pOid->getPollMSec();
      pBin->pList = new snmpPointerList();
      pBin->tail = 0;
      binList->append(pBin);
    }
    // add oid to bin
    pBin->pList->append(pOid);
  }

  redoBins = false;
}
//----------------------------------------------------------------------
readBinListItem *snmpWeightCollection::findBin(int pollMSec)
// only called inside of transactionBuildStart..End
// mutex will be held
{
  int count = binList->count();
  for (int ii = 0; ii < count; ii++) {
    readBinListItem *pBin = (readBinListItem *) binList->itemAt(ii);
    if (! pBin) continue;
    if (pBin->pollMSec == pollMSec) return(pBin);
  }
  return(NULL);
}
//----------------------------------------------------------------------
devSnmp_oid *snmpWeightCollection::topOID(void)
// only called inside of transactionBuildStart..End
// mutex will be held
{
  if (binIdx < 0) return(NULL);
  return( binArray[binIdx]->rawArray[ binArray[binIdx]->tail ] );
}
//----------------------------------------------------------------------
void snmpWeightCollection::nextOID(void)
// only called inside of transactionBuildStart..End
// mutex will be held
{
  // increment 'tail' for current bin
  binArray[binIdx]->tail++;
  if (binArray[binIdx]->tail >= binArray[binIdx]->count) binArray[binIdx]->tail = 0;

  // update bin's topPollWeight
  binArray[binIdx]->topPollWeight = binArray[binIdx]->rawArray[binArray[binIdx]->tail]->getPollWeight();

  // see if we need to increment binIdx
  bool shiftBin = false;
  if (binArray[binIdx]->tail == binArray[binIdx]->start) {
    // if tail has wrapped around, increment binIdx
    shiftBin = true;
  } else {
    int binAfter = binIdx + 1;
    if (binAfter < binCount) {
      // if next bin's top is more needy than current one, shift
      shiftBin = (binArray[binAfter]->topPollWeight < binArray[binIdx]->topPollWeight);
    }
  }

  // if need to shift binIdx, do so
  if (shiftBin) {
    binIdx++;
    if (binIdx >= binCount) binIdx = -1;  // end of oids
  }
}
//----------------------------------------------------------------------
void snmpWeightCollection::transactionBuildStart(void)
{
  epicsMutexLock(mutex); // will be unlocked in transactionBuildEnd
  inBuild = true;

  // if need to shuffle oids into bins
  // (either first time or after binChange)
  // then do so
  if (redoBins) createBins();

  // set binCount and binArray
  binCount = binList->count();
  binArray = (readBinListItem **) binList->rawArray();

  // roll through each bin:
  //   update count, rawArray fields
  //   make sure 'tail' is valid
  //   set 'start'
  //   update topPollWeight
  for (int ii = 0; ii < binCount; ii++) {
    readBinListItem *pBin = binArray[ii];
    if (! pBin) continue;
    pBin->count = pBin->pList->count();
    pBin->rawArray = (devSnmp_oid **) pBin->pList->rawArray();
    if (pBin->tail >= pBin->count) pBin->tail = 0;
    pBin->start = pBin->tail;
    devSnmp_oid *topOID = pBin->rawArray[pBin->tail];
    pBin->topPollWeight = topOID->getPollWeight();
  }

  // sort bins by lowest-to-greatest topPollWeight
  binList->sort( binListItem_compare );

  // will start drawing from most-needy bin
  binIdx = 0;
}
//----------------------------------------------------------------------
void snmpWeightCollection::transactionBuildEnd(void)
{
  inBuild = false;
  epicsMutexUnlock(mutex);
}
//----------------------------------------------------------------------
// class devSnmp_setting
//--------------------------------------------------------------------
devSnmp_setting::devSnmp_setting(char set_type, char *set_string)
{
  settingType = set_type;
  settingString = dup_string(set_string);
}
//--------------------------------------------------------------------
devSnmp_setting::~devSnmp_setting(void)
{
  if (settingString) {
    delete [] settingString;
    settingString = NULL;
  }
}
//--------------------------------------------------------------------
const char *devSnmp_setting::setString(void)
{
  return((const char *)settingString);
}
//--------------------------------------------------------------------
char devSnmp_setting::setType(void)
{
  return(settingType);
}
//--------------------------------------------------------------------
// class devSnmp_session
//--------------------------------------------------------------------
devSnmp_session::devSnmp_session(devSnmp_manager *pMgr, devSnmp_group *pGroup, bool is_set)
{
  pOurMgr     = pMgr;
  pOurGroup   = pGroup;
  session     = NULL;
  pdu         = NULL;
  is_setting  = is_set;
  completed   = false;
  sent        = false;
  tried_send  = false;
  oidList     = new snmpPointerList();
  timeStarted.start(&globalLastTick);
  timeSent.clear();
}
//--------------------------------------------------------------------
devSnmp_session::~devSnmp_session(void)
{
  close();
  if (oidList) {
    // don't delete OID objects in list
    // they belong to their devSnmp_group object, not us
    delete oidList;
    oidList = NULL;
  }
}
//--------------------------------------------------------------------
SNMP_SESSION *devSnmp_session::getSession(void)
{
  return(session);
}
//--------------------------------------------------------------------
int devSnmp_session::replyProcessing(int op, SNMP_SESSION *sp, int reqId, SNMP_PDU *pdu)
// called from snmpSessionCallback() in read thread
// host session mutex will be held already
{
  // if we're already completed this is a duplicate, disregard it
  if (completed) {
    return(1);  // 1 = we're done with request, don't keep pending
  }

  // inform our group of reply arrival (for timing purposes)
  if (pOurGroup) pOurGroup->sessionGotReply(this);

  // untick manager's request count so its read task knows when to stop reading
  pOurMgr->decActiveRequests();

  int oidCount = oidList->count();
  devSnmp_oid **oidArray = (devSnmp_oid **) oidList->rawArray();

  if (op == NETSNMP_CALLBACK_OP_RECEIVED_MESSAGE) {
    //
    // received a reply
    //
    if (pdu->errstat == SNMP_ERR_NOERROR) {
      //
      // no errors reported in request
      //

      //
      // sanity check: count variables returned, is an error if doesn't
      // match count we asked for
      //
      // this is needed for MOXA devices (possibly others) that return no
      // error if too many oids requested and just return as many as it
      // could handle (very unfriendly!)
      //
      int sanityCount = 0;
      netsnmp_variable_list *vars = pdu->variables;
      while (vars) {
        vars = vars->next_variable;
        sanityCount++;
      }

      if (sanityCount != oidList->count()) {
        // SANITY CHECK FAILED
        if (pOurGroup) pOurGroup->sessionGotError(this);
        if (snmpDebugLevel >= 3) {
          printf("*** devSnmp %s transaction (GET) sanity check failed: asked for %d oids got %d\n"
                 "*** (try adding epicsSnmpSetMaxOidsPerReq call to ioc script)\n",
                 ((pOurGroup) ? pOurGroup->hostName() : ""), oidCount, sanityCount);
        }
        // inform PVs we had a problem but it isn't their fault
        // (send them SNMP_ERR_NOERROR but var is NULL)
        for (int ii = 0; ii < oidCount; ii++) {
          devSnmp_oid *pOID = oidArray[ii];
          if (! pOID) continue;
          pOID->replyProcessing(this,op,SNMP_ERR_NOERROR,NULL);
        }
      } else {
        // roll through reply variables in PDU, sending data along to the
        // devSnmp_oid they belong to for processing
        netsnmp_variable_list *vars = pdu->variables;
        int idx = 0;
        int badReturns = 0;
        while (vars) {
          devSnmp_oid *pOID = oidArray[idx];
          if (pOID) {
            int stat = pOID->replyProcessing(this,op,SNMP_ERR_NOERROR,vars);
            if (stat != epicsOk) badReturns++;
          }
          vars = vars->next_variable;
          idx++;
        }
        if ((badReturns) && (snmpDebugLevel >= 2)) analyzeMismatch(pdu->variables,oidArray);
      }
    } else {
      //
      // error in request
      //
      // send error status to PV that caused it
      // send no error (but no var either) to other innocent PVs
      //
      if (pOurGroup) pOurGroup->sessionGotError(this);
      int bad_index = pdu->errindex - 1;  // is 1..x, make 0..x-1
      if (snmpDebugLevel >= 3) {
        printf("*** devSnmp %s transaction (GET) error = %ld (%s)\n",
               ((pOurGroup) ? pOurGroup->hostName() : ""),
               pdu->errstat, snmp_errstring(pdu->errstat) );
      }
      for (int ii = 0; ii < oidCount; ii++) {
        devSnmp_oid *pOID = oidArray[ii];
        if (! pOID) continue;
        if (ii == bad_index)
          pOID->replyProcessing(this,op,pdu->errstat,NULL);
        else
          pOID->replyProcessing(this,op,SNMP_ERR_NOERROR,NULL);
      }
    }
  } else {
    //
    // operation FAILED (timed out, etc)
    //
    // send op to each PV for processing (we have to pass an error code,
    // so we just send SNMP_ERR_NOERROR, PVs will be processing by op)
    //
    if (pOurGroup) pOurGroup->sessionGotError(this);
    if (snmpDebugLevel >= 3) {
      printf("*** devSnmp %s transaction (GET) op = %d\n",
              ((pOurGroup) ? pOurGroup->hostName() : ""),
              op);
    }
    for (int ii = 0; ii < oidCount; ii++) {
      devSnmp_oid *pOID = oidArray[ii];
      if (! pOID) continue;
      if (pOID) pOID->replyProcessing(this,op,SNMP_ERR_NOERROR,NULL);
    }
  }

  // done with reply, flag ourselves as completed so we can be
  // deleted later by our owner
  completed = true;

  return(1);  // 1 = we're done with request, don't keep it pending
}
//--------------------------------------------------------------------
bool devSnmp_session::open(SNMP_SESSION *psess)
{
  // error if already opened
  if (session) return(false);

  // open session
  session = snmp_open(psess);
  if (! session) return(false);
  session->callback = snmpSessionCallback;
  session->callback_magic = this;

  // create PDU
  pdu = snmp_pdu_create( (is_setting) ? SNMP_MSG_SET : SNMP_MSG_GET );
  if (! pdu) {
    snmp_close(session);
    session = NULL;
    return(false);
  }

  return(true);
}
//--------------------------------------------------------------------
void devSnmp_session::addReading(devSnmp_oid *pOID)
{
  if ((pdu) && (! is_setting)) {
    snmp_add_null_var(pdu,pOID->getOid()->Oid,pOID->getOid()->OidLen);
    oidList->append(pOID);
  }
}
//--------------------------------------------------------------------
void devSnmp_session::addSetting(devSnmp_oid *pOID, devSnmp_setting *pSet)
{
  if ((pdu) && (is_setting)) {
    if (snmpDebugLevel >= 2) {
      printf("%s devSnmp_session::addSetting '/%s/%s/%s' : '%c' '%s'\n",
             tnow(),
             pOID->hostName(),
             pOID->communityName(),
             pOID->oidName(),
             pSet->setType(),
             pSet->setString());
    }
    snmp_add_var(pdu,pOID->getOid()->Oid,pOID->getOid()->OidLen,
                 pSet->setType(),pSet->setString());
    oidList->append(pOID);
  }
}
//--------------------------------------------------------------------
bool devSnmp_session::send(void)
{
  tried_send = true;

  bool state;
  if (snmp_send(session,pdu)) {
    timeSent.start(&globalLastTick);
    sent = true;
    // inc manager's request counter so its read task knows to be reading
    pOurMgr->incActiveRequests();
    state = true;
  } else {
    state = false;
  }

  // inform group of send success (for counters purposes)
  if (pOurGroup) pOurGroup->sessionSent(state);

  // inform PVs of send status
  int oidCount = oidList->count();
  devSnmp_oid **oidArray = (devSnmp_oid **) oidList->rawArray();
  for (int ii = 0; ii < oidCount; ii++) {
    devSnmp_oid *pOID = oidArray[ii];
    if (! pOID) continue;
    if (is_setting)
      pOID->settingSendStatus(state);
    else
      pOID->readingSendStatus(state);
  }

  return(sent);
}
//--------------------------------------------------------------------
void devSnmp_session::close(void)
{
  pOurMgr->sessionMutexLock();

  try {
    // if tried to send then session owns pdu and we shouldn't free it
    // otherwise we need to free it
    if ((pdu) && (! tried_send)) snmp_free_pdu(pdu);
    if (session) snmp_close(session);
  } catch( ... ) {
    printf("devSnmp : EXCEPTION in devSnmp_session::close\n");
  }

  pdu = NULL;
  session = NULL;

  pOurMgr->sessionMutexUnlock();
}
//--------------------------------------------------------------------
int devSnmp_session::itemCount(void)
{
  return (oidList) ? oidList->count() : 0;
}
//--------------------------------------------------------------------
bool devSnmp_session::wasSent(void)
{
  return(sent);
}
//--------------------------------------------------------------------
bool devSnmp_session::isCompleted(void)
{
  return(completed);
}
//--------------------------------------------------------------------
bool devSnmp_session::isSetting(void)
{
  return(is_setting);
}
//--------------------------------------------------------------------
double devSnmp_session::secondsSinceCreated(epicsTimeStamp *pnow)
{
  return( timeStarted.elapsedSeconds(pnow) );
}
//--------------------------------------------------------------------
double devSnmp_session::secondsSinceSent(epicsTimeStamp *pnow)
{
  return( timeSent.elapsedSeconds(pnow) );
}
//--------------------------------------------------------------------
long devSnmp_session::millisecondsSinceCreated(epicsTimeStamp *pnow)
{
  return( timeStarted.elapsedMilliseconds(pnow) );
}
//--------------------------------------------------------------------
long devSnmp_session::millisecondsSinceSent(epicsTimeStamp *pnow)
{
  return( timeSent.elapsedMilliseconds(pnow) );
}
//--------------------------------------------------------------------
// class devSnmp_transaction
//--------------------------------------------------------------------
devSnmp_transaction::devSnmp_transaction(bool isSet)
{
  createTime.start(&globalLastTick);
  is_set = isSet;
}
//--------------------------------------------------------------------
devSnmp_transaction:: ~devSnmp_transaction(void)
{
  // nothing to do here
}
//--------------------------------------------------------------------
bool devSnmp_transaction::isSetting(void)
{
  return(is_set);
}
//--------------------------------------------------------------------
long devSnmp_transaction::millisecondsSinceCreated(epicsTimeStamp *pnow)
{
  return( createTime.elapsedMilliseconds(pnow) );
}
//--------------------------------------------------------------------
// class devSnmp_getTransaction
//--------------------------------------------------------------------
devSnmp_getTransaction::devSnmp_getTransaction(int MaxItems)
   : devSnmp_transaction(false)
{
  oidList = new snmpPointerList();
  ourMaxItems = (MaxItems > 0) ? MaxItems : 1;
}
//--------------------------------------------------------------------
devSnmp_getTransaction::~devSnmp_getTransaction(void)
{
  //
  // delete oidList but not items in it, those belong to other objects
  // we also inform all OIDs in the list they're now unqueued
  //
  if (oidList) {
    int oidCount = oidList->count();
    devSnmp_oid **oidArray = (devSnmp_oid **) oidList->rawArray();
    for (int ii = 0; ii < oidCount; ii++) {
      devSnmp_oid *pOID = oidArray[ii];
      if (! pOID) continue;
      pOID->setGetQueued(false);
    }
    delete oidList;
    oidList = NULL;
  }
}
//--------------------------------------------------------------------
void devSnmp_getTransaction::addOID(devSnmp_oid *pOID)
{
  if (oidList) {
    oidList->append(pOID);
    pOID->setGetQueued(true);
  }
}
//--------------------------------------------------------------------
int devSnmp_getTransaction::count(void)
{
  if (! oidList) return(0);
  return(oidList->count());
}
//--------------------------------------------------------------------
bool devSnmp_getTransaction::isFull(void)
{
  if (! oidList) return(true);
  if (oidList->count() >= ourMaxItems) return(true);
  return(false);
}
//--------------------------------------------------------------------
devSnmp_session *devSnmp_getTransaction::createSession(void)
{
  // no good if no OIDs
  if ((! oidList) || (oidList->count() == 0)) return(NULL);

  // create session
  devSnmp_oid *pFirstOID = (devSnmp_oid *) oidList->itemAt(0);
  devSnmp_group *pGroup = pFirstOID->getGroup();
  devSnmp_session *pSession = new devSnmp_session(pFirstOID->getManager(),
                                                  pGroup,
                                                  false);

  // open session
  if (! pSession->open(pGroup->getBaseSession())) {
    snmp_perror("devSnmp_getTransaction::createSession : GET session open failed");
    delete pSession;
    return(NULL);
  }

  // add OIDs to session
  int oidCount = oidList->count();
  devSnmp_oid **oidArray = (devSnmp_oid **) oidList->rawArray();
  for (int ii = 0; ii < oidCount; ii++) {
    devSnmp_oid *pOID = oidArray[ii];
    if (! pOID) continue;
    pSession->addReading(pOID);
  }

  return(pSession);
}
//--------------------------------------------------------------------
// class devSnmp_setTransaction
//--------------------------------------------------------------------
devSnmp_setTransaction::devSnmp_setTransaction(devSnmp_oid *pOID)
   : devSnmp_transaction(true)
{
  // store oid
  ourOID = pOID;
  ourSetting = NULL;  /* this is set in createSession */
}
//--------------------------------------------------------------------
devSnmp_setTransaction::~devSnmp_setTransaction(void)
{
  if (ourSetting) {
    delete ourSetting;
    ourSetting = NULL;
  }
}
//--------------------------------------------------------------------
devSnmp_session *devSnmp_setTransaction::createSession(void)
{
  // create session
  if (! ourOID) return(NULL);
  ourSetting = ourOID->getNextSetting();
  if (! ourSetting) return(NULL);

/*
  if (strcmp(ourOID->oidName(),"WIENER-CRATE-MIB::outputVoltage.u0") == 0) {
    printf("%s  %s createSession %s\n",tnow(),ourOID->oidName(),ourSetting->setString());
  }
*/
  devSnmp_group *pGroup = ourOID->getGroup();
  devSnmp_session *pSession = new devSnmp_session(ourOID->getManager(),
                                                  pGroup,
                                                  true);

  // open session
  if (! pSession->open(pGroup->getBaseSession())) {
    snmp_perror("devSnmp_setTransaction::createSession : SET session open failed");
    delete pSession;
    return(NULL);
  }

  // add setting to session
  pSession->addSetting(ourOID,ourSetting);

  return(pSession);
}
//--------------------------------------------------------------------
bool devSnmp_setTransaction::sameOid(devSnmp_setTransaction *pTrans)
{
  return( (pTrans->ourOID == ourOID) ? true : false );
}
//--------------------------------------------------------------------
// class devSnmp_oid
//--------------------------------------------------------------------
devSnmp_oid::devSnmp_oid
     (devSnmp_manager *pMgr,
      devSnmp_group   *pGroup,
      configDataOid   *pOidBase,
      OID             *pOid,
      bool            *okay)
{
  (*okay) = false;

  // store parameters
  pOurMgr   = pMgr;
  pOurGroup = pGroup;
  memcpy(&oidBase,pOidBase,sizeof(configDataOid));
  memcpy(&oid,pOid,sizeof(OID));

  // init variables
  read_data        = NULL;
  valMutex         = epicsMutexCreate();
  setMutex         = epicsMutexCreate();
  flagged_read_bad = SNMP_ERR_NOERROR;
  has_reading      = false;
  queued_for_get   = false;
  validFlag        = false;
  pollSendCount    = 0;
  pollReplyCount   = 0;
  setCount         = 0;
  errorCount       = 0;
  compFailures     = 0;
  pollWeight       = snmpDoNotPollWeight;
  settingToSend    = NULL;
  setDebugging     = false;
  strcpy(lastError,"(none)");

  // set some defaults, PVs that use us will override as appropriate
  setPollMSec(10000);
  setDataLength(40);

  // init times
  pollStart.start(&globalLastTick);
  lastSetSent.clear();
  lastPollSent.clear();
  lastPollReply.clear();

  (*okay) = true;
}
//--------------------------------------------------------------------
devSnmp_oid::~devSnmp_oid(void)
{
  if (settingToSend) {
    delete settingToSend;
    settingToSend = NULL;
  }
  epicsMutexDestroy(setMutex);
  epicsMutexDestroy(valMutex);
}
//--------------------------------------------------------------------
const char *devSnmp_oid::errorString(void)
{
  return(lastError);
}
//--------------------------------------------------------------------
bool devSnmp_oid::hasReading(void)
{
  return(has_reading);
}
//--------------------------------------------------------------------
bool devSnmp_oid::getRawValueString(char *str, int maxsize)
{
  // if we have no valid reading, return false
  //
  // NOTE: having a reading doesn't mean it's a reading value the PV
  // will like when it goes to parse it or that the specified mask
  // will be matched, just that we got a non-timeout response from the
  // remote host with a string value.
  if (! has_reading) {
    strcpy(lastError,"oid has no valid reading");
    return(false);
  }

  epicsMutexLock(valMutex);
  copy_string(str,maxsize,read_data);
  epicsMutexUnlock(valMutex);
  return(true);
}
//--------------------------------------------------------------------
void devSnmp_oid::periodicProcessing(epicsTimeStamp *pnow)
{
  // if we have a reading, make sure it isn't too stale and flag
  // a read error if it is (guards against manager's readTask hanging up)
  if ((has_reading) && (lastPollReply.elapsedMilliseconds(pnow) > snmpDataStaleTimeoutMSec)) {
    if (snmpDebugLevel) printf("%s  devSnmp %s data stale (%d msec overdue for poll)\n",
                               tnow(), oidName(), snmpDataStaleTimeoutMSec);
    epicsMutexLock(valMutex);
    has_reading = false;
    epicsMutexUnlock(valMutex);
  }
}
//--------------------------------------------------------------------
int devSnmp_oid::getDataLength(void)
{
  return(data_len);
}
//--------------------------------------------------------------------
void devSnmp_oid::setDataLength(int length)
{
  epicsMutexLock(valMutex);

  data_len = length;

  // set length of val field
  if (! read_data) {
    read_data = new char[data_len];
    read_data[0] = 0;
  } else {
    char *newval = new char[data_len];
    copy_string(newval,data_len,read_data);
    delete [] read_data;
    read_data = newval;
  }

  epicsMutexUnlock(valMutex);
}
//--------------------------------------------------------------------
int devSnmp_oid::badFlag(void)
{
  return(flagged_read_bad);
}
//--------------------------------------------------------------------
double devSnmp_oid::pollsPerSecond(void)
{
  if (pollReplyCount == 0) return(0.0);
  double diffsec = pollStart.elapsedSeconds(NULL);
  if (diffsec <= 0.0) return(0.0);
  return(pollReplyCount / diffsec);
}
//--------------------------------------------------------------------
long devSnmp_oid::pollAgeMSec(void)
{
  return( lastPollReply.elapsedMilliseconds(NULL) );
}
//--------------------------------------------------------------------
int devSnmp_oid::getPollMSec(void)
{
  return(pollMSec);
}
//--------------------------------------------------------------------
void devSnmp_oid::setPollMSec(int msec)
{
  // prevent gap from being less than 100 msec (10 poll/sec)
  pollMSec = (msec < 100) ? 100 : msec;
}
//--------------------------------------------------------------------
void devSnmp_oid::debugSetStart()
{
  if (setDebugging) {
    debugSetProgress("debugSetStart already called");
  } else {
    setDebugging = true;
    debugSetTime.start(NULL);
    debugSetProgress("----------------------------------------");
    debugSetProgress("set start");
  }
}
//--------------------------------------------------------------------
void devSnmp_oid::debugSetProgress(const char *msg)
{
  if (setDebugging) {
    char tmp[256];
    sprintf(tmp,"[%9.3lf] debugSetProgress %s %s\n",
            debugSetTime.elapsedSeconds(NULL),
            oidName(),
            msg);
    printf("%s  %s\n",tnow(),tmp);
  }
}
//--------------------------------------------------------------------
void devSnmp_oid::debugSetEnd()
{
  if (setDebugging) {
    debugSetProgress("set end");
    setDebugging = false;
  }
}
//--------------------------------------------------------------------
void devSnmp_oid::set(char set_type, char *str)
{
  epicsMutexLock(setMutex);

  // create a setting object
  devSnmp_setting *pSet = new devSnmp_setting(set_type,str);

  if (setDebugging) {
    char tmp[256];
    sprintf(tmp,"set('%c',\"%s\")\n",set_type,str);
    debugSetProgress(tmp);
  }

  // replace any waiting setting with this one
/*
  if (strcmp(oidName(),"WIENER-CRATE-MIB::outputVoltage.u0") == 0) {
    if (settingToSend)
      printf("%s  %s replace %s with %s\n",tnow(),oidName(),settingToSend->setString(),str);
    else
      printf("%s  %s set to %s\n",tnow(),oidName(),str);
  }
*/
  if (settingToSend) {
    delete settingToSend;
    settingToSend = NULL;
  }
  settingToSend = pSet;
  setCount++;

  epicsMutexUnlock(setMutex);
}
//--------------------------------------------------------------------
bool devSnmp_oid::needsSet(void)
{
  // return whether there's a setting waiting to go out
  bool need = (settingToSend != NULL);
  return(need);
}
//--------------------------------------------------------------------
devSnmp_setting *devSnmp_oid::getNextSetting(void)
{
  devSnmp_setting *return_setting = NULL;

  epicsMutexLock(setMutex);

  debugSetProgress("getNextSetting()");

  return_setting = settingToSend;
  if (settingToSend) {
    // we don't delete settingToSend here
    // that gets done elsewhere after set is completed
    // just NULL-out pointer
    settingToSend = NULL;
  }

  epicsMutexUnlock(setMutex);

  return(return_setting);
}
//--------------------------------------------------------------------
long devSnmp_oid::recalcPollWeight(epicsTimeStamp *pnow)
{
  // don't poll us if:
  //     we're already queued for a poll (or still waiting for a reply)
  //     or we've been flagged as a bad oid in some previous poll
  if ((queued_for_get) || (flagged_read_bad != SNMP_ERR_NOERROR)) {
    pollWeight = snmpDoNotPollWeight;
    return(pollWeight);
  }

  //
  // calculate poll weight based on time until we need to be polled again
  //
  // a poll weight of:
  //   < 0 : PV is past due for poll (more negative = more overdue)
  //   = 0 : PV is due for poll right now
  //   > 0 : PV isn't due for a poll yet (larger num = more time until due)
  //
  pollWeight = pollMSec - lastPollSent.elapsedMilliseconds(pnow);
  // keep from getting too large
  if (pollWeight < -65535) pollWeight = -65535;
  if (pollWeight >  65535) pollWeight =  65535;
  return(pollWeight);
}
//--------------------------------------------------------------------
long devSnmp_oid::getPollWeight(void)
{
  return(pollWeight);
}
//--------------------------------------------------------------------
void devSnmp_oid::setGetQueued(bool state)
{
  queued_for_get = state;
}
//--------------------------------------------------------------------
devSnmp_manager *devSnmp_oid::getManager(void)
{
  return(pOurMgr);
}
//--------------------------------------------------------------------
devSnmp_group *devSnmp_oid::getGroup(void)
{
  return(pOurGroup);
}
//--------------------------------------------------------------------
const char *devSnmp_oid::oidName(void)
{
  return( (oid.Name) ? oid.Name : "(null)" );
}
//--------------------------------------------------------------------
const char *devSnmp_oid::hostName(void)
{
  return(oidBase.host);
}
//--------------------------------------------------------------------
const char *devSnmp_oid::communityName(void)
{
  return(oidBase.community);
}
//--------------------------------------------------------------------
OID *devSnmp_oid::getOid(void)
{
  return(&oid);
}
//--------------------------------------------------------------------
void devSnmp_oid::settingSendStatus(bool state)
{
  if (state) {
    lastSetSent.start(&globalLastTick);
  } else {
    errorCount++;
  }
}
//--------------------------------------------------------------------
void devSnmp_oid::readingSendStatus(bool state)
{
  if (state) {
    pollSendCount++;
    lastPollSent.start(&globalLastTick);
  } else {
    errorCount++;
  }
}
//--------------------------------------------------------------------
void devSnmp_oid::zeroCounters(void)
{
  pollSendCount = 0;
  pollReplyCount = 0;
  setCount = 0;
  errorCount = 0;
  pollStart.start(&globalLastTick);
}
//--------------------------------------------------------------------
bool devSnmp_oid::reportMatchAny(char *match)
{
  if (! match) return(true);  // no match string specified, always matches
  return( strstr(oidName(),match) ? true : false );
}
//--------------------------------------------------------------------
void devSnmp_oid::report(int level, char *match)
{
  if (level < 1) return;  // we don't report by-OID data unless level is 1+

  if (level == 1) {
    double desiredPollsPerSec = (1000.0 / pollMSec);
    printf("    %10ld  %10ld  %8ld  %9.2lf  %9.2lf  %10ld  %6ld  %s\n",
           pollSendCount,
           pollReplyCount,
           setCount,
           desiredPollsPerSec,
           pollsPerSecond(),
           pollAgeMSec(),
           errorCount,
           oidName());
    return;
  }

  char timestr[40];
  epicsMutexLock(setMutex);
  long settingsWaiting;
  settingsWaiting = (settingToSend) ? 1 : 0;
  epicsMutexUnlock(setMutex);
  lastPollReply.toDateTimeString(timestr);
  printf("    %s\n",OIDPV_BAR);
  printf("    OID '%s'\n", oidName());
  printf("    %s\n",OIDPV_BAR);
  printf("    Host/Community   : %s/%s\n",   hostName(), communityName());
  printf("    Poll weight      : %ld\n",     pollWeight);
  printf("    Polls sent       : %ld\n",     pollSendCount);
  printf("    Poll replies     : %ld\n",     pollReplyCount);
  printf("    Settings sent    : %ld\n",     setCount);
  printf("    Settings waiting : %ld\n",     settingsWaiting);
  printf("    Errors           : %ld\n",     errorCount);
  printf("    Poll rate (msec) : %ld\n",     pollMSec);
  printf("    Poll age (msec)  : %ld\n",     pollAgeMSec());
  printf("    Polls/sec        : %.2lf\n",   pollsPerSecond());
  printf("    Last reply at    : %s\n",      timestr);
  printf("    Max data length  : %d\n",      data_len);
  printf("    Raw value string : '%s'\n",    read_data);
  if (flagged_read_bad != SNMP_ERR_NOERROR)
    printf("    *** Read flag bad  : %d (%s)\n",
           flagged_read_bad, snmp_errstring(flagged_read_bad));
  printf("\n");
}
//--------------------------------------------------------------------
void devSnmp_oid::debugDump(void)
{
  printf("  oid %s\n",oidName());
  printf("    Has reading      : %d\n",has_reading);
  printf("    Max data length  : %d\n",data_len);
  printf("    Raw value string : '%s'\n",read_data);
}
//--------------------------------------------------------------------
int devSnmp_oid::replyProcessing
     (devSnmp_session       *pSession,
      int                    op,
      long                   errCode,
      netsnmp_variable_list *var)
// called via snmpSessionCallback() in read thread
// host session mutex will be held already
{
  int retVal;

  if (pSession->isSetting())
    retVal = setReplyProcessing(pSession,op,errCode,var);
  else
    retVal = readReplyProcessing(pSession,op,errCode,var);

  return(retVal);
}
//--------------------------------------------------------------------
int devSnmp_oid::readReplyProcessing
     (devSnmp_session       *pSession,
      int                    op,
      long                   errCode,
      netsnmp_variable_list *var)
// called via snmpSessionCallback() in read thread
// host session mutex will be held already
{
  epicsMutexLock(valMutex);

  int retVal = epicsOk;

  if (op == NETSNMP_CALLBACK_OP_RECEIVED_MESSAGE) {
    // a session for us received a message
    if (errCode == SNMP_ERR_NOERROR) {
      // no error code for us
      if (var) {
        // our reading succeeded, make sure variable oid matches our requested oid
        if (snmp_oid_compare(var->name,var->name_length,oid.Oid,oid.OidLen) != 0) {
          // compare failed
          compFailures++;
          if (snmpDebugLevel >= 2)
            printf("%s  devSnmp oid mismatch (%ld/%d) for '%s/%s'\n",
                   tnow(),
                   compFailures,
                   snmpMaxOidCompFailures,
                   hostName(),
                   oidName());
          if ((int)compFailures > snmpMaxOidCompFailures) {
            // exceeded our max compare failures
            if (snmpDebugLevel) printf("%s  devSnmp oid mismatch limit exceeded (%ld/%d) for '%s/%s'\n",
                                     tnow(),
                                     compFailures,
                                     snmpMaxOidCompFailures,
                                     hostName(),
                                     oidName());
            has_reading = false;
          }
          retVal = epicsError;
        } else {
          // compare succeeded, pull out data
          compFailures = 0;
          char buffer[1024];
          snprint_value(buffer, sizeof(buffer), var->name, var->name_length, var);
          copy_string(read_data,data_len,buffer);
          has_reading = true;
        }
      } else {
        // no error but DIDN'T get a variable, so some other PV in our
        // session caused an error and as a result WE don't get our data!
        //
        // clear our lastPollSent time so we're polled again at the
        // next available opportunity
        lastPollSent.clear();
        retVal = epicsOk;
      }
      // this counts as a poll reply, even if we got no data because
      // some other PV in our session had an error
      pollReplyCount++;
      lastPollReply.start(&globalLastTick);
    } else {
      // WE were the PV that caused our session to fail
      errorCount++;
      if (snmpDebugLevel >= 3) {
        printf("*** devSnmp %s (GET) error = %ld (%s)\n",
                oidBase.oidStr, errCode, snmp_errstring(errCode));
      }
      // if was because no such oid, set flag so we don't get polled again
      if (errCode == SNMP_ERR_NOSUCHNAME) {
        flagged_read_bad = errCode;
        printf("*** devSnmp %s (GET) flagged bad, error = %ld (%s)\n",
                oidBase.oidStr, errCode, snmp_errstring(errCode));
      }
      retVal = epicsError;
    }
  } else {
    // no message from session (timed out, is disconnected, etc)
    errorCount++;
    retVal = epicsError;
  }

  epicsMutexUnlock(valMutex);

  return(retVal);
}
//--------------------------------------------------------------------
int devSnmp_oid::setReplyProcessing
     (devSnmp_session       *pSession,
      int                    op,
      long                   errCode,
      netsnmp_variable_list *var)
// called via snmpSessionCallback() in read thread
// host session mutex will be held already
{
  int retVal = epicsOk;

  if (op == NETSNMP_CALLBACK_OP_RECEIVED_MESSAGE) {
    // a session for us received a message
    if (errCode == SNMP_ERR_NOERROR) {
      // no error code for us
      if (var) {
        // we got a variable passed to us, so our set succeeded
        // (nothing to do)
        if (snmpDebugLevel >= 2) {
          printf("----- devSnmp SET %s okay\n",oidBase.oidStr);
          retVal = epicsError;
        }
        debugSetProgress("setReplyProcessing() : set successful");
        debugSetEnd();
        retVal = epicsOk;
      }
    } else {
      // our set failed
      errorCount++;
      if (snmpDebugLevel) printf("----- devSnmp SET %s failed : %ld (%s)\n",
                                oidBase.oidStr, errCode, snmp_errstring(errCode));
      debugSetProgress("setReplyProcessing() : set FAILED");
      retVal = epicsError;
    }
  } else {
    // no message from session (timed out, got disconnected, etc)
    errorCount++;
    if (snmpDebugLevel) {
      printf("----- devSnmp SET %s failed : op=%d\n",oidBase.oidStr, op);
      retVal = epicsError;
    }
    debugSetProgress("setReplyProcessing() : set TIMED OUT");

    // we DON'T try to re-send setting that failed, it is lost
    // (don't want to be sending old stale settings if remote host
    // is down for a couple days...)
  }

  return(retVal);
}
//--------------------------------------------------------------------
// class devSnmp_pv
//--------------------------------------------------------------------
devSnmp_pv::devSnmp_pv
     (devSnmp_manager  *pMgr,
      devSnmp_group    *pGroup,
      struct dbCommon  *pRecord,
      devSnmp_oid      *pOID,
      configDataPV     *pOidExtra,
      bool             *okay)
{
  (*okay) = false;  // for now...

  // store parameters
  pOurMgr    = pMgr;
  pOurGroup  = pGroup;
  pOurRecord = pRecord;
  pOurOID    = pOID;
  memcpy(&oidExtra,pOidExtra,sizeof(configDataPV));

  // initialize misc variables
  pPeriodicFunction = NULL;
  in_rec_process    = false;
  periodicMSec      = 0;
  setCount          = 0;
  lastSetSent.clear();
  lastPeriodicFuncCall.clear();
  strcpy(lastError,"(none)");

  // set data length
  setDataLength(oidExtra.data_len);

  // get our scan time, make sure OID object is set to poll at least that fast
  switch (pRecord->scan) {
    case menuScanPassive:   pollMSec = snmpPassivePollMSec; break;
    case menuScan10_second: pollMSec = 10000;               break;
    case menuScan5_second:  pollMSec = 5000;                break;
    case menuScan2_second:  pollMSec = 2000;                break;
    case menuScan1_second:  pollMSec = 1000;                break;
    case menuScan_5_second: pollMSec = 500;                 break;
    case menuScan_2_second: pollMSec = 200;                 break;
    case menuScan_1_second: pollMSec = 100;                 break;
    default:                pollMSec = 200;                 break;
  }
  if (pollMSec < pOurOID->getPollMSec()) pOurOID->setPollMSec(pollMSec);

  (*okay) = true;
}
//--------------------------------------------------------------------
devSnmp_pv::~devSnmp_pv(void)
{
  // we don't delete OID object, it belongs to our group
}
//--------------------------------------------------------------------
const char *devSnmp_pv::devSnmp_pv::errorString(void)
{
  return(lastError);
}
//--------------------------------------------------------------------
bool devSnmp_pv::getValueString(char *str, int maxsize)
{
  // get raw string
  char rawData[ oidExtra.data_len ];
  if (! getRawValueString(rawData,oidExtra.data_len)) return(false);

  // try to locate our mask
  char *pc = snmpStrStr(rawData, oidExtra.mask);
  if (! pc) {
    sprintf(lastError,"mask '%s' not found in '%s'",oidExtra.mask,rawData);
    return(false);
  }

  // skip over mask
  pc += strlen(oidExtra.mask);

  // skip over whitespace
  while (ISSPACE(*pc) && ((*pc) != 0)) pc++;

  // copy string to caller's buffer
  copy_string(str,maxsize,pc);

  // do any special handling applicable
  if (oidExtra.special_flags & SPECIAL_FLAG_PACK) {
    // remove all whitespace
    int jj = 0;
    for (int ii = 0; str[ii] != 0; ii++) {
      if ((str[ii] != ' ') && (str[ii] != '\t')) {
        str[jj] = str[ii];
        jj++;
      }
    }
    str[jj] = 0;
  }

  return(true);
}
//--------------------------------------------------------------------
bool devSnmp_pv::getValueDouble(double *value)
{
  char str[ oidExtra.data_len ];
  if (! getValueString(str,sizeof(str))) return(false);
  if (epicsScanDouble(str,value) != 1) {
    sprintf(lastError,"epicsScanDouble failed on '%s'",str);
    return(false);
  }
  if (isnan(*value)) {
    sprintf(lastError,"'%s' = isnan",str);
    return(false);
  }
  return(true);
}
//--------------------------------------------------------------------
bool devSnmp_pv::getValueLong(long *value)
{
  char str[ oidExtra.data_len ];
  if (! getValueString(str,sizeof(str))) return(false);

  if (! (oidExtra.special_flags & SPECIAL_FLAG_HEXBITS)) {
    // is just a regular number
    if (! snmpScanLong(str,value)) {
      sprintf(lastError,"snmpScanLong failed on '%s'",str);
      return(false);
    }
  } else {
    // is hexbits, hex-encoded bytes separated by spaces
    // an annoying format used by Wiener/ISEG crates...
    //
    // we need to count up pairs of valid hex-encoded bytes
    // and remove the spaces
    strcat(str," ");  // make sure string ends with a space delimiter
    int pairs = 0;
    char *pc = str;
    while (VALIDHEX(pc[0]) && VALIDHEX(pc[1]) && ISSPACE(pc[2])) {
      memmove(str+(pairs*2),pc,3);
      pc += 3;
      pairs++;
      if (pairs == 4) break;
    }
    str[pairs*2] = 0;

    // parse resulting hex-encoded string
    if (! snmpScanLong(str,value,16)) {
      sprintf(lastError,"snmpScanLong(hex) failed on '%s'",str);
      return(false);
    }

    // another annoying feature of Wiener/ISEG crates:
    // they return bit-order as LSB..MSB
    if (oidExtra.special_flags & SPECIAL_FLAG_REVERSE_BITS) {
      // reverse bits
      long N = (*value);
      long R = 0;
      int bits = pairs * 8;
      for (int ii = 0; ii < bits; ii++) {
        long nmask = 0x0001 << ii;
        long rmask = 0x0001 << (bits-ii-1);
        if (N & nmask) R |= rmask;
      }
      (*value) = R;
    }
  }

  return(true);
}
//--------------------------------------------------------------------
bool devSnmp_pv::getRawValueString(char *str, int maxsize)
{
  if (! pOurOID) {
    sprintf(lastError,"oid object is NULL");
    return(false);
  }
  bool retstat = pOurOID->getRawValueString(str,maxsize);
  if (! retstat) strcpy(lastError,pOurOID->errorString());
  return(retstat);
}
//--------------------------------------------------------------------
double devSnmp_pv::pollsPerSecond(void)
{
  if (! pOurOID) {
    sprintf(lastError,"oid object is NULL");
    return(0.0);
  }
  return( pOurOID->pollsPerSecond() );
}
//--------------------------------------------------------------------
int devSnmp_pv::getPollMSec(void)
{
  if (! pOurOID) {
    sprintf(lastError,"oid object is NULL");
    return(0);
  }
  return( pOurOID->getPollMSec() );
}
//--------------------------------------------------------------------
void devSnmp_pv::setPollMSec(int msec)
{
  if (pOurOID) pOurOID->setPollMSec(msec);
}
//--------------------------------------------------------------------
void devSnmp_pv::periodicProcessing(epicsTimeStamp *pnow)
{
  // call periodic callback if is time to
  if (pPeriodicFunction) {
    if (lastPeriodicFuncCall.elapsedMilliseconds(pnow) >= periodicMSec) {
      lastPeriodicFuncCall.start(pnow);
      (*pPeriodicFunction)(this);
    }
  }
}
//--------------------------------------------------------------------
void devSnmp_pv::debugSetStart()
{
  if (pOurOID) pOurOID->debugSetStart();
}
//--------------------------------------------------------------------
void devSnmp_pv::debugSetProgress(const char *msg)
{
  if (pOurOID) pOurOID->debugSetProgress(msg);
}
//--------------------------------------------------------------------
void devSnmp_pv::debugSetEnd()
{
  if (pOurOID) pOurOID->debugSetEnd();
}
//--------------------------------------------------------------------
void devSnmp_pv::set(char *str)
{
  if (pOurOID) {
    setCount++;
    pOurOID->set(oidExtra.set_type,str);
    lastSetSent.start(&globalLastTick);
    if (snmpDebugLevel >= 2) {
      printf("%s  devSnmp %s setting type:'%c' value:'%s'\n",tnow(),pOurRecord->name, oidExtra.set_type, str);
    }
  }
}
//--------------------------------------------------------------------
bool devSnmp_pv::wasSetRecently(void)
{
  if (! pOurOID) {
    sprintf(lastError,"oid object is NULL");
    return(false);
  }
  return( (lastSetSent.elapsedMilliseconds(&globalLastTick) < snmpSetSkipReadbackMSec) ? true : false );
}
//--------------------------------------------------------------------
bool devSnmp_pv::doingProcess(void)
{
  return(in_rec_process);
}
//--------------------------------------------------------------------
void devSnmp_pv::processRecord(void)
{
  dbScanLock(pOurRecord);
  in_rec_process = true;
  dbProcess(pOurRecord);
  in_rec_process = false;
  dbScanUnlock(pOurRecord);
}
//--------------------------------------------------------------------
void devSnmp_pv::setPeriodicCallback(DEVSNMP_DEVFUNC procFunc, double seconds)
{
  pPeriodicFunction = procFunc;
  periodicMSec = (long) (seconds * 1000.0);
  lastPeriodicFuncCall.start(&globalLastTick);
}
//--------------------------------------------------------------------
const char *devSnmp_pv::recordName(void)
{
  return(pOurRecord->name);
}
//--------------------------------------------------------------------
struct dbCommon *devSnmp_pv::record(void)
{
  return(pOurRecord);
}
//--------------------------------------------------------------------
int devSnmp_pv::getDataLength(void)
{
  return( pOurOID ? pOurOID->getDataLength() : 0 );
}
//--------------------------------------------------------------------
void devSnmp_pv::setDataLength(int length)
{
  // set OID's length if given value is larger than oid currently has
  if ((pOurOID) && (length > pOurOID->getDataLength()))
    pOurOID->setDataLength(length);
}
//--------------------------------------------------------------------
void devSnmp_pv::zeroCounters(void)
{
  setCount = 0;
}
//--------------------------------------------------------------------
bool devSnmp_pv::reportMatchAny(char *match)
{
  if (! match) return(true);  // no match string specified, always matches
  if (strstr(recordName(),match)) return(true);
  if ((pOurOID) && (pOurOID->reportMatchAny(match))) return(true);
  return(false);
}
//--------------------------------------------------------------------
void devSnmp_pv::report(int level, char *match)
{
  if (level < 1) return;  // we don't report by-PV data unless level is 1+

  if (level == 1) {
    double desiredPollsPerSec = (1000.0 / pollMSec);
    printf("    %9.2lf  %9.2lf  %10ld  %8ld  %s\n",
           desiredPollsPerSec,
           pOurOID->pollsPerSecond(),
           pOurOID->pollAgeMSec(),
           setCount,
           pOurRecord->name);
    return;
  }

  char rstr[oidExtra.data_len];
  char vstr[oidExtra.data_len];
  if (! getRawValueString(rstr,sizeof(rstr))) strcpy(rstr,"n/a");
  if (! getValueString(vstr,sizeof(vstr))) strcpy(vstr,"n/a");
  printf("    %s\n",OIDPV_BAR);
  printf("    PV '%s'\n", recordName());
  printf("    %s\n",OIDPV_BAR);
  printf("    OID name         : %s\n",      pOurOID->oidName());
  printf("    Host/Community   : %s/%s\n",   pOurOID->hostName(), pOurOID->communityName());
  printf("    Set type         : '%c'\n",    oidExtra.set_type);
  printf("    Special flags    : 0x%04lx\n", oidExtra.special_flags);
  printf("    Settings count   : %ld\n",     setCount);
  printf("    Max data length  : %d\n",      oidExtra.data_len);
  printf("    Raw value string : '%s'\n",    rstr);
  printf("    Reply mask       : '%s'\n",    oidExtra.mask);
  printf("    Value string     : '%s'\n",    vstr);
  int bad_flag = pOurOID->badFlag();
  if (bad_flag != SNMP_ERR_NOERROR)
    printf("    *** Read flag bad  : %d (%s)\n",
           bad_flag, snmp_errstring(bad_flag));
  printf("\n");
}
//--------------------------------------------------------------------
void devSnmp_pv::debugDump(void)
{
  printf("%s PV %s\n",tnow(),recordName());
  char tmp[128];
  tmp[0] = 0;
  bool vok = getValueString(tmp,sizeof(tmp));
  printf("  value okay   : %d\n",vok);
  printf("  value string : '%s'\n",tmp);
  printf("  mask         : '%s'\n",oidExtra.mask);
  if (! pOurOID)
    printf("  NO OID!\n");
  else
    pOurOID->debugDump();
}
//--------------------------------------------------------------------
// class devSnmp_group
//--------------------------------------------------------------------
devSnmp_group::devSnmp_group(devSnmp_manager *pMgr, devSnmp_host *host, char *community, bool *okay)
{
  (*okay) = false; // for now...

  pOurMgr          = pMgr;
  pOurHost         = host;
  pvList           = new snmpPointerList();
  oidList          = new snmpPointerList();
  weightCollection = new snmpWeightCollection();
  bestReplyMsec    = 0;
  worstReplyMsec   = 0;
  avgReplyMsec     = 0.0;
  sendCount        = 0;
  replyCount       = 0;
  errorCount       = 0;
  maxOidsPerReq    = pOurHost->getMaxOidsPerReq();

  // create base session structure
  base_session = new SNMP_SESSION;
  if (! base_session) return;
  memset(base_session,0,sizeof(SNMP_SESSION));

  // initialize base session structure
  snmp_sess_init(base_session);
  base_session->retries       = snmpSessionRetries;
  base_session->timeout       = snmpSessionTimeout;
  base_session->peername      = dup_string(pOurHost->hostName());
  base_session->community     = (unsigned char *) dup_string(community);
  base_session->community_len = strlen(community);
  base_session->version       = pOurMgr->getHostSnmpVersion(base_session->peername);
  base_session->callback      = snmpSessionCallback;

  (*okay) = true;
}
//--------------------------------------------------------------------
devSnmp_group::~devSnmp_group(void)
{
  // delete our PVs
  if (pvList) {
    int pvCount = pvList->count();
    for (int ii = pvCount-1; ii >= 0; ii--) {
      devSnmp_pv *pTmp = (devSnmp_pv *) pvList->removeItemAt(ii);
      if (! pTmp) continue;
      delete pTmp;
    }
    delete pvList;
    pvList = NULL;
  }

  // delete our OIDs
  if (oidList) {
    int oidCount = oidList->count();
    for (int ii = oidCount-1; ii >= 0; ii--) {
      devSnmp_oid *pTmp = (devSnmp_oid *) oidList->removeItemAt(ii);
      if (! pTmp) continue;
      delete pTmp;
    }
    delete oidList;
    oidList = NULL;
  }

  // delete weight list
  if (weightCollection) {
    delete weightCollection;
    weightCollection = NULL;
  }

  // delete base session
  if (base_session) {
    delete [] base_session->peername;
    delete [] base_session->community;
    delete base_session;
    base_session = NULL;
  }
}
//--------------------------------------------------------------------
devSnmp_pv *devSnmp_group::addPV
     (configDataOid    *base,
      configDataPV     *extra,
      OID              *oid,
      struct dbCommon  *pRecord)
{
  // find or create matching oid object
  devSnmp_oid *pOID = findOID(base->oidStr);
  if (! pOID) pOID = createOID(base,oid);
  if (! pOID) return(NULL);

  // now we can create a PV object, return that to caller
  return( createPV(extra,pOID,pRecord) );
}
//--------------------------------------------------------------------
devSnmp_oid *devSnmp_group::findOID(char *oidStr)
{
  int oidCount = oidList->count();
  devSnmp_oid **oidArray = (devSnmp_oid **) oidList->rawArray();
  for (int ii = 0; ii < oidCount; ii++) {
    devSnmp_oid *pOID = oidArray[ii];
    if (! pOID) continue;
    if (strcmp(oidStr, pOID->oidName()) == 0) return(pOID);
  }
  return(NULL);
}
//--------------------------------------------------------------------
devSnmp_oid *devSnmp_group::createOID(configDataOid *base, OID *oid)
{
  // create OID object
  bool ok;
  devSnmp_oid *pOID = new devSnmp_oid(pOurMgr,this,base,oid,&ok);
  if (! pOID) return(NULL);
  if (! ok) {
    delete pOID;
    return(NULL);
  }

  // add item to our OID list and weight collection
  oidList->append(pOID);
  weightCollection->addOID(pOID);

  // keep OID list sorted
  // (no real reason, just makes reports nicer)
  oidList->sort(devSnmp_oidobj_compare);

  return(pOID);
}
//--------------------------------------------------------------------
devSnmp_pv *devSnmp_group::createPV
     (configDataPV    *extra,
      devSnmp_oid     *pOID,
      struct dbCommon *pRecord)
{
  // create PV object
  bool ok;
  devSnmp_pv *pPV = new devSnmp_pv(pOurMgr,this,pRecord,pOID,extra,&ok);
  if (! pPV) return(NULL);
  if (! ok) {
    delete pPV;
    return(NULL);
  }

  // add item to our PV list
  pvList->append(pPV);

  // keep PV list sorted
  // (no real reason, just makes reports nicer)
  pvList->sort(devSnmp_pv_compare);

  return(pPV);
}
//--------------------------------------------------------------------
const char *devSnmp_group::hostName(void)
{
  return(base_session->peername);
}
//--------------------------------------------------------------------
const char *devSnmp_group::communityName(void)
{
  return((const char *)base_session->community);
}
//--------------------------------------------------------------------
SNMP_SESSION *devSnmp_group::getBaseSession(void)
{
  return(base_session);
}
//--------------------------------------------------------------------
int devSnmp_group::snmpVersion(void)
{
  return(base_session->version);
}
//--------------------------------------------------------------------
void devSnmp_group::setMaxOidsPerReq(int maxoids)
{
  maxOidsPerReq = (maxoids > 0) ? maxoids : 1;
}
//--------------------------------------------------------------------
int devSnmp_group::getMaxOidsPerReq(void)
{
  return(maxOidsPerReq);
}
//--------------------------------------------------------------------
void devSnmp_group::processing(epicsTimeStamp *pnow)
{
  if ((! pvList) || (pvList->count() == 0)) return;    // if no PVs, we're done
  if ((! oidList) || (oidList->count() == 0)) return;  // if no OIDs, we're done

  //
  // allow OIDs to do any periodic processing they have
  //
  int oidCount = oidList->count();
  devSnmp_oid **oidArray = (devSnmp_oid **) oidList->rawArray();
  for (int ii = 0; ii < oidCount; ii++) {
    devSnmp_oid *pOID = oidArray[ii];
    if (! pOID) continue;
    if (pOID) pOID->periodicProcessing(pnow);
  }

  //
  // allow PVs to do any periodic processing they have
  //
  int pvCount = pvList->count();
  devSnmp_pv **pvArray = (devSnmp_pv **) pvList->rawArray();
  for (int ii = 0; ii < pvCount; ii++) {
    devSnmp_pv *pPV = pvArray[ii];
    if (! pPV) continue;
    if (pPV) pPV->periodicProcessing(pnow);
  }

  //
  // if any OIDs have a setting they need to send out, build a transaction
  // for each and queue it with our host to be sent out
  //
  for (int ii = 0; ii < oidCount; ii++) {
    devSnmp_oid *pOID = oidArray[ii];
    if (! pOID) continue;
    if (pOID->needsSet()) {
      // create devSnmp_setTransaction for setting and queue it
      devSnmp_setTransaction *pSetTrans = new devSnmp_setTransaction(pOID);
      pOurHost->queueSetTransaction(pSetTrans);
    }
  }

  //
  // do 'get' polling
  //

  // instruct all OIDs to recalculate their poll weight
  long minW = snmpDoNotPollWeight;
  for (int ii = 0; ii < oidCount; ii++) {
    devSnmp_oid *pOID = oidArray[ii];
    if (! pOID) continue;
    long thisW = pOID->recalcPollWeight(pnow);
    if ((ii == 0) || (thisW < minW)) minW = thisW;
  }

  // nothing else to do if no OIDs need to be polled
  if (minW > snmpMaxTopPollWeight) return;

  /*
    If we get here we need to poll at least one OID.  weightCollection
    object will arrange OIDs by most..least needing to be polled (lowest
    to highest poll weight) and we'll queue a get transaction for the
    most needy batch of them *IF* the most needy one's poll weight is
    less than or equal to MaxTopPollWeight (i.e. something actually
    needs to be polled right now).  This will ensure OIDs that need to
    be polled are polled, and we may as well fill up a request for other
    OIDs that need to be polled (or soon will be) while we're at it.

    OIDs needing a poll that don't make it into this request will be
    picked up on subsequent calls of through this routine.
  */
  weightCollection->transactionBuildStart();
  devSnmp_getTransaction *pGetTrans = new devSnmp_getTransaction(maxOidsPerReq);
  do {
    // get next OID to poll
    devSnmp_oid *pOID = weightCollection->topOID();
    if (! pOID) break;
    if (pOID->getPollWeight() >= snmpDoNotPollWeight) break;

    // move to next OID now (so we're pointing at it afterwards
    // even if we fill up our transaction below)
    weightCollection->nextOID();

    // add this PV to request, and stop if the request is then full
    pGetTrans->addOID(pOID);
    if (pGetTrans->isFull()) break;
  } while (true);

  weightCollection->transactionBuildEnd();

  // queue our constructed get transaction with our host object
  pOurHost->queueGetTransaction(pGetTrans);

  // that's it for us until next time...
}
//--------------------------------------------------------------------
void devSnmp_group::sessionGotError(devSnmp_session *pSession)
{
  errorCount++;
}
//--------------------------------------------------------------------
void devSnmp_group::sessionSent(int state)
{
  if (state)
    sendCount++;
  else
    errorCount++;
}
//--------------------------------------------------------------------
void devSnmp_group::sessionGotReply(devSnmp_session *pSession)
{
  // update avgReplyMsec, etc

  double msec = 1000.0 * pSession->secondsSinceSent(&globalLastTick);

  if (replyCount) {
    if (msec < bestReplyMsec) bestReplyMsec = msec;
    if (msec > worstReplyMsec) worstReplyMsec = msec;
  } else {
    bestReplyMsec = msec;
    worstReplyMsec = msec;
  }

  double tot_msec = avgReplyMsec * replyCount;
  replyCount++;
  tot_msec += msec;
  avgReplyMsec = tot_msec / replyCount;
}
//--------------------------------------------------------------------
void devSnmp_group::sessionRetriesChange(void)
{
  // applicable global variable value changed
  base_session->retries = snmpSessionRetries;
  // fix ? - unknown whether changing this on-the-fly will have any effect
  // might be picked up when devSnmp_[get/set]Transaction::createSession is called
}
//--------------------------------------------------------------------
void devSnmp_group::sessionTimeoutChange(void)
{
  // applicable global variable value changed
  base_session->timeout = snmpSessionTimeout;
  // fix ? - unknown whether changing this on-the-fly will have any effect
  // might be picked up when devSnmp_[get/set]Transaction::createSession is called
}
//--------------------------------------------------------------------
void devSnmp_group::zeroCounters(void)
{
  bestReplyMsec = 0.0;
  worstReplyMsec = 0.0;
  avgReplyMsec = 0.0;
  sendCount = 0;
  replyCount = 0;
  errorCount = 0;

  // roll through PVs zero-ing their counters
  int pvCount = pvList->count();
  for (int ii = 0; ii < pvCount; ii++) {
    devSnmp_pv *pPV = (devSnmp_pv *) pvList->itemAt(ii);
    if (! pPV) continue;
    pPV->zeroCounters();
  }

  // roll through OIDs zero-ing their counters
  int oidCount = oidList->count();
  for (int ii = 0; ii < oidCount; ii++) {
    devSnmp_oid *pOID = (devSnmp_oid *) oidList->itemAt(ii);
    if (! pOID) continue;
    pOID->zeroCounters();
  }
}
//--------------------------------------------------------------------
bool devSnmp_group::reportMatchAny(char *match)
{
  if (! match) return(true);  // no match string specified, always matches
  int pvCount = pvList->count();
  for (int ii = 0; ii < pvCount; ii++) {
    devSnmp_pv *pPV = (devSnmp_pv *) pvList->itemAt(ii);
    if (! pPV) continue;
    if (pPV->reportMatchAny(match)) return(true);
  }
  return(false);
}
//--------------------------------------------------------------------
void devSnmp_group::report(int level, char *match)
{
  printf("    %s\n",GROUP_BAR);
  printf("    Group '%s'\n",communityName());
  printf("    %s\n",GROUP_BAR);
  printf("    Host             : %s\n",hostName());
  printf("    Send count       : %ld\n",sendCount);
  printf("    Reply count      : %ld\n",replyCount);
  printf("    Error count      : %ld\n",errorCount);
  printf("    Avg reply time   : %.1lf msec\n",avgReplyMsec);
  printf("    Best reply time  : %ld msec\n",bestReplyMsec);
  printf("    Worst reply time : %ld msec\n",worstReplyMsec);
  printf("    PV count         : %d\n",pvList->count());
  printf("    OID count        : %d\n",oidList->count());
  printf("\n");

  if (level <= 0) {
    // we don't display individual PVs/OIDs, we just aggregate average poll
    // times for sets of OIDs with the same pollMSec
    snmpPollAggregate aggregate;
    int oidCount = oidList->count();
    for (int ii = 0; ii < oidCount; ii++) {
      devSnmp_oid *pOID = (devSnmp_oid *) oidList->itemAt(ii);
      if (! pOID) continue;
      if (pOID->reportMatchAny(match)) aggregate.addOID(pOID);
    }
    aggregate.report();
    printf("\n");
    return;
  }

  // do PVs
  printf("    PVs:\n");
  printf("\n");
  if (level == 1) {
    printf("    desired    actual\n");
    printf("    polls/sec  polls/sec  age (msec)  settings  PV name\n");
    printf("    ---------  ---------  ----------  --------  ----------\n");
  }
  int pvCount = pvList->count();
  for (int ii = 0; ii < pvCount; ii++) {
    devSnmp_pv *pPV = (devSnmp_pv *) pvList->itemAt(ii);
    if (! pPV) continue;
    if (pPV->reportMatchAny(match)) pPV->report(level,match);
  }
  printf("\n");

  // do OIDs
  printf("    OIDs:\n");
  printf("\n");
  if (level == 1) {
    printf("                                      desired    actual\n");
    printf("    polls sent  replies     settings  polls/sec  polls/sec  age (msec)  errors  OID name\n");
    printf("    ----------  ----------  --------  ---------  ---------  ----------  ------  ------------\n");
  }
  int oidCount = oidList->count();
  for (int ii = 0; ii < oidCount; ii++) {
    devSnmp_oid *pOID = (devSnmp_oid *) oidList->itemAt(ii);
    if (! pOID) continue;
    if (pOID->reportMatchAny(match)) pOID->report(level,match);
  }
  printf("\n");
}
//--------------------------------------------------------------------
// class devSnmp_host
//--------------------------------------------------------------------
devSnmp_host::devSnmp_host(devSnmp_manager *pMgr, char *host, bool *okay)
{
  (*okay) = false;  // for now...

  // copy params
  pOurMgr = pMgr;
  hostname = dup_string(host);

  // init variables
  groupList         = new snmpPointerList();
  getQueue          = new snmpPointerList();
  setQueue          = new snmpPointerList();
  activeSessionList = new snmpPointerList();

  // set some defaults (user can override later)
  snmpVersion   = SNMP_VERSION_2c;
  maxOidsPerReq = DEFAULT_MAX_OIDS_PER_REQ;

  (*okay) = true;
}
//--------------------------------------------------------------------
devSnmp_host::~devSnmp_host(void)
{
  // delete any remaining active sessions
  if (activeSessionList) {
    int sessCount = activeSessionList->count();
    for (int ii = sessCount-1; ii >= 0; ii--) {
      devSnmp_session *pTmp = (devSnmp_session *) activeSessionList->removeItemAt(ii);
      if (! pTmp) continue;
      delete pTmp;
    }
    delete activeSessionList;
    activeSessionList = NULL;
  }

  // delete set queue
  if (setQueue) {
    int count = setQueue->count();
    for (int ii = count-1; ii >= 0; ii--) {
      devSnmp_setTransaction *pTmp = (devSnmp_setTransaction *) setQueue->removeItemAt(ii);
      if (! pTmp) continue;
      delete pTmp;
    }
    delete setQueue;
    setQueue = NULL;
  }

  // delete get queue
  if (getQueue) {
    int qCount = getQueue->count();
    for (int ii = qCount-1; ii >= 0; ii--) {
      devSnmp_getTransaction *pTmp = (devSnmp_getTransaction *) getQueue->removeItemAt(ii);
      if (! pTmp) continue;
      delete pTmp;
    }
    delete getQueue;
    getQueue = NULL;
  }

  // delete group list
  if (groupList) {
    int groupCount = groupList->count();
    for (int ii = groupCount-1; ii >= 0; ii--) {
      devSnmp_group *pTmp = (devSnmp_group *) groupList->removeItemAt(ii);
      if (! pTmp) continue;
      delete pTmp;
    }
    delete groupList;
    groupList = NULL;
  }

  // delete host name
  if (hostname) {
    delete hostname;
    hostname = NULL;
  }
}
//--------------------------------------------------------------------
char *devSnmp_host::hostName(void)
{
  return(hostname);
}
//--------------------------------------------------------------------
devSnmp_group *devSnmp_host::findGroup(char *community)
{
  int groupCount = groupList->count();
  devSnmp_group **groupArray = (devSnmp_group **) groupList->rawArray();
  for (int ii = 0; ii < groupCount; ii++) {
    devSnmp_group *pGroup = groupArray[ii];
    if (! pGroup) continue;
    if (strcmp(community, pGroup->communityName()) == 0) return(pGroup);
  }
  return(NULL);
}
//--------------------------------------------------------------------
devSnmp_group *devSnmp_host::createGroup(char *community)
{
  // create group structure
  bool okay;
  devSnmp_group *pGroup = new devSnmp_group(pOurMgr,this,community,&okay);
  if (! pGroup) return(NULL);
  if (! okay) {
    delete pGroup;
    return(NULL);
  }

  // add created group to group list
  groupList->append(pGroup);

  // keep group list sorted
  // (no real reason, just makes reports nicer)
  groupList->sort(devSnmp_group_compare);

  // done
  return(pGroup);
}
//--------------------------------------------------------------------
devSnmp_pv *devSnmp_host::addPV
     (configDataOid    *base,
      configDataPV     *extra,
      OID              *oid,
      struct dbCommon  *pRecord)
{
  // find or create group for this PV
  devSnmp_group *pGroup = findGroup(base->community);
  if (! pGroup) pGroup = createGroup(base->community);
  if (! pGroup) return(NULL);

  // have group add PV
  return( pGroup->addPV(base,extra,oid,pRecord) );
}
//--------------------------------------------------------------------
void devSnmp_host::queueGetTransaction(devSnmp_getTransaction *pTrans)
{
  if (getQueue) getQueue->append(pTrans);
}
//--------------------------------------------------------------------
void devSnmp_host::queueSetTransaction(devSnmp_setTransaction *pTrans)
{
  if (setQueue) {
    /* see if there is already a set transaction for the given oid, if so
       then we can discard this one */
    bool found = false;
    int count = setQueue->count();
    for (int ii = 0; ii < count; ii++) {
      devSnmp_setTransaction *pTmp = (devSnmp_setTransaction *) setQueue->itemAt(ii);
      if (! pTmp) continue;
      if (pTrans->sameOid(pTmp)) {
        delete pTrans;
        found = true;
        break;
      }
    }
    /* no existing set transaction found to overwrite, append this one */
    if (! found) setQueue->append(pTrans);
  }
}
//--------------------------------------------------------------------
int devSnmp_host::getSnmpVersion(void)
{
  return(snmpVersion);
}
//--------------------------------------------------------------------
void devSnmp_host::setSnmpVersion(int version)
{
  snmpVersion = version;
}
//--------------------------------------------------------------------
void devSnmp_host::setMaxOidsPerReq(int maxoids)
{
  // set our local variable
  maxOidsPerReq = (maxoids > 0) ? maxoids : 1;

  // roll through each host/community group, setting max oids for it
  int groupCount = groupList->count();
  for (int group_idx = 0; group_idx < groupCount; group_idx++) {
    devSnmp_group *pGroup = (devSnmp_group *) groupList->itemAt(group_idx);
    if (! pGroup) continue;
    pGroup->setMaxOidsPerReq(maxOidsPerReq);
  }
}
//--------------------------------------------------------------------
int devSnmp_host::getMaxOidsPerReq(void)
{
  return(maxOidsPerReq);
}
//--------------------------------------------------------------------
void devSnmp_host::processing(epicsTimeStamp *pnow)
{
  // roll through each host/community group, having it process
  int groupCount = groupList->count();
  devSnmp_group **groupArray = (devSnmp_group **) groupList->rawArray();
  for (int group_idx = 0; group_idx < groupCount; group_idx++) {
    devSnmp_group *pGroup = groupArray[group_idx];
    if (! pGroup) continue;
    pGroup->processing(pnow);
  }

  //
  // dispose of any completed GET/SET sessions
  //
  // we also dispose of sessions that are older than 60 seconds,
  // which shouldn't happen but we want to make sure they're deleted
  //
  // if we find any sessions that were sent but aren't complete, we're 'busy'
  // (some hosts like Wiener/ISEG crates can't handle simultaneous requests,
  //  so we do all transactions one at a time)
  //
  // we go backwards in list so removing items doesn't mess up our indexing
  //
  bool busy = false;
  int sessionCount = activeSessionList->count();
  devSnmp_session **sessionArray = (devSnmp_session **) activeSessionList->rawArray();
  for (int ii = sessionCount-1; ii >= 0; ii--) {
    devSnmp_session *pSession = sessionArray[ii];
    if (! pSession) continue;
    if ((pSession->isCompleted()) || (pSession->secondsSinceCreated(pnow) > 60)) {
      if ((snmpDebugLevel) && (pSession->secondsSinceCreated(pnow) > 60))
        printf("*** devSnmp: %s deleted stale session\n",hostName());
      activeSessionList->removeItemAt(ii);
      delete pSession;
    } else if (pSession->wasSent()) {
      busy = true;
    }
  }

  // if we're busy, that's all for now
  if (busy) return;

  //
  // not busy if we got this far, pull a transaction out of our queues to send
  //
  devSnmp_transaction *pTrans = NULL;
  if (setQueue->count() > 0) {
    // there are set transactions waiting to go out
    if (getQueue->count() <= 0) {
      // no 'get' transactions waiting, set transaction can go out
      pTrans = (devSnmp_transaction *) setQueue->removeItemAt(0);
    } else {
      // 'get' transactions are waiting, select 'set' transaction only
      // if oldest 'get' transaction hasn't been waiting too long
      devSnmp_transaction *pTopGet = (devSnmp_transaction *) getQueue->itemAt(0);
      if (pTopGet->millisecondsSinceCreated(pnow) < snmpReadStarvationMSec)
        pTrans = (devSnmp_transaction *) setQueue->removeItemAt(0);
      else
        pTrans = (devSnmp_transaction *) getQueue->removeItemAt(0);
    }
  } else if (getQueue->count() > 0) {
    // no 'set' transactions waiting, select next 'get' transaction in queue
    pTrans = (devSnmp_transaction *) getQueue->removeItemAt(0);
  }

  if (pTrans) {
    // create session from transaction object
    devSnmp_session *pSession = pTrans->createSession();

    if (! pSession) {
      // could not create session, nothing else to do here
    } else if (! pSession->send()) {
      // send failed
      snmp_perror("devSnmp_host::processing : session send failed");
      delete pSession;
    } else {
      // send succeeded, add it to active session list
      activeSessionList->append(pSession);
    }

    // delete transaction object, we're done with it now
    delete pTrans;
  }
}
//--------------------------------------------------------------------
void devSnmp_host::sessionRetriesChange(void)
{
  // roll through each host/community group, informing it of global variable change
  int groupCount = groupList->count();
  for (int group_idx = 0; group_idx < groupCount; group_idx++) {
    devSnmp_group *pGroup = (devSnmp_group *) groupList->itemAt(group_idx);
    if (! pGroup) continue;
    pGroup->sessionRetriesChange();
  }
}
//--------------------------------------------------------------------
void devSnmp_host::sessionTimeoutChange(void)
{
  // roll through each host/community group, informing it of global variable change
  int groupCount = groupList->count();
  for (int group_idx = 0; group_idx < groupCount; group_idx++) {
    devSnmp_group *pGroup = (devSnmp_group *) groupList->itemAt(group_idx);
    if (! pGroup) continue;
    pGroup->sessionTimeoutChange();
  }
}
//--------------------------------------------------------------------
void devSnmp_host::zeroCounters(void)
{
  // roll through each host/community group, having it zero
  int groupCount = groupList->count();
  for (int group_idx = 0; group_idx < groupCount; group_idx++) {
    devSnmp_group *pGroup = (devSnmp_group *) groupList->itemAt(group_idx);
    if (! pGroup) continue;
    pGroup->zeroCounters();
  }
}
//--------------------------------------------------------------------
bool devSnmp_host::reportMatchAny(char *match)
{
  if (! match) return(true);  // no match string specified, always matches
  int groupCount = groupList->count();
  for (int group_idx = 0; group_idx < groupCount; group_idx++) {
    devSnmp_group *pGroup = (devSnmp_group *) groupList->itemAt(group_idx);
    if (! pGroup) continue;
    if (pGroup->reportMatchAny(match)) return(true);
  }
  return(false);
}
//--------------------------------------------------------------------
void devSnmp_host::report(int level, char *match)
{
  printf("  %s\n",HOST_BAR);
  printf("  host %s\n",hostName());
  printf("  %s\n",HOST_BAR);

  printf("  SNMP version    : %s\n",snmpVersionString(getSnmpVersion()));
  printf("  Max OIDS/req    : %d\n",getMaxOidsPerReq());
  printf("  Get Queue items : %d\n",getQueue->count());
  printf("  Set Queue items : %d\n",setQueue->count());
  printf("  Active sessions : %d\n",activeSessionList->count());
  printf("\n");

  // roll through each group, having it report if it matches
  int groupCount = groupList->count();
  for (int group_idx = 0; group_idx < groupCount; group_idx++) {
    devSnmp_group *pGroup = (devSnmp_group *) groupList->itemAt(group_idx);
    if (! pGroup) continue;
    if (pGroup->reportMatchAny(match)) pGroup->report(level,match);
  }
}
//--------------------------------------------------------------------
// class devSnmp_manager
//--------------------------------------------------------------------
devSnmp_manager::devSnmp_manager(void)
{
  init_snmp("asynchapp");
#if devSnmp_NETSNMP_VERSION < 50400
  init_mib();
#else
  netsnmp_init_mib();
#endif

  started               = false;
  sendTask_abort        = false;
  readTask_abort        = false;
  sendTask_exited       = false;
  readTask_exited       = false;
  readTask_inSelect     = false;
  readTask_loops        = 0;
  readTask_ignoreBlocks = 0;
  sendTask_loops        = 0;
  readTask_id           = 0;
  sendTask_id           = 0;
  activeRequests        = 0;
  sessionMutex          = epicsMutexCreate();
  snmpHostList          = new snmpPointerList();
}
//--------------------------------------------------------------------
devSnmp_manager::~devSnmp_manager(void)
{
  snmpTimeObject T;

  // stop send task
  int t = 0;
  sendTask_abort = true;
  while ((! sendTask_exited) && (t < 3000)) {
    epicsThreadSleep(0.1);
    t += 100;
  }

  // stop read task
  t = 0;
  readTask_abort = true;
  while ((! readTask_exited) && (t < 3000)) {
    epicsThreadSleep(0.1);
    t += 100;
  }

  // delete host list
  if (snmpHostList) {
    int hostCount = snmpHostList->count();
    for (int ii = hostCount-1; ii >= 0; ii--) {
      devSnmp_host *pTmp = (devSnmp_host *) snmpHostList->removeItemAt(ii);
      if (! pTmp) continue;
      delete pTmp;
    }
    delete snmpHostList;
    snmpHostList = NULL;
  }
}
//--------------------------------------------------------------------
int devSnmp_manager::start(void)
{
  if (started) {
    printf("devSnmp: error, already started\n");
    return(epicsError);
  }

  sendTask_id = epicsThreadCreate("snmpSendTask",
                                 76,
                                 epicsThreadGetStackSize(epicsThreadStackBig),
                                 (EPICSTHREADFUNC) snmpSendTask,
                                 this);

  readTask_id = epicsThreadCreate("snmpReadTask",
                                 77,
                                 epicsThreadGetStackSize(epicsThreadStackBig),
                                 (EPICSTHREADFUNC) snmpReadTask,
                                 this);

  started = true;

  return(epicsOk);
}
//--------------------------------------------------------------------
int devSnmp_manager::readTask(void)
{
  readTask_start.start(NULL);
  readTask_abort = false;
  readTask_exited = false;
  readTask_loops = 0;
  while (! readTask_abort) {
    readTask_loops++;

    sessionMutexLock();
    if (activeRequests) {

      FD_ZERO(&readTask_fdset);
      readTask_fds = 0;
      readTask_block = 1;
      memset(&readTask_timeout,0,sizeof(struct timeval));

      snmp_select_info(&readTask_fds, &readTask_fdset, &readTask_timeout, &readTask_block);
      if (readTask_fds > 0) {
/**/
        if (readTask_block) {
          readTask_ignoreBlocks++;
          readTask_timeout.tv_sec = 2;
          readTask_timeout.tv_usec = 0;
          readTask_block = 0;
        }
/**/
        readTask_inSelect = true;
        readTask_stat = select(readTask_fds, &readTask_fdset, NULL, NULL, readTask_block ? NULL : &readTask_timeout);
        readTask_inSelect = false;

        if (readTask_stat < 0) {
          perror("devSnmp: readTask select() failed");
        } else if (readTask_stat > 0) {
          snmp_read(&readTask_fdset);
        } else {
          snmp_timeout();
        }
      } // fds > 0
    }
    sessionMutexUnlock();

    epicsThreadSleep(snmpThreadSleepMSec / 1000.0);
  }
  readTask_exited = true;
  return(epicsError);
}
//--------------------------------------------------------------------
int devSnmp_manager::sendTask(void)
{
  sendTask_start.start(NULL);
  sendTask_abort = false;
  sendTask_exited = false;
  sendTask_loops = 0;
  while (! sendTask_abort) {
    // do processing
    epicsTimeGetCurrent(&globalLastTick);
    processing(&globalLastTick);

    // wait a bit, so as not to hog CPU
    epicsThreadSleep(snmpThreadSleepMSec / 1000.0);
    sendTask_loops++;
  }
  sendTask_exited = true;
  return(epicsError);
}
//--------------------------------------------------------------------
void devSnmp_manager::sessionMutexLock(void)
{
  epicsMutexLock(sessionMutex);
}
//--------------------------------------------------------------------
void devSnmp_manager::sessionMutexUnlock(void)
{
  epicsMutexUnlock(sessionMutex);
}
//--------------------------------------------------------------------
void devSnmp_manager::incActiveRequests(void)
{
  sessionMutexLock();
  activeRequests++;
  sessionMutexUnlock();
}
//--------------------------------------------------------------------
void devSnmp_manager::decActiveRequests(void)
{
  sessionMutexLock();
  if (activeRequests) activeRequests--;
  sessionMutexUnlock();
}
//--------------------------------------------------------------------
int devSnmp_manager::getHostSnmpVersion(char *host)
{
  devSnmp_host *pHost = findHost(host);
  return( pHost ? pHost->getSnmpVersion() : SNMP_VERSION_2c );
}
//--------------------------------------------------------------------
void devSnmp_manager::setHostSnmpVersion(char *host, char *versionStr)
{
  // parse version
  int vers;
  if (strcmp(versionStr,"SNMP_VERSION_1") == 0)
    vers = SNMP_VERSION_1;
  else if (strcmp(versionStr,"SNMP_VERSION_2") == 0)
    vers = SNMP_VERSION_2c;
  else if (strcmp(versionStr,"SNMP_VERSION_2c") == 0)
    vers = SNMP_VERSION_2c;
  else if (strcmp(versionStr,"SNMP_VERSION_3") == 0)
    vers = SNMP_VERSION_3;
  else
    vers = SNMP_VERSION_2c;

  // locate matching host
  devSnmp_host *pHost = findHost(host);

  // if host does not exist, create it
  if (! pHost) pHost = createHost(host);

  // set version for host
  if (pHost) pHost->setSnmpVersion(vers);
}
//--------------------------------------------------------------------
void devSnmp_manager::setMaxOidsPerReq(char *host, int maxoids)
{
  if (maxoids < 1) maxoids = 1;

  // locate matching host
  devSnmp_host *pHost = findHost(host);

  // if host does not exist, create it
  if (! pHost) pHost = createHost(host);

  // set max oids for host
  if (pHost) pHost->setMaxOidsPerReq(maxoids);
}
//--------------------------------------------------------------------
int devSnmp_manager::getHostMaxOidsPerReq(char *host)
{
  // locate matching host
  devSnmp_host *pHost = findHost(host);
  return( pHost ? pHost->getMaxOidsPerReq() : DEFAULT_MAX_OIDS_PER_REQ );
}
//--------------------------------------------------------------------
devSnmp_pv *devSnmp_manager::addPV(struct dbCommon *pRec, struct link *pLink)
{
  // parse INP/OUT line
  char *instioStr = pLink->value.instio.string;
  char *cp, *mp;
  configDataOid base;
  configDataPV extra;
  cp = instioStr;
  while (((mp=strstr(cp, "%(")) != NULL) || ((mp=strstr(cp, "%{")) != NULL)) {
    *mp = '$';
    cp = mp;
    }
  instioStr = macEnvExpand(instioStr);
  if (! snmpParseInOut(instioStr,&base,&extra)) return(NULL);

  // validate OID text, fill in OID structure
  OID oid;
  oid.Name   = dup_string(base.oidStr);
  oid.OidLen = MAX_OID_LEN;
  if (! get_node(oid.Name, oid.Oid, &oid.OidLen)) {
    if (! read_objid(oid.Name,oid.Oid,&oid.OidLen)) {
      printf("devSnmp: error parsing %s '%s'\n",pRec->name,instioStr);
      printf("         OID '%s' read_objid failed\n",oid.Name);
      snmp_perror("devSnmp_manager::addPV");
      return(NULL);
    }
  }

  // find or create host object for this OID
  devSnmp_host *pHost = findHost(base.host);
  if (! pHost) pHost = createHost(base.host);
  if (! pHost) return(NULL);

  // add PV to this host
  devSnmp_pv *pPV = pHost->addPV(&base,&extra,&oid,pRec);

  // point record's dpvt at created PV object
  pRec->dpvt = pPV;

  // and we're done
  return(pPV);
}
//--------------------------------------------------------------------
devSnmp_host *devSnmp_manager::createHost(char *host)
{
  // create host structure
  bool okay;
  devSnmp_host *pHost = new devSnmp_host(this,host,&okay);
  if (! pHost) return(NULL);
  if (! okay) {
    delete pHost;
    return(NULL);
  }

  // add created host to host list
  snmpHostList->append(pHost);

  // keep host list sorted
  // (no real reason, just makes reports nicer)
  snmpHostList->sort(devSnmp_host_compare);

  // done
  return(pHost);
}
//--------------------------------------------------------------------
devSnmp_host *devSnmp_manager::findHost(char *host)
{
  int hostCount = snmpHostList->count();
  devSnmp_host **hostArray = (devSnmp_host **) snmpHostList->rawArray();
  for (int ii = 0; ii < hostCount; ii++) {
    devSnmp_host *pHost = hostArray[ii];
    if (! pHost) continue;
    if (strcmp(host, pHost->hostName()) == 0) return(pHost);
  }
  return(NULL);
}
//--------------------------------------------------------------------
void devSnmp_manager::processing(epicsTimeStamp *pnow)
{
  // roll through each host, having it process
  int hostCount = snmpHostList->count();
  devSnmp_host **hostArray = (devSnmp_host **) snmpHostList->rawArray();
  for (int host_idx = 0; host_idx < hostCount; host_idx++) {
    devSnmp_host *pHost = hostArray[host_idx];
    if (! pHost) continue;
    pHost->processing(pnow);
  }
}
//--------------------------------------------------------------------
void devSnmp_manager::sessionRetriesChange(void)
{
  // roll through each host, having informing it global variable has changed
  int hostCount = snmpHostList->count();
  for (int host_idx = 0; host_idx < hostCount; host_idx++) {
    devSnmp_host *pHost = (devSnmp_host *) snmpHostList->itemAt(host_idx);
    if (! pHost) continue;
    pHost->sessionRetriesChange();
  }
}
//--------------------------------------------------------------------
void devSnmp_manager::sessionTimeoutChange(void)
{
  // roll through each host, having informing it global variable has changed
  int hostCount = snmpHostList->count();
  for (int host_idx = 0; host_idx < hostCount; host_idx++) {
    devSnmp_host *pHost = (devSnmp_host *) snmpHostList->itemAt(host_idx);
    if (! pHost) continue;
    pHost->sessionTimeoutChange();
  }
}
//--------------------------------------------------------------------
void devSnmp_manager::zeroCounters(void)
{
  // clear task counters
  readTask_start.start(NULL);
  readTask_loops = 0;
  readTask_ignoreBlocks = 0;
  sendTask_start.start(NULL);
  sendTask_loops = 0;

  // roll through each host, having it zero counters
  int hostCount = snmpHostList->count();
  for (int host_idx = 0; host_idx < hostCount; host_idx++) {
    devSnmp_host *pHost = (devSnmp_host *) snmpHostList->itemAt(host_idx);
    if (! pHost) continue;
    pHost->zeroCounters();
  }
}
//--------------------------------------------------------------------
bool devSnmp_manager::reportMatchAny(char *match)
{
  if (! match) return(true);  // no match string specified, always matches
  int hostCount = snmpHostList->count();
  for (int host_idx = 0; host_idx < hostCount; host_idx++) {
    devSnmp_host *pHost = (devSnmp_host *) snmpHostList->itemAt(host_idx);
    if (! pHost) continue;
    if (pHost->reportMatchAny(match)) return(true);
  }
  return(false);
}
//--------------------------------------------------------------------
void devSnmp_manager::report(int level, char *match)
{
  if ((! snmpHostList) || (snmpHostList->count() == 0)) {
    printf("no devSnmp hosts defined\n");
    return;
  }

  if ((match) && (! reportMatchAny(match))) {
    printf("no PV/OID names contain '%s'\n",match);
    return;
  }

  printf("%s\n",MANAGER_BAR);
  printf("devSnmp report\n");
  printf("%s\n",MANAGER_BAR);

  // show if tasks have exited
  if (readTask_exited) printf("*** READ TASK HAS EXITED !!!\n");
  if (sendTask_exited) printf("*** SEND TASK HAS EXITED !!!\n");

  // show task counter data
  double r_sec = readTask_start.elapsedSeconds(NULL);
  double r_loopps = (r_sec < 0.01) ? 0.0 : (readTask_loops / r_sec);
  double s_sec = sendTask_start.elapsedSeconds(NULL);
  double s_loopps = (s_sec < 0.01) ? 0.0 : (sendTask_loops / s_sec);
  printf("read task loops : %ld (%.1lf per sec)\n",readTask_loops,r_loopps);
  printf("send task loops : %ld (%.1lf per sec)\n",sendTask_loops,s_loopps);
  printf("active requests : %d\n",activeRequests);
  printf("\n");

  // roll through each host, having it report if it matches
  int hostCount = snmpHostList->count();
  for (int host_idx = 0; host_idx < hostCount; host_idx++) {
    devSnmp_host *pHost = (devSnmp_host *) snmpHostList->itemAt(host_idx);
    if (! pHost) continue;
    if (pHost->reportMatchAny(match)) pHost->report(level,match);
  }

  printf("verifying devSnmp tasks are running...\n");

  unsigned long read_ticks = readTask_loops;
  unsigned long send_ticks = sendTask_loops;

  int M = 0;
  do {
    epicsThreadSleep(0.1);
    M += 100;
  } while ((M < 2000) && ((read_ticks == readTask_loops) || (send_ticks == sendTask_loops)));

  bool showSendDetail = (level > 0);
  if (sendTask_loops != send_ticks) {
    printf("  sendTask is alive\n");
  } else {
    printf("  *** sendTask appears dead/hung\n");
    showSendDetail = true;
  }
  if (showSendDetail) {
    char tmp[128];
    snmpTimeObject T;
    T.start(&globalLastTick);
    T.toDateTimeString(tmp);
    printf("      sendTask_abort  : %d\n",sendTask_abort);
    printf("      sendTask_exited : %d\n",sendTask_exited);
    printf("      globalLastTick  : %s\n",tmp);
  }

  bool showReadDetail = (level > 0);
  if (readTask_loops != read_ticks)
    printf("  readTask is alive\n");
  else {
    printf("  *** readTask appears dead/hung\n");
    showReadDetail = true;
  }
  if (showReadDetail) {
    printf("      readTask_abort        : %d\n",readTask_abort);
    printf("      readTask_exited       : %d\n",readTask_exited);
    printf("      readTask_inSelect     : %d\n",readTask_inSelect);
    printf("      readTask_fds          : %d\n",readTask_fds);
    printf("      readTask_block        : %d\n",readTask_block);
    printf("      readTask_ignoreBlocks : %ld\n",readTask_ignoreBlocks);
    printf("      readTask_timeout      : %ld sec %ld usec\n",
           (long) readTask_timeout.tv_sec,
           (long) readTask_timeout.tv_usec);
  }
}
//--------------------------------------------------------------------
// class snmpPollAggregate
//--------------------------------------------------------------------
snmpPollAggregate::snmpPollAggregate(void)
{
  lists = new snmpPointerList();
}
//--------------------------------------------------------------------
snmpPollAggregate::~snmpPollAggregate(void)
{
  if (lists) {
    int listcc = lists->count();
    for (int ii = listcc-1; ii >= 0; ii--) {
      snmpPointerList *pList = (snmpPointerList *) lists->removeItemAt(ii);
      if (! pList) continue;
      delete pList;
    }
    delete lists;
    lists = NULL;
  }
}
//--------------------------------------------------------------------
void snmpPollAggregate::addOID(devSnmp_oid *pOID)
{
  unsigned long msec = pOID->getPollMSec();
  snmpPointerList *pList = findList(msec);
  if (! pList) {
    pList = new snmpPointerList();
    pList->setContext((void *)msec);
    lists->append(pList);
  }
  pList->append(pOID);
}
//--------------------------------------------------------------------
snmpPointerList *snmpPollAggregate::findList(unsigned long msec)
{
  int listcc = lists->count();
  for (int ii = 0; ii < listcc; ii++) {
    snmpPointerList *pList = (snmpPointerList *) lists->itemAt(ii);
    if (! pList) continue;
    unsigned long thisctxt = (unsigned long) pList->context();
    if (thisctxt == msec) return(pList);
  }
  return(NULL);
}
//--------------------------------------------------------------------
void snmpPollAggregate::report(void)
{
  if ((! lists) || (lists->count() == 0)) return;
  lists->sort(list_context_compare);
  printf("    Poll rate  OIDs  Avg rate  Variation\n");
  printf("    ---------  ----  --------  ---------\n");
  int listcc = lists->count();
  for (int ii = 0; ii < listcc; ii++) {
    snmpPointerList *pList = (snmpPointerList *) lists->itemAt(ii);
    if (! pList) continue;
    unsigned long msec = (unsigned long) pList->context();
    double rate = (msec == 0) ? 0.0 : (1000.0 / msec);
    double tot = 0.0;
    double ppsMin = 0.0;
    double ppsMax = 0.0;
    int realCount = 0;
    int plistcc = pList->count();
    for (int jj = 0; jj < plistcc; jj++) {
      devSnmp_oid *pOID = (devSnmp_oid *) pList->itemAt(jj);
      if (! pOID) continue;
      double thisPPS = pOID->pollsPerSecond();
      tot += thisPPS;
      if ((realCount == 0) || (thisPPS < ppsMin)) ppsMin = thisPPS;
      if ((realCount == 0) || (thisPPS > ppsMax)) ppsMax = thisPPS;
      realCount++;
    }
    double avgr = (realCount) ? (tot / ((double) realCount)) : 0.0;
    printf("    %9.3lf  %4d  %8.3lf  %9.3lf\n",rate,plistcc,avgr,ppsMax-ppsMin);
  }
}
//--------------------------------------------------------------------

//--------------------------------------------------------------------
// device support definition structures
//--------------------------------------------------------------------

#include <aiRecord.h>
#include <stringinRecord.h>
#include <longinRecord.h>
#include <waveformRecord.h>
#include <aoRecord.h>
#include <stringoutRecord.h>
#include <longoutRecord.h>
#include <snmpMSURecord.h>

extern "C" {
  static long snmpAiInit(struct aiRecord *pai);
  static long snmpAiRead(struct aiRecord *pai);
  static long snmpLiInit(struct longinRecord *pli);
  static long snmpLiRead(struct longinRecord *pli);
  static long snmpSiInit(struct stringinRecord *psi);
  static long snmpSiRead(struct stringinRecord *psi);
  static epicsStatus snmpWfInit(struct waveformRecord *pwf);
  static epicsStatus snmpWfRead(struct waveformRecord *pwf);
  static long snmpAoInit(struct aoRecord *pao);
  static long snmpAoWrite(struct aoRecord *pao);
  static long snmpAoReadback(devSnmp_pv *pPV);
  static long snmpLoInit(struct longoutRecord *plo);
  static long snmpLoWrite(struct longoutRecord *plo);
  static long snmpLoReadback(devSnmp_pv *pPV);
  static long snmpSoInit(struct stringoutRecord *pso);
  static long snmpSoWrite(struct stringoutRecord *pso);
  static long snmpSoReadback(devSnmp_pv *pPV);
//--------------------------------------------------------------------
//--------------------------------------------------------------------

  static long snmpSoftInit(struct snmpRecord *psnmp);
  static long snmpSoftWrite(struct snmpRecord *psnmp);
  static long snmpSoftReadback(devSnmp_pv *pPV);
//--------------------------------------------------------------------
  static long snmpstrSoftInit(struct snmpstrRecord *psnmpstr);
  static long snmpstrSoftWrite(struct snmpstrRecord *psnmpstr);
  static long snmpstrSoftReadback(devSnmp_pv *pPV);
//--------------------------------------------------------------------
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------

// snmp
static struct {
  long            number;
  DEVSUPFUN       report;
  DEVSUPFUN       init;
  DEVSUPFUN       init_record;
  DEVSUPFUN       ioIntInfoGet;
  DEVSUPFUN       write_snmp;
  DEVSUPFUN       special_linconv;
} devSnmpSoft = {
  6,
  NULL,
  NULL,
  (DEVSUPFUN) snmpSoftInit,
  NULL,
  (DEVSUPFUN) snmpSoftWrite,
  NULL
};
epicsExportAddress(dset,devSnmpSoft);
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------

// snmpstr
static struct {
  long       number;
  DEVSUPFUN  report;
  DEVSUPFUN  init;
  DEVSUPFUN  init_record;
  DEVSUPFUN  ioIntInfoGet;
  DEVSUPFUN  write_snmpstr;
  DEVSUPFUN  special_linconv;
} devSnmpstrSoft = {
  6,
  NULL,
  NULL,
  (DEVSUPFUN) snmpstrSoftInit,
  NULL,
  (DEVSUPFUN) snmpstrSoftWrite,
  NULL
};
epicsExportAddress(dset,devSnmpstrSoft);

//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------


// ai
static struct {
  long            number;
  DEVSUPFUN       report;
  DEVSUPFUN       init;
  DEVSUPFUN       init_record;
  DEVSUPFUN       ioIntInfoGet;
  DEVSUPFUN       read_ai;
  DEVSUPFUN       special_linconv;
} devSnmpAi = {
  6,
  NULL,
  NULL,
  (DEVSUPFUN) snmpAiInit,
  NULL,
  (DEVSUPFUN) snmpAiRead,
  NULL
};
epicsExportAddress(dset,devSnmpAi);

// longin
static struct {
  long            number;
  DEVSUPFUN       report;
  DEVSUPFUN       init;
  DEVSUPFUN       init_record;
  DEVSUPFUN       ioIntInfoGet;
  DEVSUPFUN       read_li;
  DEVSUPFUN       special_linconv;
} devSnmpLi = {
  6,
  NULL,
  NULL,
  (DEVSUPFUN) snmpLiInit,
  NULL,
  (DEVSUPFUN) snmpLiRead,
  NULL
};
epicsExportAddress(dset,devSnmpLi);


// stringin
static struct {
  long            number;
  DEVSUPFUN       report;
  DEVSUPFUN       init;
  DEVSUPFUN       init_record;
  DEVSUPFUN       ioIntInfoGet;
  DEVSUPFUN       read_si;
  DEVSUPFUN       special_linconv;
} devSnmpSi = {
  6,
  NULL,
  NULL,
  (DEVSUPFUN) snmpSiInit,
  NULL,
  (DEVSUPFUN) snmpSiRead,
  NULL
};
epicsExportAddress(dset,devSnmpSi);

// wave form
static struct {
  long            number;
  DEVSUPFUN       report;
  DEVSUPFUN       init;
  DEVSUPFUN       init_record;
  DEVSUPFUN       ioIntInfoGet;
  DEVSUPFUN       read_wf;
  DEVSUPFUN       special_linconv;
} devSnmpWf = {
  6,
  NULL,
  NULL,
  (DEVSUPFUN) snmpWfInit,
  NULL,
  (DEVSUPFUN) snmpWfRead,
  NULL
};
epicsExportAddress(dset,devSnmpWf);

// ao
static struct {
  long       number;
  DEVSUPFUN  report;
  DEVSUPFUN  init;
  DEVSUPFUN  init_record;
  DEVSUPFUN  ioIntInfoGet;
  DEVSUPFUN  write_ao;
  DEVSUPFUN  special_linconv;
} devSnmpAo = {
  6,
  NULL,
  NULL,
  (DEVSUPFUN) snmpAoInit,
  NULL,
  (DEVSUPFUN) snmpAoWrite,
  NULL
};
epicsExportAddress(dset,devSnmpAo);

// longout
static struct {
  long       number;
  DEVSUPFUN  report;
  DEVSUPFUN  init;
  DEVSUPFUN  init_record;
  DEVSUPFUN  ioIntInfoGet;
  DEVSUPFUN  write_lo;
  DEVSUPFUN  special_linconv;
} devSnmpLo = {
  6,
  NULL,
  NULL,
  (DEVSUPFUN) snmpLoInit,
  NULL,
  (DEVSUPFUN) snmpLoWrite,
  NULL
};
epicsExportAddress(dset,devSnmpLo);

// stringout
static struct {
  long       number;
  DEVSUPFUN  report;
  DEVSUPFUN  init;
  DEVSUPFUN  init_record;
  DEVSUPFUN  ioIntInfoGet;
  DEVSUPFUN  write_so;
  DEVSUPFUN  special_linconv;
} devSnmpSo = {
  6,
  NULL,
  NULL,
  (DEVSUPFUN) snmpSoInit,
  NULL,
  (DEVSUPFUN) snmpSoWrite,
  NULL
};
epicsExportAddress(dset,devSnmpSo);

extern "C" {
  typedef struct snmpTypeConvStruct
  {
     char *str;
     int (*func)(void*, char*);
     size_t size;
  } SNMP_TYPE_CONV;

  static epicsStatus snmpWfCntr32Convert(void *rval, char *sval);
  static epicsStatus snmpWfCntr64Convert(void *rval, char *sval);
  static epicsStatus snmpWfGauge32Convert(void *rval, char *sval);
  static epicsStatus snmpWfGauge64Convert(void *rval, char *sval);
  static epicsStatus snmpWfTimeTicksConvert(void *rval, char *sval);
  static epicsStatus snmpWfHexStrConvert(void *rval, char *sval);
  static epicsStatus snmpWfNetAddrConvert(void *rval, char *sval);
  static epicsStatus snmpWfIntConvert(void *rval, char *sval);
  static epicsStatus snmpWfStrConvert(void *rval, char *sval);
  static epicsStatus snmpWfIpAddrConvert(void *rval, char *sval);

  SNMP_TYPE_CONV wfTypes[] =
  {
     { (char *) "Counter32",       snmpWfCntr32Convert,    sizeof(unsigned int)  },
     { (char *) "Counter64",       snmpWfCntr64Convert,    sizeof(SNMP_UINT64)   },
     { (char *) "Gauge32",         snmpWfGauge32Convert,   sizeof(unsigned int)  },
     { (char *) "Gauge64",         snmpWfGauge64Convert,   sizeof(SNMP_UINT64)   },
     { (char *) "Timeticks",       snmpWfTimeTicksConvert, sizeof(unsigned long) },
     { (char *) "Hex-STRING",      snmpWfHexStrConvert,    (size_t) 0            },
     { (char *) "Network Address", snmpWfNetAddrConvert,   (size_t) 0            },
     { (char *) "INTEGER",         snmpWfIntConvert,       sizeof(int)           },
     { (char *) "STRING",          snmpWfStrConvert,       (size_t) 0            },
     { (char *) "IpAddress",       snmpWfIpAddrConvert,    (size_t) 0            },
     { NULL,              NULL,                   (size_t) 0            }
  };
}  // extern "C"

//--------------------------------------------------------------------
// routines accessible from ioc shell
//--------------------------------------------------------------------
int epicsSnmpInit(int param)
{
  if (! checkInit()) return(epicsError);
  printf("(epicsSnmpInit() is obsolete, init now done automatically via initHooks)\n");
  return(epicsOk);
}
//--------------------------------------------------------------------
int devSnmpSetSnmpVersion(char *hostName, char *versionStr)
{
  if (! checkInit()) return(epicsError);
  pManager->setHostSnmpVersion(hostName,versionStr);
  return(epicsOk);
}
//--------------------------------------------------------------------
int devSnmpSetMaxOidsPerReq(char *hostName, int maxoids)
{
  if (! checkInit()) return(epicsError);
  pManager->setMaxOidsPerReq(hostName,maxoids);
  return(epicsOk);
}
//--------------------------------------------------------------------
int devSnmpSetParam(const char *param, int value)
{
  if (! checkInit()) return(epicsError);
  if ((param == NULL) || (param[0] == 0)) {
    // no argument, show current values
    int longest = 0;
    for (int ii = 0; setParamTable[ii].paramName != NULL; ii++) {
      int thisLen = strlen(setParamTable[ii].paramName);
      if (thisLen > longest) longest = thisLen;
    }
    printf("\ndevSnmp settable parameters\n\n");
    printf("%-*s  value\n",longest,"parameter name");
    char bar1[longest+1];
    memset(bar1,'-',longest);
    bar1[longest] = 0;
    printf("%s  -----\n",bar1);
    for (int ii = 0; setParamTable[ii].paramName != NULL; ii++) {
      printf("%-*s  %d\n",longest,setParamTable[ii].paramName,(*setParamTable[ii].valuePointer));
    }
    printf("\n");
  } else {
    bool found = false;
    for (int ii = 0; setParamTable[ii].paramName != NULL; ii++) {
      if (strcmp(setParamTable[ii].paramName,param) == 0) {
        found = true;
        int oldVal = (*setParamTable[ii].valuePointer);
        (*setParamTable[ii].valuePointer) = value;
        if (setParamTable[ii].changeFunc) (*setParamTable[ii].changeFunc)();
        printf("devSnmp set %s to %d (was %d)\n",param,value,oldVal);
        break;
      }
    }
    if (! found) printf("devSnmp error: no such settable parameter '%s'\n",param);
  }
  return(epicsOk);
}
//----------------------------------------------------------------------
int devSnmpSetDebug(int level)
{
  if (! checkInit()) return(epicsError);
  devSnmpSetParam("DebugLevel",level);
  printf("(devSnmpSetDebug deprecated, use devSnmpSetParam(\"DebugLevel\",%d) instead\n",level);
  return(epicsOk);
}
//--------------------------------------------------------------------
int snmpr(int level, char *match)
{
  if (! checkInit()) return(epicsError);
  pManager->report(level,match);
  return(epicsOk);
}
//--------------------------------------------------------------------
int snmpz(void)
{
  if (! checkInit()) return(epicsError);
  pManager->zeroCounters();
  return(epicsOk);
}
//--------------------------------------------------------------------
int snmpzr(int level, char *match)
{
  if (! checkInit()) return(epicsError);
  snmpz();
  printf("waiting 10 seconds for counts to rack up...\n");
  epicsThreadSleep(10.0);
  return( snmpr(level,match) );
}
//--------------------------------------------------------------------
// record processing routines
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------

static long snmpSoftInit(struct snmpRecord *psnmp)
{
  // snmp.scan must be Passive
  //if (psnmp->scan != menuScanPassive) {
  //  recGblRecordError(S_db_badField,(void *)psnmp,
  //                    "devSnmpSoft (init_record) SCAN must be Passive");
  //  return(S_db_badField);
  //}

  // snmp.out must be an INST_IO
  switch (psnmp->out.type) {
  case INST_IO:
    break;

  default :
    recGblRecordError(S_db_badField,(void *)psnmp,
                      "devSnmpSoft (init_record) Illegal OUT field");
    return(S_db_badField);
  }

  if (! checkInit()) return(epicsError);
  devSnmp_pv *pPV = pManager->addPV((struct dbCommon *) psnmp, &psnmp->out);
  if (! pPV) {
    recGblRecordError(S_db_badField,
                      (void *)psnmp,"devSnmpSoft (init_record) bad parameters");
    return(S_db_badField);
  }
  pPV->setPollMSec(snmpPassivePollMSec);

  /* request a periodic call to snmpSoftReadback, so our display value is
     periodically updated with value from the remote host */
  pPV->setPeriodicCallback(snmpSoftReadback,(snmpPassivePollMSec / 1000.0));

  psnmp->udf = false;
  return(epicsOk);
}
//--------------------------------------------------------------------
static long snmpSoftWrite(struct snmpRecord *psnmp)
{
  devSnmp_pv *pPV;

  if (psnmp->dpvt == NULL)
    return(epicsError);
  else
    pPV = (devSnmp_pv *) psnmp->dpvt;

  /* if we're being called as a result of a readback update,
     just report success */
  if (pPV->doingProcess()) return(epicsOk);

  // test to make sure value is valid
  if (isnan(psnmp->val)) {
    if (snmpDebugLevel)
      printf("----- devSnmp ERROR: Soft setting %s to invalid number!\n",psnmp->name);
    return(epicsError);
  }

  // fill in set data
  // guard against negative zero, it annoys some devices
  if (strstr(psnmp->desc,"SETDBG")) pPV->debugSetStart();
  double outv = psnmp->val;
  if (local_isNegativeZero(outv)) outv = 0.0;
  long status = epicsOk;
  char tmp[40];
  sprintf(tmp,"%lg",outv);
  pPV->set(tmp);
  if (snmpDebugLevel)
    printf("----- devSnmp Soft setting %s to [%s] ...\n",psnmp->name,tmp);

  return(status);
}
//--------------------------------------------------------------------
static long snmpSoftReadback(devSnmp_pv *pPV)
{
  struct snmpRecord *psnmp = (struct snmpRecord *) pPV->record();
  epicsStatus status = epicsError;
  double new_val;
  bool process_record = false;

  status = -1;

  if (pPV->getValueDouble(&new_val)) {
    status = 2;

    /* if it hasn't been long enough since last setting sent, skip
       checking versus our current val field (don't want our val
       field over-written if pPV hasn't had time to process
       setting yet) */
    if (! pPV->wasSetRecently()) {
      /* we use a different-enough function rather than directly
         comparing the old/new values to avoid being triggered often
         due to meaningless noise */
      if (differentEnough(psnmp->val,new_val)) {
        // value read from remote host doesn't match what we think val is
        if (snmpDebugLevel)
          printf("----- devSnmp Soft value change %s [%lg] -> [%lg] ...\n",psnmp->name,psnmp->val,new_val);

          //psnmp->val = new_val;
         //++silee
         //menucalc
        psnmp->rval = new_val;
        char oval[40]; 
        char sval[40]; //percent
        switch(psnmp->mjp)
        {
                case menu_Plus: 
                                psnmp->val = ceil(psnmp->rval+psnmp->oval*psnmp->sval);
                break;
                case menu_Minus: 
                                psnmp->val = ceil(psnmp->rval-psnmp->oval*psnmp->sval);
                break;
                case menu_Mul: 
                                psnmp->val = ceil(psnmp->rval*psnmp->oval*psnmp->sval);
                break;
                case menu_Div: 
                                psnmp->val = ceil(psnmp->rval/psnmp->oval*psnmp->sval);
                break;
        }


        process_record = true;
      }
    }


    // need to clear invalid alarm if present
    if (psnmp->sevr == INVALID_ALARM) {
      recGblResetAlarms(psnmp);
      process_record = true;
    }

  } else {
    status = recGblSetSevr(psnmp, READ_ALARM, INVALID_ALARM);
    if (status && (psnmp->stat != READ_ALARM || psnmp->sevr != INVALID_ALARM)) {
      // trigger processing so stat/sevr is updated
      process_record = true;
      if (snmpDebugLevel) printf("%s  devSnmp Soft read error '%s' : %s\n",tnow(),psnmp->name,pPV->errorString());
    }
    status = -1;
  }

  // process record if needed
  if (process_record) pPV->processRecord();

  return(status);
}

//--------------------------------------------------------------------
//--------------------------------------------------------------------


static long snmpstrSoftInit(struct snmpstrRecord *psnmpstr)
{
  // so.scan must be Passive
  //if (psnmpstr->scan != menuScanPassive) {
  //  recGblRecordError(S_db_badField,(void *)psnmpstr,
  //                    "devSnmpstrSoft (init_record) SCAN must be Passive");
  //  return(S_db_badField);
  //}

  // so.out must be an INST_IO
  switch (psnmpstr->out.type) {
  case INST_IO:
    break;

  default :
    recGblRecordError(S_db_badField,(void *)psnmpstr,
                      "devSnmpstrSoft (init_record) Illegal OUT field");
    return(S_db_badField);
  }

  if (! checkInit()) return(epicsError);
  devSnmp_pv *pPV = pManager->addPV((struct dbCommon *) psnmpstr, &psnmpstr->out);
  if (! pPV) {
    recGblRecordError(S_db_badField,
                      (void *)psnmpstr,"devSnmpstrSoft (init_record) bad parameters");
    return(S_db_badField);
  }
  pPV->setPollMSec(snmpPassivePollMSec);

  /* request a periodic call to snmpstrSoftReadback, so our display value is
     periodically updated with value from the remote host */
  pPV->setPeriodicCallback(snmpstrSoftReadback,(snmpPassivePollMSec / 1000.0));

  psnmpstr->udf = false;
  return(epicsOk);
}

//--------------------------------------------------------------------
static long snmpstrSoftWrite(struct snmpstrRecord *psnmpstr)
{
  devSnmp_pv *pPV;

  if (psnmpstr->dpvt == NULL)
    return(epicsError);
  else
    pPV = (devSnmp_pv *) psnmpstr->dpvt;

  /* if we're being called as a result of a readback update,
     just report success */
  if (pPV->doingProcess()) return(epicsOk);

  // fill in set data
  long status = epicsOk;
  pPV->set(psnmpstr->val);

  if (snmpDebugLevel)
    printf("----- devSnmpstr Soft setting %s to [%s] ...\n",psnmpstr->name,psnmpstr->val);

  return(status);
}




//--------------------------------------------------------------------
  static long snmpstrSoftReadback(devSnmp_pv *pPV)
{
  struct snmpstrRecord *psnmpstr = (struct snmpstrRecord *) pPV->record();
  epicsStatus status = epicsError;
  char val[40];
  char sval[40];
//  char new_val[40];
  bool process_record = false;

  status = -1;
  //std::string sval; //c++
  if (pPV->getValueString(val,sizeof(val))) {
    //strcpy(new_val,val);
    //sval = val;
    status = 2;
    int j = 0;
    for(int i = 0; i < strlen(val); i++)  //c 
{
      char cmp;
      cmp = val[i];
      if ( cmp == '"' ) continue;

      sval[j] = cmp;
      j++;   
}
     sval[j] = '\0';

    /* if it hasn't been long enough since last setting sent, skip
       checking versus our current val field (don't want our val
       field over-written if pPV hasn't had time to process
       setting yet) */
    if (! pPV->wasSetRecently()) {
      if (strcmp(sval,psnmpstr->val)) {
        // value read from remote host doesn't match what we think val is
        if (snmpDebugLevel)
          printf("----- devSnmpstr Soft value change %s [%s] -> [%s] ...\n",psnmpstr->name,psnmpstr->val,sval);
        strcpy(psnmpstr->val,sval);
        //psnmpstr->val = sval.c_str();
        process_record = true;
      }
    }

    // need to clear invalid alarm if present
    if (psnmpstr->sevr == INVALID_ALARM) {
      recGblResetAlarms(psnmpstr);
      process_record = true;
    }
  } else {
    status = recGblSetSevr(psnmpstr, READ_ALARM, INVALID_ALARM);
    if (status && (psnmpstr->stat != READ_ALARM || psnmpstr->sevr != INVALID_ALARM)) {
      // trigger processing so stat/sevr is updated
      strcpy(psnmpstr->val, "INVALID");
      process_record = true;
      if (snmpDebugLevel) printf("%s  devSnmpstr Soft read error '%s' : %s\n",tnow(),psnmpstr->name,pPV->errorString());
    }
    status = -1;
  }

  // process record if needed
  if (process_record) pPV->processRecord();

  return(status);
}










//--------------------------------------------------------------------
//--------------------------------------------------------------------
//--------------------------------------------------------------------



static long snmpAiInit(struct aiRecord *pai)
{
  // ai.inp must be an INST_IO
  switch (pai->inp.type) {
    case INST_IO:
        break;

    default :
        recGblRecordError(S_db_badField,(void *)pai,
                         "devSnmpAi (init_record) Illegal INP field");
        return(S_db_badField);
  }

  if (! checkInit()) return(epicsError);
  devSnmp_pv *pPV = pManager->addPV((struct dbCommon *) pai, &pai->inp);
  if (! pPV) {
    recGblRecordError(S_db_badField,
                      (void *)pai,"devSnmpAi (init_record) bad parameters");
    return(S_db_badField);
  }

  pai->udf = false;
  return(epicsOk);
}
//--------------------------------------------------------------------
static long snmpAiRead(struct aiRecord *pai)
{
  devSnmp_pv *pPV;
  epicsStatus status = epicsError;
  double new_val;

  if (pai->dpvt == NULL)
    return(epicsError);
  else
    pPV = (devSnmp_pv *) pai->dpvt;

  status = -1;

  if (pPV->getValueDouble(&new_val)) {
    pai->val = new_val;
    status = 2;
  } else {
#ifdef AI_ON_READ_ERROR_VALUE
    pai->val = AI_ON_READ_ERROR_VALUE;
#endif
    status = recGblSetSevr(pai, READ_ALARM, INVALID_ALARM);
    if (status && (pai->stat != READ_ALARM || pai->sevr != INVALID_ALARM)) {
      if (snmpDebugLevel) printf("%s  devSnmp AI read error '%s' : %s\n",tnow(),pai->name,pPV->errorString());
    }
    status = -1;
  }

  return(status);
}
//--------------------------------------------------------------------
static long snmpLiInit(struct longinRecord *pli)
{
  // ai.inp must be an INST_IO
  switch (pli->inp.type) {
  case INST_IO:
    break;

  default :
    recGblRecordError(S_db_badField,(void *)pli,
                      "devSnmpLi (init_record) Illegal INP field");
    return(S_db_badField);
  }

  if (! checkInit()) return(epicsError);
  devSnmp_pv *pPV = pManager->addPV((struct dbCommon *) pli, &pli->inp);
  if (! pPV) {
    recGblRecordError(S_db_badField,
                      (void *)pli,"devSnmpLi (init_record) bad parameters");
    return(S_db_badField);
  }

  pli->udf = false;
  return(epicsOk);
}
//--------------------------------------------------------------------
static long snmpLiRead(struct longinRecord *pli)
{
  devSnmp_pv  *pPV;
  epicsStatus status = epicsError;
  long new_val;

  if (pli->dpvt == NULL)
    return(epicsError);
  else
    pPV = (devSnmp_pv *) pli->dpvt;

  status = 2;

  if (pPV->getValueLong(&new_val)) {
    pli->val = new_val;
    status = epicsOk;
  } else {
#ifdef LI_ON_READ_ERROR_VALUE
    pli->val = LI_ON_READ_ERROR_VALUE;
#endif
    status = recGblSetSevr(pli, READ_ALARM, INVALID_ALARM);
    if (status && (pli->stat != READ_ALARM || pli->sevr != INVALID_ALARM)) {
      if (snmpDebugLevel) printf("%s  devSnmp LI read error '%s' : %s\n",tnow(),pli->name,pPV->errorString());
    }
    status = 2;
  }

  return(status);
}
//--------------------------------------------------------------------
static long snmpSiInit(struct stringinRecord *psi)
{
  // si.inp must be an VME_IO
  switch (psi->inp.type) {
  case INST_IO:
    break;

  default :
    recGblRecordError(S_db_badField,(void *)psi,
                      "devSnmpSi (init_record) Illegal INP field");
    return(S_db_badField);
  }

  if (! checkInit()) return(epicsError);
  devSnmp_pv *pPV = pManager->addPV((struct dbCommon *) psi, &psi->inp);
  if (! pPV) {
    recGblRecordError(S_db_badField,
                      (void *)psi,"devSnmpSi (init_record) bad parameters");
    return(S_db_badField);
  }

  psi->udf = false;
  return(epicsOk);
}
//--------------------------------------------------------------------
static long snmpSiRead(struct stringinRecord *psi)
{
  devSnmp_pv *pPV;
  epicsStatus status = epicsError;
  char val[40];

  if (psi->dpvt == NULL)
    return(epicsError);
  else
    pPV = (devSnmp_pv *) psi->dpvt;

  status = 2;

  if (pPV->getValueString(val,sizeof(val))) {
    memset(psi->val, '\0', 40);
    strcpy(psi->val, val);
    status = epicsOk;
  } else {
#ifdef SI_ON_READ_ERROR_VALUE
    strcpy(psi->val, SI_ON_READ_ERROR_VALUE);
#endif
    status = recGblSetSevr(psi, READ_ALARM, INVALID_ALARM);
    if (status && (psi->stat != READ_ALARM || psi->sevr != INVALID_ALARM)) {
      if (snmpDebugLevel) printf("%s  devSnmp SI read error '%s' : %s\n",tnow(),psi->name,pPV->errorString());
    }
    status = 2;
  }

  return(status);
}
//--------------------------------------------------------------------
static epicsStatus snmpWfInit(struct waveformRecord *pwf)
{
  epicsStatus status = epicsOk;

  // ai.inp must be an INST_IO
  switch (pwf->inp.type) {
  case INST_IO:
    break;

  default :
    recGblRecordError(S_db_badField,(void *)pwf,
                      "devSnmpWf (init_record) Illegal INP field");
    return(S_db_badField);
  }

  if (! checkInit()) return(epicsError);
  devSnmp_pv *pPV = pManager->addPV((struct dbCommon *) pwf, &pwf->inp);
  if (! pPV) {
    recGblRecordError(S_db_badField,
                      (void *)pwf,"devSnmpWf (init_record) bad parameters");
    return(S_db_badField);
  }

  switch (pwf->ftvl) {
    case DBF_STRING:
    case DBF_CHAR:
    case DBF_UCHAR:
        pPV->setDataLength((sizeof(char) * pwf->nelm) + 1);
        break;
    default:
        pPV->setDataLength(sizeof(char) * 40);
        break;
  }

  pwf->udf = false;
  return(status);
}
//--------------------------------------------------------------------
static epicsStatus snmpWfRead(struct waveformRecord *pwf)
{
  devSnmp_pv *pPV;
  long status = epicsError;
  int i;

  if (pwf->dpvt == NULL)
    return(epicsError);
  else
    pPV = (devSnmp_pv *) pwf->dpvt;

  status = 2;

  int data_sz = pPV->getDataLength();
  char *val = new char [data_sz];

  if (pPV->getRawValueString(val,data_sz)) {
    for (i = 0; wfTypes[i].str != NULL; i++) {
      if (!snmpStrCmp(wfTypes[i].str, (char *)val)) {
        if (!wfTypes[i].func(pwf->bptr, (char *)val)) {
          pwf->nord = 1;
#if     1
          switch (pwf->ftvl) {
            case DBF_CHAR:
            case DBF_UCHAR:
            case DBF_STRING:
                pwf->nord = strlen((char *)pwf->bptr);
                break;
#if     0
            case DBF_CHAR:
            case DBF_UCHAR:
                printf("bptr char %c\n", pwf->bptr,0,0,0,0,0);
                break;

            case DBF_SHORT:
            case DBF_USHORT:
                printf("bptr short %d\n", pwf->bptr,0,0,0,0,0);
                break;

            case DBF_LONG:
            case DBF_ULONG:
                {
                  long *lp;
                  lp = (long*) pwf->bptr;
                  printf("bptr long %ld\n", *lp,0,0,0,0,0);
                }
                break;
#endif
            default:
                break;
          }
#endif
          status = epicsOk;
          break;
        }
      }
    }
  } else {
    status = recGblSetSevr(pwf, READ_ALARM, INVALID_ALARM);
    if (status && (pwf->stat != READ_ALARM || pwf->sevr != INVALID_ALARM)) {
      if (snmpDebugLevel) printf("%s  devSnmp WF read error '%s' : %s\n",tnow(),pwf->name,pPV->errorString());
    }
    status = 2;
  }

  if (val) delete [] val;

  return(status);
}
//--------------------------------------------------------------------
static long snmpAoInit(struct aoRecord *pao)
{
  // ao.scan must be Passive
  if (pao->scan != menuScanPassive) {
    recGblRecordError(S_db_badField,(void *)pao,
                      "devSnmpAo (init_record) SCAN must be Passive");
    return(S_db_badField);
  }

  // ao.out must be an INST_IO
  switch (pao->out.type) {
  case INST_IO:
    break;

  default :
    recGblRecordError(S_db_badField,(void *)pao,
                      "devSnmpAo (init_record) Illegal OUT field");
    return(S_db_badField);
  }

  if (! checkInit()) return(epicsError);
  devSnmp_pv *pPV = pManager->addPV((struct dbCommon *) pao, &pao->out);
  if (! pPV) {
    recGblRecordError(S_db_badField,
                      (void *)pao,"devSnmpAo (init_record) bad parameters");
    return(S_db_badField);
  }
  pPV->setPollMSec(snmpPassivePollMSec);

  /* request a periodic call to snmpAoReadback, so our display value is
     periodically updated with value from the remote host */
  pPV->setPeriodicCallback(snmpAoReadback,(snmpPassivePollMSec / 1000.0));

  pao->udf = false;
  return(epicsOk);
}
//--------------------------------------------------------------------
static long snmpAoWrite(struct aoRecord *pao)
{
  devSnmp_pv *pPV;

  if (pao->dpvt == NULL)
    return(epicsError);
  else
    pPV = (devSnmp_pv *) pao->dpvt;

  /* if we're being called as a result of a readback update,
     just report success */
  if (pPV->doingProcess()) return(epicsOk);

  // test to make sure value is valid
  if (isnan(pao->val)) {
    if (snmpDebugLevel)
      printf("----- devSnmp ERROR: AO setting %s to invalid number!\n",pao->name);
    return(epicsError);
  }

  // fill in set data
  // guard against negative zero, it annoys some devices
  if (strstr(pao->desc,"SETDBG")) pPV->debugSetStart();
  double outv = pao->val;
  if (local_isNegativeZero(outv)) outv = 0.0;
  long status = epicsOk;
  char tmp[40];
  sprintf(tmp,"%lg",outv);
  pPV->set(tmp);
  if (snmpDebugLevel)
    printf("----- devSnmp AO setting %s to [%s] ...\n",pao->name,tmp);

  return(status);
}
//--------------------------------------------------------------------
static long snmpAoReadback(devSnmp_pv *pPV)
{
  struct aoRecord *pao = (struct aoRecord *) pPV->record();
  epicsStatus status = epicsError;
  double new_val;
  bool process_record = false;

  status = -1;

  if (pPV->getValueDouble(&new_val)) {
    status = 2;

    /* if it hasn't been long enough since last setting sent, skip
       checking versus our current val field (don't want our val
       field over-written if pPV hasn't had time to process
       setting yet) */
    if (! pPV->wasSetRecently()) {
      /* we use a different-enough function rather than directly
         comparing the old/new values to avoid being triggered often
         due to meaningless noise */
      if (differentEnough(pao->val,new_val)) {
        // value read from remote host doesn't match what we think val is
        if (snmpDebugLevel)
          printf("----- devSnmp AO value change %s [%lg] -> [%lg] ...\n",pao->name,pao->val,new_val);
        pao->val = new_val;
        process_record = true;
      }
    }

    // need to clear invalid alarm if present
    if (pao->sevr == INVALID_ALARM) {
      recGblResetAlarms(pao);
      process_record = true;
    }

  } else {
    status = recGblSetSevr(pao, READ_ALARM, INVALID_ALARM);
    if (status && (pao->stat != READ_ALARM || pao->sevr != INVALID_ALARM)) {
      // trigger processing so stat/sevr is updated
      process_record = true;
      if (snmpDebugLevel) printf("%s  devSnmp AO read error '%s' : %s\n",tnow(),pao->name,pPV->errorString());
    }
    status = -1;
  }

  // process record if needed
  if (process_record) pPV->processRecord();

  return(status);
}
//--------------------------------------------------------------------
static long snmpLoInit(struct longoutRecord *plo)
{
  // lo.scan must be Passive
  if (plo->scan != menuScanPassive) {
    recGblRecordError(S_db_badField,(void *)plo,
                      "devSnmpLo (init_record) SCAN must be Passive");
    return(S_db_badField);
  }

  // lo.out must be an INST_IO
  switch (plo->out.type) {
  case INST_IO:
    break;

  default :
    recGblRecordError(S_db_badField,(void *)plo,
                      "devSnmpLo (init_record) Illegal OUT field");
    return(S_db_badField);
  }

  if (! checkInit()) return(epicsError);
  devSnmp_pv *pPV = pManager->addPV((struct dbCommon *) plo, &plo->out);
  if (! pPV) {
    recGblRecordError(S_db_badField,
                      (void *)plo,"devSnmpLo (init_record) bad parameters");
    return(S_db_badField);
  }
  pPV->setPollMSec(snmpPassivePollMSec);

  /* request a periodic call to snmpLoReadback, so our display value is
     periodically updated with value from the remote host */
  pPV->setPeriodicCallback(snmpLoReadback,(snmpPassivePollMSec / 1000.0));

  plo->udf = false;
  return(epicsOk);
}
//--------------------------------------------------------------------
static long snmpLoWrite(struct longoutRecord *plo)
{
  devSnmp_pv *pPV;

  if (plo->dpvt == NULL)
    return(epicsError);
  else
    pPV = (devSnmp_pv *) plo->dpvt;

  /* if we're being called as a result of a readback update,
     just report success */
  if (pPV->doingProcess()) return(epicsOk);

  // fill in set data
  long status = epicsOk;
  char tmp[40];
  sprintf(tmp,"%ld", (long) plo->val);
  pPV->set(tmp);

  return(status);
}
//--------------------------------------------------------------------
static long snmpLoReadback(devSnmp_pv *pPV)
{
  struct longoutRecord *plo = (struct longoutRecord *) pPV->record();
  epicsStatus status = epicsError;
  char val[40];
  long new_val;
  bool process_record = false;

  if (pPV->getValueString(val,sizeof(val))) {
    if (strstr(val,"undefined")) {
      /* if string contains "undefined" is a write-only channel (some Wiener
         channels do this, for example groupsSwitch.x) */
      status = 2;
      new_val = -1;
    } else if (pPV->getValueLong(&new_val)) {
      status = 2;
    } else {
      status = -1;
    }
  } else {
    status = -1;
  }

  if (status >= 0) {
    /* if it hasn't been long enough since last setting sent, skip
       checking versus our current val field (don't want our val
       field over-written if pPV hasn't had time to process
       setting yet) */
    if (! pPV->wasSetRecently()) {
      if (new_val != plo->val) {
        // value read from remote host doesn't match what we think val is
        if (snmpDebugLevel)
          printf("----- devSnmp LO value change %s [%ld] -> [%ld] ...\n",plo->name,(long)plo->val,new_val);
        plo->val = new_val;
        process_record = true;
      }
    }

    // need to clear invalid alarm if present
    if (plo->sevr == INVALID_ALARM) {
      recGblResetAlarms(plo);
      process_record = true;
    }
  } else {
    status = recGblSetSevr(plo, READ_ALARM, INVALID_ALARM);
    if (status && (plo->stat != READ_ALARM || plo->sevr != INVALID_ALARM)) {
      // trigger processing so stat/sevr is updated
      process_record = true;
      if (snmpDebugLevel) printf("%s  devSnmp LO read error '%s' : %s\n",tnow(),plo->name,pPV->errorString());
    }
    status = -1;
  }

  // process record if needed
  if (process_record) pPV->processRecord();

  return(status);
}
//--------------------------------------------------------------------
static long snmpSoInit(struct stringoutRecord *pso)
{
  // so.scan must be Passive
  if (pso->scan != menuScanPassive) {
    recGblRecordError(S_db_badField,(void *)pso,
                      "devSnmpSo (init_record) SCAN must be Passive");
    return(S_db_badField);
  }

  // so.out must be an INST_IO
  switch (pso->out.type) {
  case INST_IO:
    break;

  default :
    recGblRecordError(S_db_badField,(void *)pso,
                      "devSnmpSo (init_record) Illegal OUT field");
    return(S_db_badField);
  }

  if (! checkInit()) return(epicsError);
  devSnmp_pv *pPV = pManager->addPV((struct dbCommon *) pso, &pso->out);
  if (! pPV) {
    recGblRecordError(S_db_badField,
                      (void *)pso,"devSnmpSo (init_record) bad parameters");
    return(S_db_badField);
  }
  pPV->setPollMSec(snmpPassivePollMSec);

  /* request a periodic call to snmpSoReadback, so our display value is
     periodically updated with value from the remote host */
  pPV->setPeriodicCallback(snmpSoReadback,(snmpPassivePollMSec / 1000.0));

  pso->udf = false;
  return(epicsOk);
}
//--------------------------------------------------------------------
static long snmpSoWrite(struct stringoutRecord *pso)
{
  devSnmp_pv *pPV;

  if (pso->dpvt == NULL)
    return(epicsError);
  else
    pPV = (devSnmp_pv *) pso->dpvt;

  /* if we're being called as a result of a readback update,
     just report success */
  if (pPV->doingProcess()) return(epicsOk);

  // fill in set data
  long status = epicsOk;
  pPV->set(pso->val);

  if (snmpDebugLevel)
    printf("----- devSnmp SO setting %s to [%s] ...\n",pso->name,pso->val);

  return(status);
}
//--------------------------------------------------------------------
static long snmpSoReadback(devSnmp_pv *pPV)
{
  struct stringoutRecord *pso = (struct stringoutRecord *) pPV->record();
  epicsStatus status = epicsError;
  char val[40];
  char new_val[40];
  bool process_record = false;

  status = -1;

  if (pPV->getValueString(val,sizeof(val))) {
    strcpy(new_val,val);
    status = 2;

    /* if it hasn't been long enough since last setting sent, skip
       checking versus our current val field (don't want our val
       field over-written if pPV hasn't had time to process
       setting yet) */
    if (! pPV->wasSetRecently()) {
      if (strcmp(new_val,pso->val)) {
        // value read from remote host doesn't match what we think val is
        if (snmpDebugLevel)
          printf("----- devSnmp SO value change %s [%s] -> [%s] ...\n",pso->name,pso->val,new_val);
        strcpy(pso->val,new_val);
        process_record = true;
      }
    }

    // need to clear invalid alarm if present
    if (pso->sevr == INVALID_ALARM) {
      recGblResetAlarms(pso);
      process_record = true;
    }
  } else {
    status = recGblSetSevr(pso, READ_ALARM, INVALID_ALARM);
    if (status && (pso->stat != READ_ALARM || pso->sevr != INVALID_ALARM)) {
      // trigger processing so stat/sevr is updated
      strcpy(pso->val, "INVALID");
      process_record = true;
      if (snmpDebugLevel) printf("%s  devSnmp SO read error '%s' : %s\n",tnow(),pso->name,pPV->errorString());
    }
    status = -1;
  }

  // process record if needed
  if (process_record) pPV->processRecord();

  return(status);
}
//--------------------------------------------------------------------
static int snmpWfGauge32Convert(void *rval, char *sval)
{
  char *cp;
  SNMP_UINT32 *lp = (SNMP_UINT32*) rval;

  if (sval == NULL) return(epicsError);
  cp = strchr(sval, ':') + 1;
  if ((cp != NULL) && (lp != NULL) && sscanf(cp, "%" SCNu32, lp))
    return(epicsOk);
  else
    return(epicsError);
}
//--------------------------------------------------------------------
static int snmpWfGauge64Convert(void *rval, char *sval)
{
  char *cp;
  SNMP_UINT64 *llp = (SNMP_UINT64 *) rval;

  if (sval == NULL) return(epicsError);
  cp = strchr(sval, ':') + 1;
  if ((cp != NULL) && (llp != NULL) && sscanf(cp, "%" SCNu64, llp))
    return(epicsOk);
  else
    return(epicsError);
}
//--------------------------------------------------------------------
static int snmpWfCntr32Convert(void *rval, char *sval)
{
  char *cp;
  SNMP_UINT32 *lp = (SNMP_UINT32*) rval;

  if (sval == NULL) return(epicsError);
  cp = strchr(sval, ':') + 1;
  if ((cp != NULL) && (lp != NULL) && sscanf(cp, "%" SCNu32, lp))
    return(epicsOk);
  else
    return(epicsError);
}
//--------------------------------------------------------------------
static int snmpWfCntr64Convert(void *rval, char *sval)
{
  char *cp;
  SNMP_UINT64 *llp = (SNMP_UINT64*) rval;

  if (sval == NULL) return(epicsError);
  cp = strchr(sval, ':') + 1;
  if ((cp != NULL) && (llp != NULL) && sscanf(cp, "%" SCNu64, llp))
    return(epicsOk);
  else
    return(epicsError);
}
//--------------------------------------------------------------------
static int snmpWfTimeTicksConvert(void *rval, char *sval)
{
  char *cp;
  SNMP_UINT32 *lp = (SNMP_UINT32*) rval;

  if (sval == NULL) return(epicsError);

  // timeticks have the value enclosed in '()' so we bump over it
  cp = strchr(sval, '(') + 1;

  if ((cp != NULL) && (lp != NULL) && sscanf(cp, "%" SCNu32, lp))
    return(epicsOk);
  else
    return(epicsError);
}
//--------------------------------------------------------------------
static int snmpWfHexStrConvert(void *rval, char *sval)
{
  char *cp;
  char *lp = (char*) rval;

  if (sval == NULL) return(epicsError);
  cp = strchr(sval, ':') + 1;

  if ((cp != NULL) && (lp != NULL) && sscanf(cp, "%s", lp))
    return(epicsOk);
  else
    return(epicsError);
}
//--------------------------------------------------------------------
static int snmpWfNetAddrConvert(void *rval, char *sval)
{
  char *cp;
  char *lp = (char*) rval;

  if (sval == NULL) return(epicsError);
  cp = strchr(sval, ':') + 1;

  if ((cp != NULL) && (lp != NULL) && sscanf(cp, "%s", lp))
    return(epicsOk);
  else
    return(epicsError);
}
//--------------------------------------------------------------------
static int snmpWfIntConvert(void *rval, char *sval)
{
  char *cp;
  SNMP_UINT32 *lp = (SNMP_UINT32*) rval;

  if (sval == NULL) return(epicsError);
  cp = strchr(sval, ':') + 1;

  if ((cp != NULL) && (lp != NULL) && sscanf(cp, "%ul", lp))
    return(epicsOk);
  else
    return(epicsError);
}
//--------------------------------------------------------------------
static int snmpWfStrConvert(void *rval, char *sval)
{
  char *cp;
  char *lp = (char*) rval;

  if (sval == NULL) return(epicsError);
  cp = strchr(sval, ':') + 1;

#if     0
  printf("StrConvert %s\n", cp,0,0,0,0,0);
#endif

  if ((cp != NULL) && (lp != NULL)) {
    while (*cp)
      *lp++ = *cp++;
    return(epicsOk);
  } else
    return(epicsError);
}
//--------------------------------------------------------------------
static int snmpWfIpAddrConvert(void *rval, char *sval)
{
  char *cp;
  char *lp = (char*) rval;

  if (sval == NULL) return(epicsError);
  cp = strchr(sval, ':') + 1;

  if ((cp != NULL) && (lp != NULL) && sscanf(cp, "%s", lp))
    return(epicsOk);
  else
    return(epicsError);
}
//--------------------------------------------------------------------

 
