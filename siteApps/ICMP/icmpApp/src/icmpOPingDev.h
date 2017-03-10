#ifndef __ICMP_OPING_H
#define __ICMP_OPING_H
//////////////////////
#ifndef TARGET_UID
#define TARGET_UID 0
#endif
#ifndef TARGET_GID
#define TARGET_GID 0
#endif
#ifndef UID_MIN
#define UID_MIN 500
#endif
#ifndef GID_MIN
#define GID_MIN 500
#endif
//////////////////////

#include <vector>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <signal.h>
#include <string>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <errno.h>
#include <oping.h>
#include <sys/time.h>
# include <netdb.h> /* NI_MAXHOST */
    
#include "alarm.h"
#include "cvtTable.h"
#include "dbDefs.h"
#include "dbAccess.h" 
#include "recGbl.h"
#include "recSup.h"
#include "devSup.h"
#include "link.h"
#include "menuScan.h"
#include "epicsExport.h"
#include "shareLib.h"
#include "dbCommon.h"
#include "dbScan.h"
#include "callback.h"
#include "cantProceed.h"
#include "epicsStdio.h"
#include "epicsMutex.h"
#include "epicsEvent.h"
#include "epicsThread.h"
#include "iocsh.h"
#include "icmpRecord.h"


#ifndef HISTORY_SIZE_MAX
# define HISTORY_SIZE_MAX 900
#endif

enum INPUTLINK{ICMP_FILD=0,
	ICMP_FILDA, ICMP_FILDB, ICMP_FILDC, ICMP_FILDD, ICMP_FILDE, ICMP_FILDF, ICMP_FILDG, ICMP_FILDH, ICMP_FILDI, ICMP_FILDJ, ICMP_FILDK, 
	ICMP_FILDL, ICMP_FILDM, ICMP_FILDN, ICMP_FILDO, ICMP_FILDP, ICMP_FILDQ, ICMP_FILDR, ICMP_FILDS, ICMP_FILDT, ICMP_FILDU, ICMP_FILDV,
	ICMP_FILDW, ICMP_FILDX, ICMP_FILDY, ICMP_FILDZ };


using namespace std;
        
#ifdef __cplusplus
extern "C" {
#endif

typedef struct ping_context
{
	char host[NI_MAXHOST];
	char addr[NI_MAXHOST];

	int index;
	int req_sent;
	int req_rcvd;

	double latency_total;

	/* The last n RTTs in the order they were sent. */
	double history_by_time[HISTORY_SIZE_MAX];

	/* Current number of entries in the history. This is a value between 0
	 * and HISTORY_SIZE_MAX. */
	size_t history_size;

	/* Number "received" entries in the history, i.e. non-NAN entries. */
	size_t history_received;

	/* Index of the next RTT to be written to history_by_time. This wraps
	 * around to 0 once the histroty has grown to HISTORY_SIZE_MAX. */
	size_t history_index;

	/* The last history_size RTTs sorted by value. timed out packets (NAN
	 * entries) are sorted to the back. */
	double history_by_value[HISTORY_SIZE_MAX];

	/* If set to true, history_by_value has to be re-calculated. */
	bool history_dirty;

} ping_context_t;

long init_record (icmpRecord *precord);
long icmp_oping(icmpRecord* precord);

epicsShareFunc int icmpOPingInit(const char *ipfile, const char *device, int timeout);

typedef struct devicmpOPingSoft{
	long        number;
	DEVSUPFUN   report;
	DEVSUPFUN   init;
	DEVSUPFUN   init_record;
	DEVSUPFUN   get_ioint_info;
	//long  (*get_ioint_info) (int, dbCommon*, IOSCANPVT*);
	DEVSUPFUN   icmp_oping;
}devicmpOPingSoft;

class OPingManager
{
public:
	OPingManager(const string filename, const string device, const int timeout);
	//OPingManager();
	~OPingManager();
	int Ping();
	
private:
	uid_t ruid, euid, suid; /* Real, Effective, Saved user ID */
	gid_t rgid, egid, sgid; /* Real, Effective, Saved group ID */
	pingobj_t      *ping;
	pingobj_iter_t *iter;

	int     send_ttl;
	uint8_t send_qos;
	double	interval;
	int     addrfamily;

	string mfilename, mdevice;
	int mtimeout;
	vector<string> iplist;
	vector<string> failist;

	struct sigaction sigint_action;
	struct timeval  tv_begin;
	struct timeval  tv_end;
	struct timespec ts_wait;
	struct timespec ts_int;

	void init();
	int gainPrivileges();
	int dropPrivileges();
	int opingInit();
	int addHost();
	int ping_initialize_contexts();
	ping_context_t *context_create ();
	int pre_loop_hook();
	void update_host_hook (pingobj_iter_t *iter, int index);
	void update_context (ping_context_t *ctx, double latency);
	void context_destroy (ping_context_t *context);
	void context_destroy_all();
	void time_calc (struct timespec *ts_dest, const struct timespec *ts_int,
		const struct timeval  *tv_begin, const struct timeval  *tv_end);
	void time_normalize (struct timespec *ts);
};




#ifdef __cplusplus
}
#endif

#endif // __ICMP_OPING_H

