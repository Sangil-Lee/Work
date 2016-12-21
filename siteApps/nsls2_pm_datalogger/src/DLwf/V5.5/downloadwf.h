#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <cadef.h>
#include <epicsStdlib.h>
#include <epicsString.h>
#include <epicsGetopt.h>
#include <epicsTime.h>
#include <mclib.h>
#include <his_def.h>

#define DD_DEB 1

/* in mclib.h
#define PC_MSGSIZE 132
#define TIMETEXTLEN 32

#ifndef PV_NAMELEN
#define PV_NAMELEN 40
typedef char PV_NAME[PV_NAMELEN];
#endif
*/

/* In mclib.h
#define TS_LENGTH 40
typedef char TS_STR[TS_LENGTH];
*/

extern int errno;
extern char *strerror();
int dd_readpvs_dbrtype();
int dd_writepvs_bo();
int dd_resetstatus();

/* --------------------------------------------------------------------- */
#ifdef REFERENCE
typedef struct
{
  PV_NAME name;
  int nelems, value_size; /* tsize in bytes of value*/
  int created;
  int connected;
  int error;
  chid chid;
  long dbfType;
  long dbrType;
  int status;
  void *value;
  epicsTimeStamp ts;
  char mess[132];
} EPICS_PV; /* EPICS System PV */

typedef struct epicsTimeStamp {
    epicsUInt32    secPastEpoch;   /* seconds since 0000 Jan 1, 1990 */
    epicsUInt32    nsec;           /* nanoseconds within second */
} 
#endif
/* --------------------------------------------------------------------- */

#define DD_ROOT "/WFdata/WFdata"
#define DD_DATADIR "Data"
#define DD_LOGDIR "Log"
#define DD_PARFILE "downloadwf.par"
#define DD_LOGFILE "downloadwf.log"
#define DD_TAGFILE "downloadwf.tag"
#define DD_IDHOST "dlidhost.dat"
#define DD_INFOFILE "dlinfo.dat"
#define DD_TOPDIR "/WFdata"

#define DD_HOST "downloadwf.host"

#ifdef DDMAIN
#define DD_EXTERN
#else
#define DD_EXTERN extern
#endif

char *utimestring();
int dd_init();
void dd_getline();
void dd_log();
int dd_realloc();

#define DD_DETECTED 0
#define DD_NORMAL 1

typedef struct {
	PV_NAME name;
	int nelems, value_size; 
	int err;
	int created, connected;
	chid chid;
	int allocated;
	long dbrtype;
	char *dbr_value;
} DD_PV;

#define MAX_WFPROGS 20
#define MAX_RESETS 20
#define MAX_IOERRORS 10

typedef struct {
	int used_flag;
	char prog[16];
	char root[64];
	char datapath[32];
	char parfile[32];
	char cmd[320];
} WF_PROG;

DD_EXTERN HS_SAVEREC *ddsaverec;
DD_EXTERN SHM_STRUCT shm_struct;

#define DD_BPMWF 0
#define DD_BPMFAWF 1
#define DD_PSWF 2
#define DD_LLRFWF 3


typedef struct {
	char version[8];
	char host[80];
	char start_time[32];
	double connect_timeout,read_timeout, write_timeout;
	int in_download, shm_flag, reset_flag, sys64_flag;
	int test_mode;
	int secs,msecs;
	int delay;
	int priority;
	int ntrigs, nresets, nwfused, nwfprogs;
	int nsaves, ndls, mem_flag;
	int icurrent;
	int max_ioerrors;
	int nmaxsaves, max_saves, cmd_max_saves;
	WF_PROG wfprogs[MAX_WFPROGS];
	int wfused_indices[MAX_WFPROGS];
	DD_PV trig_pv[MAX_WFPROGS];
	int trig_value[MAX_WFPROGS];
	TS_STR trig_timestamp;
	TS_STR old_trig_timestamp;
	DD_PV reset_pv[MAX_RESETS];
	int reset_value[MAX_RESETS];
	int reset_fileflag;
	int shm_id, server_id, shm_size;
	char *shm;
	pid_t pid;

	char root[80]; /* Any directory */
	char datadir[32];
	char logdir[32];
	char parfile[32];
	char logfile[32];
	char resetfile[32];
	char datapath[132]; /* directory */
	char parpath[132];
	char logpath[132];  /* absolute file path */
	char infopath[132];
	char prog_name[132];
	char prog_line[400];
	char dlprog[132];
	char spare[1024];
} DDREC;

DD_EXTERN DDREC ddrec, *ddrecptr;

