/* devSysMon.c Miroslaw Dach SLS/PSI 16.10.2004*/
/* originally created for EPICS 3.14.1 */
/* For EPICS 3.14.7 compatibility it was introduced */
/* and epicsExportAddress function for every device support */  
/* This device support reads the uptime from /proc/uptime and converts it 
   to human readable format */
/* it examines also following files to obtain CPU load, load avg and mem info
   /proc/loadavg
   /proc/stat
   /proc/meminfo
*/
/*
   * add module S.I Lee KSTAR in NFRI
*/

#include <stddef.h>
#include <ctype.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>
#include <vector>

#include "epicsExit.h"
#include "alarm.h"
#include "cvtTable.h"
#include "dbDefs.h"
#include "dbAccess.h"
#include "recGbl.h"
#include "recSup.h"
#include "devSup.h"
#include "link.h"
#include "stringinRecord.h"
#include "aiRecord.h"
#include "longinRecord.h"

#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include "/usr/include/linux/version.h"

#include <sys/time.h>
#include <sys/types.h>
#include <signal.h>
#include <time.h>

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netdb.h>

#include <sys/utsname.h>
/* in vesrion 3.14.7 #include "epicsExport.h" */
#include "epicsExport.h"

using std::string;
using std::vector;

typedef void (*EPICS_EXIT_HANDLER)(void *arg);
static void exitSysMonIOC(void);

class SysMonInfo
{
public:
	SysMonInfo(){init();};
	~SysMonInfo(){destroy();};

	enum USERPROC {
		TMS=1, VMS=TMS*2, ECH=VMS*2, DDS=ECH*2, FUEL=DDS*2, GCDS=FUEL*2, ICRH=GCDS*2
	};

	enum SYSTEMPROC {
		CAREPEATER=1, NTPD=CAREPEATER*2, SYSMON=NTPD*2
	};

	string getHostname(){return mhostname;};
	unsigned int getSysProcessStatus();
	unsigned int getUserProcessStatus();
	string printBiString(unsigned int bval);
	int bitCheck(unsigned int src, unsigned int org);
	double getDiskUsage(char * parm);

private:
	string mhostname;
	void init();
	void destroy();
	void registerHostService(const char *hostname);
	pid_t checkProcess(const string processname);
	vector<string> m_userproclist;
	vector<string> m_sysproclist;
};

int SysMonInfo::bitCheck(unsigned int src, unsigned int org)
{
	return (src&org) == 0 ? 0 : 1;
};
string SysMonInfo::printBiString(unsigned int bval)
{
	unsigned int ckVal = ~0u - (~0u >> 1);
	char bitChar[64];

	int count = 0;
	while( ckVal != 0)
	{
		if((bval&ckVal)!= 0) {
			bitChar[count]='1';
		} else {
			bitChar[count]='0';
		};
		ckVal = ckVal >> 1;
		count++;
	};
	bitChar[count]='\0';
	return string(bitChar);
};

unsigned int SysMonInfo::getUserProcessStatus()
{
	vector<string>::iterator svclistiter;
	unsigned int procstatus = 0;
	for(svclistiter=m_userproclist.begin(); svclistiter!=m_userproclist.end();++svclistiter)
	{
		string strprocess = svclistiter->c_str();
		if(strprocess.compare("TMS") == 0)
		{
			if (checkProcess("TMS") == 0)
				procstatus &= ~TMS;
			else
				procstatus |= TMS;
			continue;
		}
		else if(strprocess.compare("VMS") == 0)
		{
			if (checkProcess("VMS") == 0)
				procstatus &= ~VMS;
			else
				procstatus |= VMS;
			continue;
		}
		else if(strprocess.compare("ECH") == 0)
		{
			if (checkProcess("ECH") == 0)
				procstatus &= ~ECH;
			else
				procstatus |= ECH;
			continue;
		}
		else if(strprocess.compare("DDS") == 0)
		{
			if (checkProcess("DDS") == 0)
				procstatus &= ~DDS;
			else
				procstatus |= DDS;
			continue;
		}
		else if(strprocess.compare("FUEL") == 0)
		{
			if (checkProcess("Fuel") == 0)
				procstatus &= ~FUEL;
			else
				procstatus |= FUEL;
			continue;
		}
		else if(strprocess.compare("GCDS") == 0)
		{
			if (checkProcess("GCDS") == 0)
				procstatus &= ~GCDS;
			else
				procstatus |= GCDS;
			continue;
		}
		else if(strprocess.compare("ICRH") == 0)
		{
			if (checkProcess("ICRH") == 0)
				procstatus &= ~ICRH;
			else
				procstatus |= ICRH;
			continue;
		};
	};
	return procstatus;
}
unsigned int SysMonInfo::getSysProcessStatus()
{
	vector<string>::iterator svclistiter;
	unsigned int procstatus = 0;
	for(svclistiter=m_sysproclist.begin(); svclistiter!=m_sysproclist.end();++svclistiter)
	{
		string strprocess = svclistiter->c_str();
		if(strprocess.compare("caRepeater") == 0)
		{
			if (checkProcess("caRepeater") == 0)
				procstatus |= CAREPEATER;
			else
				procstatus &= ~CAREPEATER;
			continue;
		}
		else if(strprocess.compare("ntpd") == 0)
		{
			if (checkProcess("ntpd") == 0)
				procstatus |= NTPD;
			else
				procstatus &= ~NTPD;
			continue;
		}
		else if(strprocess.compare("sysMonitor") == 0)
		{
			if (checkProcess("sysMonitor") == 0)
				procstatus |= SYSMON;
			else
				procstatus &= ~SYSMON;
			continue;
		};
	};
	return procstatus;
}

pid_t SysMonInfo::checkProcess(const string processname)
{
	string checkproc = string("/sbin/pidof ") + processname;
	FILE *fp = popen (checkproc.c_str(), "r");

	char buf[20];
	while (fgets (buf, sizeof(buf), fp) ) {};
	pclose(fp);
	//printf("procname:%s(status:%d)\n", processname.c_str(), atoi(buf));
	return atoi(buf);
};

void SysMonInfo::destroy()
{
	if(m_userproclist.empty() != true)
		m_userproclist.clear();
	if(m_sysproclist.empty() != true)
		m_sysproclist.clear();
	printf("Destroyed SysMonInfo\n");
}

void SysMonInfo::init()
{
	//printf("Created SysMonInfo\n");
	char hostname[255];
	gethostname(hostname, sizeof(hostname) );
	mhostname=string(hostname);
	registerHostService(hostname);
	epicsAtExit((EPICS_EXIT_HANDLER)exitSysMonIOC, NULL);
}

void SysMonInfo::registerHostService(const char *hostname)
{
	if(strncasecmp(hostname,"opi", 3) == 0 )
	{
		m_sysproclist.push_back("ntpd");
		m_sysproclist.push_back("caRepeater");
		m_sysproclist.push_back("sysMonitor");
		m_userproclist.push_back("VMS");
		m_userproclist.push_back("TMS");
	}
	else if(strcasecmp(hostname,"data") == 0 )
	{
		m_sysproclist.push_back("ntpd");
		m_sysproclist.push_back("caRepeater");
		m_sysproclist.push_back("sysMonitor");
		m_userproclist.push_back("http");
	}
	else if(strcasecmp(hostname,"channel") == 0 )
	{
		m_sysproclist.push_back("ntpd");
		m_sysproclist.push_back("caRepeater");
		m_sysproclist.push_back("sysMonitor");
		m_userproclist.push_back("\"ArchiveEngine -port 4812\"");
		m_userproclist.push_back("\"ArchiveEngine -port 4813\"");
		m_userproclist.push_back("\"ArchiveEngine -port 4814\"");
		m_userproclist.push_back("\"ArchiveEngine -port 4815\"");
		m_userproclist.push_back("\"ArchiveEngine -port 4816\"");
		m_userproclist.push_back("\"ArchiveEngine -port 4817\"");
		m_userproclist.push_back("\"ArchiveEngine -port 4818\"");
		m_userproclist.push_back("\"ArchiveEngine -port 4819\"");
		m_userproclist.push_back("\"ArchiveEngine -port 4820\"");
		m_userproclist.push_back("\"ArchiveEngine -port 4821\"");
		m_userproclist.push_back("\"ArchiveEngine -port 4825\"");
	}
	else if(strcasecmp(hostname,"mdsplus") == 0 )
	{
		m_sysproclist.push_back("ntpd");
		m_sysproclist.push_back("caRepeater");
		m_sysproclist.push_back("sysMonitor");
		m_userproclist.push_back("mdsip");
	}
	else if(strcasecmp(hostname,"backup") == 0 )
	{
		m_sysproclist.push_back("ntpd");
		m_sysproclist.push_back("caRepeater");
		m_sysproclist.push_back("sysMonitor");
		m_userproclist.push_back("nsrd");
	}
	else if(strcasecmp(hostname,"kstar_exp") == 0 )
	{
		m_sysproclist.push_back("ntpd");
		m_sysproclist.push_back("httpd");
		m_sysproclist.push_back("caRepeater");
		m_userproclist.push_back("tomcat");
	}
	else if(strcasecmp(hostname,"leesi") == 0 )
	{
		m_sysproclist.push_back("ntpd");
		m_sysproclist.push_back("caRepeater");
		m_sysproclist.push_back("sysMonitor");
		m_userproclist.push_back("TMS");
		m_userproclist.push_back("VMS");
		m_userproclist.push_back("FUEL");
	}
};
double SysMonInfo::getDiskUsage(char * parm)
{
	const int GPATH_MAX=64;
	FILE *fp;
	char path[GPATH_MAX];
	char *command = "df -k -P / /usr/local | grep -v FileSystem | awk '{print $3,$4}'";
	double diskusage = 0.;

	fp = popen(command, "r");
	if (fp == NULL)
	{
		fprintf(stderr,"Cannot execute %s\n", command);
		pclose(fp);
		return diskusage;
	}

	char *ptok = 0;
	int count = 0;
	unsigned int usedblk = 0, availblk = 0;
	while (fgets(path, GPATH_MAX, fp) != NULL)
	{
		count = 0;
		//printf("%s", path);
		if(!(ptok = strtok (path," ,\n"))) continue;
		while (ptok != NULL)
		{
			//printf("Tok[%d]:%s\n", count, ptok);
			if(count == 0 )
				usedblk += atoi(ptok);
			else
				availblk += atoi(ptok);
			if(!(ptok = strtok (NULL," ,\n"))) continue;
			count++;
		};
	};
	diskusage = static_cast<double>(usedblk)/(static_cast<double>(availblk)+static_cast<double>(usedblk));
	//printf("Used:%ld, Available:%ld, Diskusage:[%.1f]%\n",usedblk, availblk, diskusage*100);
	pclose(fp);
	return (diskusage*100);
}
SysMonInfo *psysmoninfo = new SysMonInfo();

void exitSysMonIOC(void)
{
	printf("Exit SysMonIOC\n");
	if(!psysmoninfo) delete psysmoninfo;
}

static int get_uptime(void);           /* get formated time from /proc/uptime */
static void format_uptime(char *str1); /* set formated up time in str1 */
static double getAvgLoad(char * parm); /* get the avg load for parm= 1min ora 5min ora 15min */

/*------------------------- Create the dset for devUptime ------------------*/
//static long init_record_s(struct stringinRecord *pStringIn);
//static long read_uptime(struct stringinRecord  *pStringIn);
static long init_record_s(void *pStringIn);
static long read_uptime(void  *pStringIn);
struct devUpTime_tag{
        long            number;
        DEVSUPFUN       report;
        DEVSUPFUN       init;
        DEVSUPFUN       init_record;
        DEVSUPFUN       get_ioint_info;
        DEVSUPFUN       read;
}devUpTime={
        5,
        NULL,
        NULL,
        init_record_s,
        NULL,
        read_uptime
};
epicsExportAddress(dset,devUpTime); 


//static long init_record_s(struct stringinRecord *pStringIn)
static long init_record_s(void *input)
{
	struct stringinRecord *pStringIn = (struct stringinRecord*)input;
    if(recGblInitConstantLink(&pStringIn->inp,DBF_STRING,&pStringIn->val))
         pStringIn->udf = FALSE;
    return(0);
}


//static long read_uptime(struct stringinRecord  *pStringIn)
static long read_uptime(void *input)
{
	struct stringinRecord *pStringIn = (struct stringinRecord*)input;
  /*     long status; */
    struct timeval        iheure;
    int    iCurTime=0;
    int    iUpTime=0;

    struct vmeio *pvmeio = &pStringIn->inp.value.vmeio;

    /* status = dbGetLink(&(pStringIn->inp),DBF_STRING, &(pStringIn->val),0,0); */
    /*If return was succesful then set undefined false*/
    /* sprintf(pStringIn->val,"ala %d",get_uptime()); */
    if(!strcmp(pvmeio->parm,"CURTIME")){
      
      gettimeofday(&iheure,NULL);
      sprintf(pStringIn->val,"%.16s",&(ctime((time_t *)&iheure.tv_sec))[0]);

    }
    else if(!strcmp(pvmeio->parm,"BOOTIME")){
	 gettimeofday(&iheure,NULL);
	 iCurTime=iheure.tv_sec;
	 iUpTime=get_uptime();
	 iCurTime=iCurTime-iUpTime;
	 sprintf(pStringIn->val,"%.16s",&(ctime((time_t *)&iCurTime))[0]);
       }
    else if(!strcmp(pvmeio->parm,"UPTIME")){
      format_uptime(pStringIn->val);
    }
    /* if(!status) */  pStringIn->udf = FALSE;
    return(0);
}

/*-----------------  get uptime in seconds -----------------------*/

int get_uptime(void){

  char str_seconds[80];  
  FILE *proc_file;
  int int_seconds;



  if((proc_file=fopen("/proc/uptime","r"))==NULL)
    { fprintf(stderr,"Cannot open /proc/uptime for reading!\n");
    return 0; /*before  exit(127); */
    } 
  
  fscanf(proc_file,"%s",str_seconds);   
  int_seconds=strtol(str_seconds,NULL,10);

  fclose(proc_file);


return int_seconds;

}/* get_uptime */

void format_uptime(char *str1)
{
  int curr,sec,min,hour,day;
  curr=get_uptime();

  day=curr/86400;
  curr=curr%86400;
  hour=curr/3600;
  curr=curr%3600;
  min=curr/60;
  sec=curr%60;
  if(day==0)
  	sprintf(str1,"%02d:%02d",hour,min);
  else if (day==1)
	  sprintf(str1,"%02d day %02d:%02d",day,hour,min);
  else
	  sprintf(str1,"%02d days %02d:%02d",day,hour,min);

}

/*---------------------------  get the AVG load ------------------------------------*/

static long init_record_ai(void *);
static long read_avg_load(void *);
struct devAvgLoad_tag{
        long            number;
        DEVSUPFUN       report;
        DEVSUPFUN       init;
        DEVSUPFUN       init_record_ai;
        DEVSUPFUN       get_ioint_info;
        DEVSUPFUN       read_ai;
        DEVSUPFUN       special_linconv;
}devAvgLoad={
        6,
        NULL,
        NULL,
        init_record_ai,
        NULL,
        read_avg_load,
		NULL
};

epicsExportAddress(dset,devAvgLoad);

//static long init_record_ai(pAiIn)
    //struct aiRecord    *pAiIn;
static long init_record_ai(void *pIn)
{
    struct aiRecord    *pAiIn = (struct aiRecord *)pIn;
    if(recGblInitConstantLink(&pAiIn->inp,DBF_DOUBLE,&pAiIn->val))
         pAiIn->udf = FALSE;
    return(0);
}


//static long read_avg_load(pAiIn)
    //struct aiRecord    *pAiIn;
static long read_avg_load(void *pIn)
{
    struct aiRecord    *pAiIn = (struct aiRecord *)pIn;
    long status=0;

    struct vmeio *pvmeio = &pAiIn->inp.value.vmeio;
    /* status = dbGetLink(&(pAiIn->inp),DBF_DOUBLE, &(pAiIn->val),0,0); */
    /*If return was succesful then set undefined false*/  
    
    pAiIn->val=getAvgLoad(pvmeio->parm);
   
    if(!status) pAiIn->udf = FALSE;
    return(2); /* no convertion fron rval to val */
}


double getAvgLoad(char * parm)
{
  FILE *proc_file;
  double avg1min=0; double avg5min=0; double avg15min=0;
  char str1[30]=""; char str2[30]="";
  double val=0;

  if((proc_file=fopen("/proc/loadavg","r"))==NULL)
  { 
	  fprintf(stderr,"Cannot open /proc/loadavg for reading!\n");
	  return val; /* before exit(127); */
  } 
  fscanf(proc_file,"%lf %lf %lf %s %s",&avg1min,&avg5min,&avg15min,str1,str2);

  if(!strcmp(parm,"1min")){
    /* printf("1min\n"); */
    val=avg1min;
  }
  else if(!strcmp(parm,"5min")){
    /* printf("5min\n"); */
    val=avg5min;
  }	
  else{
    /* printf("15min\n"); */
    val=avg15min;
  }
  /* printf("val %f %f\n",val,avg1min); */
  
  fclose(proc_file);
  return val;
}

/*-------------------------------------------------------------------*/
/*        CPU and MEMory usage functions                             */
/*-------------------------------------------------------------------*/

#define BAD_OPEN_MESSAGE					\
"Error: /proc must be mounted\n"				\
"  To mount /proc at boot you need an /etc/fstab line like:\n"	\
"      /proc   /proc   proc    defaults\n"			\
"  In the meantime, mount /proc /proc -t proc\n"


#define STAT_FILE    "/proc/stat"
static int stat_fd = -1;
#define MEMINFO_FILE "/proc/meminfo"
static int meminfo_fd = -1;
static int iFile2BufStstus = -1; 
static char buf[1024];
/* int show_memory = 1;    show memory summary */
/*        iFile2BufStstus = -1;                                   \
	return -1; before _exit(1); 	      		\ */

#define FILE_TO_BUF(filename, fd) do{				\
    static int n;						\
    iFile2BufStstus = 0;                                        \
    if (fd == -1 && (fd = open(filename, O_RDONLY)) == -1) {	\
	fprintf(stderr, BAD_OPEN_MESSAGE);			\
	close(fd);						\
        iFile2BufStstus = -1;                                   \
	return 0; /* before _exit(1); */       		\
    }								\
    lseek(fd, 0L, SEEK_SET);					\
    if ((n = read(fd, buf, sizeof buf - 1)) < 0) {		\
	perror(filename);					\
	close(fd);						\
	fd = -1;						\
	return 0;						\
    }								\
    buf[n] = '\0';						\
}while(0)

#define SET_IF_DESIRED(x,y)  if(x) *(x) = (y)	/* evals 'x' twice */
#define LINUX_VERSION(x,y,z)   (0x10000*(x) + 0x100*(y) + z)

#define JT unsigned long long
int four_cpu_numbers(JT *uret, JT *nret, JT *sret, JT *iret) 
{
	static JT u, n, s, i;
	JT user_j, nice_j, sys_j, idle_j;
	int iStatus=0;
	FILE_TO_BUF(STAT_FILE,stat_fd);
	if (iFile2BufStstus == -1) return (-1);

	sscanf(buf, "cpu %llu %llu %llu %llu", &user_j, &nice_j, &sys_j, &idle_j);
	if(user_j>=u) {
		SET_IF_DESIRED(uret, user_j-u);
	}
	else{
		SET_IF_DESIRED(uret, u-user_j);
		iStatus=1;
	}

	if(nice_j>=n){
		SET_IF_DESIRED(nret, nice_j-n);
	}
	else{
		SET_IF_DESIRED(nret, n-nice_j);
		iStatus=1;
	}

	if(sys_j>=s){
		SET_IF_DESIRED(sret,  sys_j-s);
	}
	else{
		SET_IF_DESIRED(sret,  s-sys_j);
		iStatus=1;
	}

	if(idle_j>=i){
		SET_IF_DESIRED(iret, idle_j-i);
	}
	else{
		SET_IF_DESIRED(iret, i-idle_j);
		iStatus=1;
	}
	u=user_j; n=nice_j; s=sys_j; i=idle_j;
	return iStatus;
}
#undef JT

#define MAX_ROW 3	/* these are a little liberal for flexibility */
#define MAX_COL 7
enum meminfo_row { meminfo_main = 0, meminfo_swap };

enum meminfo_col { meminfo_total = 0, meminfo_used, meminfo_free,
                   meminfo_shared, meminfo_buffers, meminfo_cached };

unsigned  long **meminfo(void)
{
	static unsigned long *row[MAX_ROW + 1];		/* row pointers */
	static unsigned long num[MAX_ROW * MAX_COL];	/* number storage */
	char *p;
	char fieldbuf[12];		/* bigger than any field name or size in kb */
	int i, j, k, l;
	int linux_version_code;

	linux_version_code=LINUX_VERSION_CODE;
	FILE_TO_BUF(MEMINFO_FILE,meminfo_fd);
	if (iFile2BufStstus == -1) return NULL; 
	if (!row[0])				/* init ptrs 1st time through */
		for (i=0; i < MAX_ROW; i++)		/* std column major order: */
			row[i] = num + MAX_COL*i;		/* A[i][j] = A + COLS*i + j */
	p = buf;
	for (i=0; i < MAX_ROW; i++)			/* zero unassigned fields */
		for (j=0; j < MAX_COL; j++)
			row[i][j] = 0;
	if (linux_version_code < LINUX_VERSION(2,0,0)) {
		for (i=0; i < MAX_ROW && *p; i++) {                /* loop over rows */
			while(*p && !isdigit(*p)) p++;          /* skip chars until a digit */
			for (j=0; j < MAX_COL && *p; j++) {     /* scanf column-by-column */
				l = sscanf(p, "%lu%n", row[i] + j, &k);
				p += k;                             /* step over used buffer */
				if (*p == '\n' || l < 1)            /* end of line/buffer */
					break;
			};
		};
	} else {
		while(*p) {
			sscanf(p,"%11s%n",fieldbuf,&k);
			if(!strcmp(fieldbuf,"MemTotal:")) {
				p+=k;
				sscanf(p," %lu",&(row[meminfo_main][meminfo_total]));
				row[meminfo_main][meminfo_total]<<=10;
				while(*p++ != '\n');
			}
			else if(!strcmp(fieldbuf,"MemFree:")) {
				p+=k;
				sscanf(p," %lu",&(row[meminfo_main][meminfo_free]));
				row[meminfo_main][meminfo_free]<<=10;
				while(*p++ != '\n');
			}
			else if(!strcmp(fieldbuf,"MemShared:")) {
				p+=k;
				sscanf(p," %lu",&(row[meminfo_main][meminfo_shared]));
				row[meminfo_main][meminfo_shared]<<=10;
				while(*p++ != '\n');
			}
			else if(!strcmp(fieldbuf,"Buffers:")) {
				p+=k;
				sscanf(p," %lu",&(row[meminfo_main][meminfo_buffers]));
				row[meminfo_main][meminfo_buffers]<<=10;
				while(*p++ != '\n');
			}
			else if(!strcmp(fieldbuf,"Cached:")) {
				p+=k;
				sscanf(p," %lu",&(row[meminfo_main][meminfo_cached]));
				row[meminfo_main][meminfo_cached]<<=10;
				while(*p++ != '\n');
			}
			else if(!strcmp(fieldbuf,"SwapTotal:")) {
				p+=k;
				sscanf(p," %lu",&(row[meminfo_swap][meminfo_total]));
				row[meminfo_swap][meminfo_total]<<=10;
				while(*p++ != '\n');
			}
			else if(!strcmp(fieldbuf,"SwapFree:")) {
				p+=k;
				sscanf(p," %lu",&(row[meminfo_swap][meminfo_free]));
				row[meminfo_swap][meminfo_free]<<=10;
				while(*p++ != '\n');
			}
			else
				while(*p++ != '\n'); /* ignore lines we don't understand */
		};
		row[meminfo_swap][meminfo_used]=row[meminfo_swap][meminfo_total]-row[meminfo_swap][meminfo_free];
		row[meminfo_main][meminfo_used]=row[meminfo_main][meminfo_total]-row[meminfo_main][meminfo_free];
	};
	return row;					/* NULL return ==> error */
}

/*------------------------------------------------------------*/
/*         CPU record specyfication                           */
/*------------------------------------------------------------*/

static long init_record_aiCPU(void *);
static long read_CPU_load(void *);
struct devCPULoad_tag{
        long            number;
        DEVSUPFUN       report;
        DEVSUPFUN       init;
        DEVSUPFUN       init_record_ai;
        DEVSUPFUN       get_ioint_info;
        DEVSUPFUN       read_ai;
        DEVSUPFUN       special_linconv;
}devCPULoad={
        6,
        NULL,
        NULL,
        init_record_aiCPU,
        NULL,
        read_CPU_load,
		NULL
};

epicsExportAddress(dset,devCPULoad);

static long init_record_aiCPU(void *pIn)
{
    struct aiRecord    *pAiIn = (struct aiRecord *)pIn;
    if(recGblInitConstantLink(&pAiIn->inp,DBF_DOUBLE,&pAiIn->val))
         pAiIn->udf = FALSE;
    /* pthread_mutex_init(&readCPUmutex,NULL); */
    return(0);
}

static float  _system_ticks = 0, _user_ticks = 0, _nice_ticks = 0, _idle_ticks = 0;

static long read_CPU_load(void *pIn)
{
    struct aiRecord    *pAiIn = (struct aiRecord *)pIn;
	long status=0;
	unsigned long long system_ticks = 0, user_ticks = 0, nice_ticks = 0, idle_ticks = 0;

	int iStatus=0;
	unsigned long long sum=0;
	/* double       dSum=0;
	   float        fIdle=0;
	   double       dV1;
	   double       dV2;
	   double       dV3;
	   double       dV4; */

	struct vmeio *pvmeio = &pAiIn->inp.value.vmeio;
	/* status = dbGetLink(&(pAiIn->inp),DBF_DOUBLE, &(pAiIn->val),0,0); */
	/*If return was succesful then set undefined false*/  

	/* pthread_mutex_lock(&readCPUmutex); */
	if(!strcmp(pvmeio->parm,"PROC")){
		iStatus=four_cpu_numbers(&user_ticks,&nice_ticks,&system_ticks,&idle_ticks);
		if ( iStatus == -1)
			return (2);

		/*if(iStatus==1){
		  printf("prev: idle %.2f nice %.2f system %.2f use %.2f \n",_idle_ticks,_nice_ticks,_system_ticks,_user_ticks);
		  } */

		sum = user_ticks+nice_ticks+system_ticks+idle_ticks;
		if (sum !=0  ){
			_user_ticks   = (float)((double)((double)(user_ticks   * 100.0) / (double)sum));
			_system_ticks = (float)((double)((double)(system_ticks * 100.0) / (double)sum));
			_nice_ticks   = (float)((double)((double)(nice_ticks   * 100.0) / (double)sum));
			_idle_ticks   = (float)((double)((double)(idle_ticks   * 100.0) / (double)sum)); 

			/*if(iStatus==1){
			  printf("new: idle %.2f nice %.2f system %.2f use %.2f \n",_idle_ticks,_nice_ticks,_system_ticks,_user_ticks);
			  }*/

			if(_idle_ticks>201) {
				fprintf(stderr,"CPU load: Wrong Calculation\n");
				/*  dSum=(double)user_ticks+(double)nice_ticks+(double)system_ticks+(double)idle_ticks;
					printf("dSum %f\n",dSum);
					dV1=user_ticks;
					dV2=nice_ticks;
					dV3=system_ticks;
					dV4=idle_ticks;
					dSum=dV1+dV2+dV3+dV4;
					printf("dSum2 %f idleTicks %f %f %f %f %f\n",dSum,(dV4/dSum)*100.0, (dV4*100.0)/dSum,_user_ticks,_system_ticks,_nice_ticks);
					printf("idle %lu nice %lu system %lu use %lu \n",idle_ticks,nice_ticks,system_ticks,user_ticks);
					fIdle=(float)((double)((((double)idle_ticks)   * 100.0) /(double) sum));
					printf("sum %lu sumd %f idle1 %lu idle2 %lu ilded %lu idlef %f\n", sum,dSum,(unsigned long)((idle_ticks  / dSum)*100.0),(unsigned long)(idle_ticks ),(unsigned long)((idle_ticks   * 100)/dSum),fIdle); */
			}

		}/* if sum !=0 */
	}

	/* printf("CPU states:"
	   " %2ld.%ld%% user, %2ld.%ld%% system,"
	   " %2ld.%ld%% nice, %2ld.%ld%% idle",
	   user_ticks / 10UL, user_ticks % 10UL,
	   system_ticks / 10UL, system_ticks % 10UL,
	   nice_ticks / 10UL, nice_ticks % 10UL,
	   idle_ticks / 10UL, idle_ticks % 10UL); */


	if(!strcmp(pvmeio->parm,"IDLE"))
		pAiIn->val=_idle_ticks;
	else if (!strcmp(pvmeio->parm,"NICE"))
		pAiIn->val=_nice_ticks;
	else if (!strcmp(pvmeio->parm,"SYSTEM"))
		pAiIn->val=_system_ticks;
	else if (!strcmp(pvmeio->parm,"USER"))
		pAiIn->val=_user_ticks;
	else
		pAiIn->val=_idle_ticks;

	if(!status) pAiIn->udf = FALSE;
	/* pthread_mutex_unlock(&readCPUmutex); */
	return(2); /* no convertion fron rval to val */
}



/*------------------------------------------------------------*/
/*         MEM record specyfication                           */
/*------------------------------------------------------------*/

static long init_record_aiMEM(void *);
static long read_MEM_load(void *);
struct devMEMLoad_tag{
        long            number;
        DEVSUPFUN       report;
        DEVSUPFUN       init;
        DEVSUPFUN       init_record_ai;
        DEVSUPFUN       get_ioint_info;
        DEVSUPFUN       read_ai;
        DEVSUPFUN       special_linconv;
}devMEMLoad={
        6,
        NULL,
        NULL,
        init_record_aiMEM,
        NULL,
        read_MEM_load,
	NULL
};

epicsExportAddress(dset,devMEMLoad);

static long init_record_aiMEM(void *pIn)
{
    struct aiRecord    *pAiIn = (struct aiRecord *)pIn;
    if(recGblInitConstantLink(&pAiIn->inp,DBF_DOUBLE,&pAiIn->val))
         pAiIn->udf = FALSE;
    return(0);
}


static long read_MEM_load(void *pIn)
{
	struct aiRecord    *pAiIn = (struct aiRecord *)pIn;
	long status=0;
	static unsigned long **mem;

	struct vmeio *pvmeio = &pAiIn->inp.value.vmeio;
	/* status = dbGetLink(&(pAiIn->inp),DBF_DOUBLE, &(pAiIn->val),0,0); */
	/*If return was succesful then set undefined false*/  

	if(!strcmp(pvmeio->parm,"PROC")){
		if (!(mem = meminfo()) ||  mem[meminfo_main][meminfo_total] == 0) {	/* cannot normalize mem usage */
			fprintf(stderr, "Cannot get size of memory from /proc/meminfo\n");
			return(2); /* bofore exit(1); */
		};
	};
	if(!strcmp(pvmeio->parm,"MEMAV"))
		pAiIn->val=mem[meminfo_main][meminfo_total] >> 10;
	else if (!strcmp(pvmeio->parm,"MEMUSED"))
		pAiIn->val=mem[meminfo_main][meminfo_used] >> 10;
	else if (!strcmp(pvmeio->parm,"MEMFREE"))
		pAiIn->val=mem[meminfo_main][meminfo_free] >> 10;
	else if (!strcmp(pvmeio->parm,"MEMSHRD"))
		pAiIn->val=mem[meminfo_main][meminfo_shared] >> 10;
	else if (!strcmp(pvmeio->parm,"MEMBUFF"))
		pAiIn->val=mem[meminfo_main][meminfo_buffers] >> 10;
	else if (!strcmp(pvmeio->parm,"SWAPAV"))
		pAiIn->val=mem[meminfo_swap][meminfo_total] >> 10;
	else if (!strcmp(pvmeio->parm,"SWAPUSED"))
		pAiIn->val=mem[meminfo_swap][meminfo_used] >> 10;
	else if (!strcmp(pvmeio->parm,"SWAPFREE"))
		pAiIn->val=mem[meminfo_swap][meminfo_free] >> 10;
	else if (!strcmp(pvmeio->parm,"SWAPCACH"))
		pAiIn->val=mem[meminfo_total][meminfo_cached] >> 10;
	else 
		pAiIn->val=mem[meminfo_main][meminfo_free] >> 10;

	if(!status) pAiIn->udf = FALSE;
	return(2); /* no convertion fron rval to val */
}

/*-------------- get IP address ---------------------------*/

unsigned long myIpAddr(void)
{
  int s;
  unsigned long r;
  struct ifreq ifr;
  
  if ((s=socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    return 0;

  strcpy(ifr.ifr_name, "eth0");
  
  if (ioctl(s, SIOCGIFADDR, &ifr))
      r = 0;
  else
      r = ((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr.s_addr;

  close(s);
  return r;
}

static long init_record_IP(void *);
static long read_uptime_IP(void *);
struct devIpAddr_tag{
        long            number;
        DEVSUPFUN       report;
        DEVSUPFUN       init;
        DEVSUPFUN       init_record;
        DEVSUPFUN       get_ioint_info;
        DEVSUPFUN       read;
}devIpAddr={
        5,
        NULL,
        NULL,
        init_record_IP,
        NULL,
        read_uptime_IP
};

epicsExportAddress(dset,devIpAddr);

static long init_record_IP(void *pIn)
{
    struct stringinRecord    *pStringIn = (struct stringinRecord *)pIn;
    if(recGblInitConstantLink(&pStringIn->inp,DBF_STRING,&pStringIn->val))
         pStringIn->udf = FALSE;
    return(0);
}


static long read_uptime_IP(void *pIn)
{
    struct stringinRecord    *pStringIn = (struct stringinRecord *)pIn;
  /*     long status; */
    unsigned long iPAddr=0;

    /* status = dbGetLink(&(pStringIn->inp),DBF_STRING, &(pStringIn->val),0,0); */
    /*If return was succesful then set undefined false*/
    /* sprintf(pStringIn->val,"ala %d",get_uptime()); */
    iPAddr=myIpAddr();

    sprintf(pStringIn->val,"%lu.%lu.%lu.%lu",iPAddr%256,(iPAddr/256)%256,(iPAddr/65536)%256,
			(iPAddr/16777216)%256);
       
    /* if(!status)*/  pStringIn->udf = FALSE;
    return(0);
}

/*-------------- get host info ---------------------------*/

static long init_record_Info(void *);
static long read_uptime_Info(void *);
struct devUname_tag{
        long            number;
        DEVSUPFUN       report;
        DEVSUPFUN       init;
        DEVSUPFUN       init_record;
        DEVSUPFUN       get_ioint_info;
        DEVSUPFUN       read;
}devUname={
        5,
        NULL,
        NULL,
        init_record_Info,
        NULL,
        read_uptime_Info
};

epicsExportAddress(dset,devUname);

static long init_record_Info(void *pIn)
{
    struct stringinRecord    *pStringIn = (struct stringinRecord *)pIn;
    if(recGblInitConstantLink(&pStringIn->inp,DBF_STRING,&pStringIn->val))
         pStringIn->udf = FALSE;
    return(0);
}


static long read_uptime_Info(void *pIn)
{
    struct stringinRecord    *pStringIn = (struct stringinRecord *)pIn;
  /* long status; */
    struct utsname name;

    struct vmeio *pvmeio = &pStringIn->inp.value.vmeio;
    /* status = dbGetLink(&(pStringIn->inp),DBF_STRING, &(pStringIn->val),0,0); */
    /*If return was succesful then set undefined false*/
    /* sprintf(pStringIn->val,"ala %d",get_uptime()); */

      if (uname (&name) == -1)
	  {
		  fprintf(stderr,"UP time info: cannot get system name\n");
      }
      else
	  {
		  if(!strcmp(pvmeio->parm,"SYSNAME"))
			  sprintf(pStringIn->val,"%s",name.sysname);
		  else if(!strcmp(pvmeio->parm,"RELEASE"))
			  sprintf(pStringIn->val,"%s",name.release);
		  else if(!strcmp(pvmeio->parm,"VERSION"))
			  sprintf(pStringIn->val,"%s",name.version);
		  else if(!strcmp(pvmeio->parm,"MACHINE"))
			  sprintf(pStringIn->val,"%s",name.machine);
	  }
       
      /* if(!status) */ pStringIn->udf = FALSE;
    return(0);
}

/*-------------- Process Status Check ---------------------------*/
static long init_record_aiStatusCheck(void *);
static long read_StatusCheck(void *);

struct devStatusCheck_tag {
        long            number;
        DEVSUPFUN       report;
        DEVSUPFUN       init;
        DEVSUPFUN       init_record_ai;
        DEVSUPFUN       get_ioint_info;
        DEVSUPFUN       read_ai;
        DEVSUPFUN       special_linconv;
} devStatusCheck= {
	6,
	NULL,
	NULL,
	init_record_aiStatusCheck,
	NULL,
	read_StatusCheck,
	NULL
};

epicsExportAddress(dset,devStatusCheck);

static long init_record_aiStatusCheck(void *pIn)
{
    struct aiRecord    *pAiIn = (struct aiRecord *)pIn;
    if(recGblInitConstantLink(&pAiIn->inp,DBF_DOUBLE,&pAiIn->val))
         pAiIn->udf = FALSE;
    return(0);
}

static long read_StatusCheck(void *pIn)
{
#if 1
    struct aiRecord    *pAiIn = (struct aiRecord *)pIn;
	unsigned int bitint = 0;
    long status=0;

    struct vmeio *pvmeio = &pAiIn->inp.value.vmeio;
	DBADDR *pdbaddr = (DBADDR*)dbGetPdbAddrFromLink(&pAiIn->flnk);

	//process status check.
	if(!strcmp(pvmeio->parm,"USERPROC"))
	{
		bitint = psysmoninfo->getUserProcessStatus();
	}
	else if(!strcmp(pvmeio->parm,"SYSPROC"))
	{
		bitint = psysmoninfo->getSysProcessStatus();
	};
	if(pdbaddr)
	{
		char *pstr = (char*)pdbaddr->pfield;
		strcpy(pstr, psysmoninfo->printBiString(bitint).c_str());
		//printf("Bit:%s\n",pstr);
	};
    pAiIn->val= bitint;
   
    if(!status) pAiIn->udf = FALSE;
#endif

    return(2); /* no convertion from rval to val */
}

/*++-------------- Remote Commander-start ---------------------------*/
static long init_record_RemoteCommanderInfo(void *);
static long read_RemoteCommander(void *);
static int executeCommand( const string command);
struct devRemoteCommander_tag{
        long            number;
        DEVSUPFUN       report;
        DEVSUPFUN       init;
        DEVSUPFUN       init_record;
        DEVSUPFUN       get_ioint_info;
        DEVSUPFUN       read;
}devRemoteCommander={
        5,
        NULL,
        NULL,
        init_record_RemoteCommanderInfo,
        NULL,
        read_RemoteCommander
};

epicsExportAddress(dset,devRemoteCommander);

static int executeCommand( const string command )
{
	FILE *fp = NULL;

	if (command.size() == 0) return(0);
	char strBuff[command.size()];
	strcpy(strBuff, command.c_str());

	char *token;
	token = strtok(strBuff, ",");
	string strCmd = string(token);

	vector<string> vecParam;
	while( token != NULL)
	{
		token = strtok(NULL, ",");
		if(token == NULL) break;
		vecParam.push_back(token);
	};

	size_t paramSize = vecParam.size();

#if 0
	for( size_t i = 0; i < paramSize; i++)
	{
		printf("Param[%d]:%s\n",i, vecParam.at(i).c_str());
	}
#endif

	string host = psysmoninfo->getHostname();
	if (strCmd.compare("OPIUPDATE") == 0)
	{
		if(strstr(host.c_str(), "opi") == NULL) goto BREAK; 
		fp = popen ("/usr/local/opi/bin/opiupdate &", "w");
	}
	else if (strCmd.compare("OPIINSTALL") == 0 )
	{
		if(strstr(host.c_str(), "opi") == NULL) goto BREAK; 
		fp = popen ("/usr/local/opi/bin/opiinstall &", "w");
	}
	else if (strCmd.compare("RUNEFIT") == 0 )
	{
		if(host.compare("data") != 0) goto BREAK;
		string efitCmd;
		if (paramSize == 1)
		{
			efitCmd = string("runefit ")+vecParam.at(0)+string(" &");
		}
		else if (paramSize == 2)
		{
			efitCmd = string("runefit ")+vecParam.at(0)+string(" ")+vecParam.at(1)+string(" &");
		}
		else if (paramSize == 3)
		{
			efitCmd = string("runefit ")+vecParam.at(0)+string(" ")+vecParam.at(1)+string(" ")+vecParam.at(2)+string(" &");
		}
		else if (paramSize == 4)
		{
			efitCmd = string("runefit ")+vecParam.at(0)+string(" ")+vecParam.at(1)+string(" ")+vecParam.at(2)+string(" ")+vecParam.at(3)+string(" &");
		}
		printf("%s\n", efitCmd.c_str());
		//fp = popen (efitCmd.c_str(), "r");
	}

BREAK:
	if(fp) pclose(fp);
	return ((fp == NULL)? 0 : 1);
};

static long init_record_RemoteCommanderInfo(void *pIn)
{
    struct stringinRecord    *pStringIn = (struct stringinRecord *)pIn;
    if(recGblInitConstantLink(&pStringIn->inp,DBF_STRING,&pStringIn->val))
         pStringIn->udf = FALSE;
    return(0);
}

static long read_RemoteCommander(void *pIn)
{
    struct stringinRecord    *pStringIn = (struct stringinRecord *)pIn;
	/* long status; */
    struct vmeio *pvmeio = &pStringIn->inp.value.vmeio;
	//printf("pStringIn Value:%s, %s\n", pStringIn->val, pvmeio->parm);
	if(!pStringIn->val) return(0);
#if 1
	int retVal = 0;
	if(!strcmp(pvmeio->parm,"COMMAND"))
	{
		retVal = executeCommand(string(pStringIn->val));
		sprintf(pStringIn->val,"%s[%d]",pStringIn->val,retVal);
	}
#endif
     pStringIn->udf = FALSE;
    return(0);
}
/*-------------- Remote Commander-end --------------------------*/

/*---------------------------  get DiskUsage-start ------------------------------------*/

static long read_disk_usage(void *);
//static double getDiskUsage(char * parm);
struct devDiskUsage_tag{
        long            number;
        DEVSUPFUN       report;
        DEVSUPFUN       init;
        DEVSUPFUN       init_record_ai;
        DEVSUPFUN       get_ioint_info;
        DEVSUPFUN       read_ai;
        DEVSUPFUN       special_linconv;
}devDiskUsage={
        6,
        NULL,
        NULL,
        init_record_ai,
        NULL,
        read_disk_usage,
		NULL
};

epicsExportAddress(dset,devDiskUsage);

static long read_disk_usage(void *pIn)
{
    struct aiRecord    *pAiIn = (struct aiRecord *)pIn;
    long status=0;

    struct vmeio *pvmeio = &pAiIn->inp.value.vmeio;
    /* status = dbGetLink(&(pAiIn->inp),DBF_DOUBLE, &(pAiIn->val),0,0); */
    /*If return was succesful then set undefined false*/  
    
    //pAiIn->val=getDiskUsage(pvmeio->parm);
    pAiIn->val=psysmoninfo->getDiskUsage(pvmeio->parm);
   
    if(!status) pAiIn->udf = FALSE;
    return(2); /* no convertion fron rval to val */
}


