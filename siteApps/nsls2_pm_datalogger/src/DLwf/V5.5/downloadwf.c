
#include <stdio.h>
#include <errno.h>
#include <db_access.h>
#include  <dirent.h>

/* include EPICS headers */
#include <cadef.h>
#define epicsAlarmGLOBAL
#include <alarm.h>


/* Defined in downloadwf.h
#define DD_ROOT "/WFdata/WFdata"
#define DD_DATADIR "Data"
#define DD_LOGDIR "Log"
#define DD_PARFILE "downloadwf.par"
#define DD_LOGFILE "downloadwf.log"
#define DD_TAGFILE "downloadwf.tag"
#define DD_IDHOST "dlidhost.dat"
#define DD_INFOFILE "dlinfo.dat"
#define DD_TOPDIR "/WFdata"
*/


/* Before download waveforms, sleep extra 60 seconds */

/*
#define DD_SIMUL 1
*/

#include <signal.h>
#include <setjmp.h>

jmp_buf begin;

static int dd_curr_save;
static int nioc_errors = 0;

static newpm_flag;

static int nreadings;

#define MSGPV "SR:AI-PM:ArchiverStatus-S"



/*
static int first_read_status = 1;
static int first_reset_status = 1;
*/
int dd_readstatus();
char *strcpy();

static int nlosses = 0;

#define DDMAIN
#include "downloadwf.h"


// reference : http://epics.web.psi.ch/training/caClientLessons/#lesson1
/* Strings describing the connection status of a channel */
const char *channel_state_str[4] = {
    "not found",
    "connection lost",
    "connected",
    "closed"
};

typedef struct {
    chid channel;
    int status;
    struct dbr_ctrl_double info;
    struct dbr_sts_double data;
} epicsDoublePV;


/* There is more than just double values, e.g. long integers */
typedef struct {
    chid channel;
    int status;
    struct dbr_ctrl_long info;
    struct dbr_sts_long data;
} epicsLongPV;

/* Enums are something special: they have a table of strings attached */
typedef struct {
    chid channel;
    int status;
    struct dbr_ctrl_enum info;
    struct dbr_sts_enum data;
} epicsEnumPV;

/* Strings don't have a dbr_ctrl_ structure. They are too primitive */
typedef struct {
    chid channel;
    int status;
    struct dbr_sts_string data;
} epicsStringPV;





void printDoublePV(const epicsDoublePV* pv)
{
    if (ca_state(pv->channel) != cs_conn)
    {
        printf("%s: <%s>\n",
            ca_name(pv->channel), channel_state_str[ca_state(pv->channel)]);
        return;
    }
    /* Print channel name, native channel type,
       value, units, severity, range and setrange */
    printf("%s (%s as DOUBLE) = %#.*f %s  %s  range:[%#.*f ... %#.*f]  setrange:[%#.*f ... %#.*f]\n",

        /* Get name and native type from channel ID */   
        ca_name(pv->channel), dbf_type_to_text(ca_field_type(pv->channel)),
        
        /* Get static info 'precision' and 'units', and dynamic data 'value' */
        pv->info.precision, pv->data.value, pv->info.units,
        
        /* Get dynamic data 'severity' */
        epicsAlarmSeverityStrings[pv->data.severity],
        
        /* Get more static infos */
        pv->info.precision, pv->info.lower_disp_limit,
        pv->info.precision, pv->info.upper_disp_limit,
        pv->info.precision, pv->info.lower_ctrl_limit,
        pv->info.precision, pv->info.upper_ctrl_limit);           
}


void printLongPV(const epicsLongPV* pv)
{
    if (ca_state(pv->channel) != cs_conn)
    {
        printf("%s: <%s>\n",
            ca_name(pv->channel), channel_state_str[ca_state(pv->channel)]);
        return;
    }
    /* Print channel name, native channel type,
       value, units, severity, range and setrange
       Note: In EPICS, LONG means 32 bit, INT and SHORT both mean 16 bit.
    */
    printf("%s (%s as LONG) = %i %s  %s  range:[%i ... %i]  setrange:[%i ... %i]\n",

        /* This is similar to the above case but uses long everywhere */   
        ca_name(pv->channel), dbf_type_to_text(ca_field_type(pv->channel)),
        pv->data.value, pv->info.units,
        epicsAlarmSeverityStrings[pv->data.severity],
        pv->info.lower_disp_limit,
        pv->info.upper_disp_limit,
        pv->info.lower_ctrl_limit,
        pv->info.upper_ctrl_limit);
}

int printEnumPV(const epicsEnumPV* pv)
{
#if 0
    int i;
    if (ca_state(pv->channel) != cs_conn)
    {
        printf("%s: <%s>\n",
            ca_name(pv->channel), channel_state_str[ca_state(pv->channel)]);
        return;
    }
    /* Print channel name, native channel type,
       value as number and as string, severity, and all defined strings.
       
       Note that enums don't have units and instead of a range,
       they have a list of strings - one for each state (max 16).
    */
    printf("%s (%s as ENUM) = %i = \"%s\"  %s  %i strings:",
    
        /* Get name and native type from channel ID */   
        ca_name(pv->channel), dbf_type_to_text(ca_field_type(pv->channel)),
        
        /* Get dynamic data 'value' and convert it to a string using the
           static information 'strs'. Never forget to check if value
           is within bounds or unexpected "segmentation fault" crashes
           may happen.
        */
        pv->data.value,
        pv->data.value < pv->info.no_str ? pv->info.strs[pv->data.value] : "",
            
        /* Get dynamic data 'severity' */
        epicsAlarmSeverityStrings[pv->data.severity],
        
        /* Get all defined stings for this channel (from static information) */
        pv->info.no_str);
    for (i = 0; i < pv->info.no_str; i++)
    {
        printf("%s\"%s\"", i>0 ? "," : "", pv->info.strs[i]);
    }
    printf ("\n");
#endif

    return pv->data.value;
}


void printStringPV(const epicsStringPV* pv)
{
    if (ca_state(pv->channel) != cs_conn)
    {
        printf("%s: <%s>\n",
            ca_name(pv->channel), channel_state_str[ca_state(pv->channel)]);
        return;
    }
    /* Print channel name, native channel type,
       value and severity */
    printf("%s (%s as STRING) = \"%s\"  %s\n",
        ca_name(pv->channel), dbf_type_to_text(ca_field_type(pv->channel)),
        pv->data.value, 
        epicsAlarmSeverityStrings[pv->data.severity]);           
}


/* Connect a PV to a channel */
#define casearch(name, pv) SEVCHK(\
    (pv).status = (ca_search((name), &(pv).channel)), name)

/* Wrapper macros to read 'info' and 'data' fields of a PV
   Call cainfo once during initialization to get all available infos.
   Later, call caget to get only the dynamic information (value and severity).
   Both macros need an additiional DBF_* argument to know the data type.
   The dbf_type_to_DBR_* macros from db_access.h are used to translate
   DBF_* to the full data structure (either DBR_CTRL_* or DBR_STS_*).
   Both macros do nothing when the channel is disconnected.
*/

/* get full information about a PV */
#define cainfo(pv, type) SEVCHK(\
    (pv).status = (ca_state((pv).channel) != cs_conn ? ECA_DISCONN : \
    ca_get(dbf_type_to_DBR_CTRL(type), (pv).channel, &(pv).info)), ca_name((pv).channel))

/* get only usually changing information about a PV */
#define caget(pv, type) SEVCHK(\
    (pv).status = (ca_state((pv).channel) != cs_conn ? ECA_DISCONN : \
    ca_get(dbf_type_to_DBR_STS(type), (pv).channel, &(pv).data)), ca_name((pv).channel))







char *ns2wf_root()
{
  char *ret_val;

  ret_val = getenv("NS2WF");
  return ret_val;
}


void dd_saveidhost()
{
  char path[132],hsys[8];
  FILE *file;

  sprintf(path,"%s/%s",ddrecptr->datapath,DD_IDHOST);
  gethostname(ddrecptr->host,80);

  file = popen("uname -m","r");
  ddrecptr->sys64_flag = 1;
  if( file != NULL ) {
	fgets(hsys, 8, file);
	if( strstr(hsys,"64") != NULL ) ddrecptr->sys64_flag = 1;
	else ddrecptr->sys64_flag = 0;
	pclose(file);
  }

  file = fopen(path,"w");
  if( file != NULL ) {
	fprintf(file,"%d %s\n",getpid(),ddrecptr->host);
	fprintf(file,"at %s\n",utimestring());
	fclose(file);
  } /* else part: ??? */

  file = fopen(ddrecptr->infopath,"a");
  if( file != NULL ) {
	fprintf(file,"%d %s at %s\n",getpid(),ddrecptr->host,utimestring());
	fclose(file);
  } /* else part: ??? */

  sprintf(path,"Host name: %s",ddrecptr->host);
  dd_log(1,0,path);

}

int dd_getserverid()
{
  int id, np, ret;
  char sline[132];

  ret = utl_checkproc_unique_1(HIS_SERVER, &np);
  if( ret == -1 ) {
	sprintf(sline,"There are %d hisctrl_server running",np);
	dd_log(1,0,sline);
	return -1;
  }
  id = utl_getprocid( HIS_SERVER );
  return id;
}


void dd_onusr1(sig)
int sig;
{
  int i, j, id;
  char sline[132];
  char *buff;

  signal( sig, dd_onusr1 );

  id = utl_gethisserverid();
  if( id == -1 ) return;
  ddrecptr->server_id = id;
  ddrecptr->shm_id = shmget((key_t)ddrecptr->pid, HS_SHMSIZE, 0666);
  if( ddrecptr->shm_id < 0 ) {
	sprintf(sline,"shmget: %s",strerror(errno));
	dd_log(1,0,sline);
	return;
  }
  ddrecptr->shm = shmat(ddrecptr->shm_id, NULL, 0);
  if( ddrecptr->shm == (char *)-1 ) {
	sprintf(sline,"shmat: %s",strerror(errno));
	dd_log(1,0,sline);
	return;
  }

  buff = ddrecptr->shm;
  memcpy(buff, (char *)(&shm_struct), sizeof(SHM_STRUCT));
  buff += sizeof(SHM_STRUCT);
  j = ddrecptr->icurrent - 1;
  if( j < 0 ) j = ddrecptr->nsaves-1;
  for(i = 0; i<ddrecptr->nsaves; i++) {
	memcpy(buff, ddsaverec+j, sizeof(HS_SAVEREC));
	j--;
	if( i < 0 ) j = ddrecptr->nsaves-1;
  }
  shmdt(ddrecptr->shm);
  return;
}

void dd_savepathinfo()
{
  return;
}

int main(argc,argv)
int argc;
char *argv[];
{
  int i,iarg,status,res,inc;
  TS_STR ts;
  char *ch,sline[320],ssline[320];
  char *prog = "downloadwf";
  HS_SAVEREC *sr;
  int loop=0;
  int dly_cnt=0;

  /*
     RF dump status      : SR:C31-{AI}Sys:Status_S0-I.B5   0:OK, 1:DUMP
     SDI Glitch detected : SR:C31-{AI}Sys:Status_S0-I.B7   0:NO glitch, 1:Glitch detected
     SR:C31-{AI}Sys:Status-PMReady-I
                 'PM Ready OK',2048  'PM Not Ready'0
   */

  epicsDoublePV beamcurrent;
  epicsEnumPV rfDumps, sdiGlitchs;

  double search_timeout = 5.0; /* seconds */
  double get_timeout = 1.0; /* seconds */
  double loop_period = 5.0; /* seconds */

  int rf_dump, sdi_glitch;
  int nwfused;  


  signal(SIGUSR1, dd_onusr1);
  ddrecptr = &ddrec;
  strcpy( ddrecptr->datadir, DD_DATADIR);
  strcpy( ddrecptr->logdir, DD_LOGDIR);
  strcpy( ddrecptr->parfile, DD_PARFILE);
  ddrecptr->test_mode = 0;
  ch = ns2wf_root();
  if( ch == NULL )
	strcpy( ddrecptr->root, DD_ROOT);
  else
	strcpy( ddrecptr->root, ch );

  ddrecptr->max_saves = HS_MAXSAVES;
  ddrecptr->cmd_max_saves = -1;
  ddrecptr->max_ioerrors = MAX_IOERRORS;

#ifdef DD_SIMUL
  ddrecptr->max_saves = 12;
#endif

  strcpy(shm_struct.prog_name, argv[0]);
  strcpy(ddrecptr->prog_name,argv[0]);
  strcpy(ddrecptr->prog_line,argv[0]);
  iarg = 1;
  if( argc >= 3 ) {
	while (iarg < argc - 1) {
		inc = 0;
		if( !strcmp(argv[iarg], "-root") ) {
			strcpy(ddrecptr->root, argv[iarg+1]);
			inc = 2;
			sprintf(sline," %s %s",argv[iarg], argv[iarg+1]);
			strcat(ddrecptr->prog_line,sline);
		}
		if( !strcmp(argv[iarg], "-parfile") ) {
			strcpy(ddrecptr->parfile, argv[iarg+1]);
			inc = 2;
			sprintf(sline," %s %s",argv[iarg], argv[iarg+1]);
			strcat(ddrecptr->prog_line,sline);
		}
		if( !strcmp(argv[iarg], "-datadir") ) {
			strcpy(ddrecptr->datadir, argv[iarg+1]);
			inc = 2;
			sprintf(sline," %s %s",argv[iarg], argv[iarg+1]);
			strcat(ddrecptr->prog_line,sline);
		}
		if( !strcmp(argv[iarg], "-sleeptime") ) {
			ddrecptr->secs = atoi(argv[iarg+1]);
			inc = 2;
			sprintf(sline," %s %s",argv[iarg], argv[iarg+1]);
			strcat(ddrecptr->prog_line,sline);
		}
		if( !strcmp(argv[iarg], "-max_saves") ) {
			ddrecptr->max_saves = ddrecptr->cmd_max_saves = atoi(argv[iarg+1]);
			if( ddrecptr->max_saves > SHM_MAXBUFFERS )
				ddrecptr->max_saves = SHM_MAXBUFFERS;
			inc = 2;
			sprintf(sline," %s %s",argv[iarg], argv[iarg+1]);
			strcat(ddrecptr->prog_line,sline);
		}
		if( !strcmp(argv[iarg], "-test") ) {
			ddrecptr->test_mode = 1;
			inc = 1;
			sprintf(sline," %s",argv[iarg]);
			strcat(ddrecptr->prog_line,sline);
		}
		if( inc == 0 ) {
			fprintf(stderr,"Invalue argument: %s\n",argv[iarg]);
			exit(1);
		} else
			iarg += inc;
	}
  }

  sprintf( ddrecptr->datapath, "%s/%s", ddrecptr->root, ddrecptr->datadir);
  sprintf( ddrecptr->logpath, "%s/%s/%s.log", DD_TOPDIR, ddrecptr->logdir, prog);
  sprintf( ddrecptr->infopath, "%s/%s", ddrecptr->datadir, DD_INFOFILE);
  sprintf( ddrecptr->parpath, "%s/%s", ddrecptr->datapath, ddrecptr->parfile);

  dd_saveidhost();
/*
  dd_setshm();
*/
  dd_savepathinfo();
  dd_init();

#ifdef DD_DEB
  fprintf(stderr,"downloadwf started at %s\n",utimestring());
  fprintf(stderr,"Data root: %s\n",ddrecptr->root);
  fprintf(stderr,"Data path: %s\n",ddrecptr->datapath);
  fprintf(stderr,"Par  file: %s\n",ddrecptr->parfile);
  fprintf(stderr,"Log  path: %s\n",ddrecptr->logpath);
  fprintf(stderr,"Par  path: %s\n",ddrecptr->parpath);
#endif

/*
  sprintf(sline,"downloadwf started");
  dd_log(0,1,sline);
  sprintf(sline,"Data root: %s",ddrecptr->root);
  dd_log(1,0,sline);
  sprintf(sline,"Data path: %s",ddrecptr->datapath);
  dd_log(1,0,sline);
  sprintf(sline,"Par  file: %s",ddrecptr->parfile);
  dd_log(1,0,sline);
  sprintf(sline,"Par  path: %s",ddrecptr->parpath);
  dd_log(1,0,sline);
*/


  /* Step1: initialize channel access and search for all channels. */
  ca_task_initialize();

  casearch("SR:C03-BI{DCCT:1}I:Total-I", beamcurrent);
  casearch("SR:C31-{AI}Sys:Status_S0-I.B5", rfDumps);
  casearch("SR:C31-{AI}Sys:Status_S0-I.B7", sdiGlitchs);

  SEVCHK(ca_pend_io(search_timeout), "casearch");
  cainfo(beamcurrent, DBF_DOUBLE);
  cainfo(rfDumps, DBF_ENUM);
  cainfo(sdiGlitchs, DBF_ENUM);
  
  caget(beamcurrent, DBF_DOUBLE);
  caget(rfDumps, DBF_ENUM);
  caget(sdiGlitchs, DBF_ENUM);

  SEVCHK(ca_pend_io(get_timeout), "cainfo");
  printDoublePV(&beamcurrent);
  printEnumPV(&rfDumps);
  printEnumPV(&sdiGlitchs);



/* Because of the situation at present, this version of program only reads one trig from
   active interlock system */

  res = dd_readstatus(DBR_TIME_INT, &status, ts, 0, 0);
  while(1) {
        
    caget(beamcurrent, DBF_DOUBLE);
    caget(rfDumps, DBF_ENUM);
    caget(sdiGlitchs, DBF_ENUM);
    SEVCHK(ca_pend_io(get_timeout), "cainfo")
    //printout
    printDoublePV(&beamcurrent);
    rf_dump = printEnumPV(&rfDumps);
    sdi_glitch = printEnumPV(&sdiGlitchs);
    printf("Status=%d, %d\n", rf_dump, sdi_glitch);

	res = dd_readstatus(DBR_TIME_INT, &status, ts, 1, 1);
	if (res != 0) {
		++nioc_errors;
		if( nioc_errors > ddrecptr->max_ioerrors ) {
		    sprintf(ssline,"\nAt %s, there are %d pv-reading errors, RESTART THE PROGRAM",
			utimestring(),nioc_errors);
		    dd_log(1,0,ssline);
		    sprintf(ssline,"Restart %s",ddrecptr->prog_name);
		    dd_log(1,0,ssline);
		    sprintf(ssline,"nohup %s &",ddrecptr->prog_line);
		    dd_log(1,0,ssline);
		    system(ssline);
		    exit(1);
		}
		sleep(ddrecptr->secs);
		continue;
	} else {
		++nreadings;
		nioc_errors = 0;
	}

	if( strcmp( ts, ddrecptr->old_trig_timestamp ) ) {
		newpm_flag = 1;
		strcpy(ddrecptr->old_trig_timestamp,ts);
	} else {
		newpm_flag = 0;
	}
	strcpy(ddrecptr->trig_timestamp,ts);
	if( status == DD_NORMAL ) {
		sprintf(sline,"caput %s %s, %d sec", MSGPV, "PM ready", loop++);
        sleep(1);
        if(loop > 60) loop = 0;
	} else {
	    if( newpm_flag == 0 ) {
		  sprintf(sline,"caput %s %s, %d sec", MSGPV, "PM not ready", loop++);
          sleep(1);
          if(loop > 1200) loop = 0;
	    } else {
		  sprintf(sline,"caput %s %s", MSGPV, "PM signal detected, wait......");
	    }
	}
	system(sline);

#ifdef DD_DEB
	if( nreadings % 20 == 0 ) {
		fprintf(stderr,"At %s, the trigger status is %d\n",utimestring(),status);
	}
#endif
	if( nreadings % 20 == 0 ) {
		sprintf(ssline,"At %s, the trigger status is %d",utimestring(),status);
		dd_log(1,0,ssline);
	}

#ifdef DD_SIMUL
	status = DD_DETECTED;
#endif

	if( status == DD_DETECTED && newpm_flag == 1) {
        dly_cnt = ddrecptr->delay;
        //sleep(ddrecptr->delay);
        //monitoring status every 1 second
        for(i=0; i<ddrecptr->delay; i++) {
          sleep(1);
          sprintf(sline,"caput %s %s %d sec", MSGPV, "PM signal detected, wait..", dly_cnt-i );
          system(sline);
          res = dd_readstatus(DBR_TIME_INT, &status, ts, 1, 1);
          if( status != DD_DETECTED ) {            
            loop=0; //clearing loop value
            break;
          }  
        }

        /* read rf dump status */
        caget(rfDumps, DBF_ENUM);
        caget(sdiGlitchs, DBF_ENUM);
        SEVCHK(ca_pend_io(get_timeout), "cainfo")
        rf_dump = printEnumPV(&rfDumps);
        sdi_glitch = printEnumPV(&sdiGlitchs);
        printf("Status=%d, %d\n", rf_dump, sdi_glitch);

        if(rf_dump == 0 && sdi_glitch != 0) {
            printf("Glitch detected.... Aarchiving 3 files\n");
            nwfused  = 3;
        }
        else {
            printf("RF trip and beam dump... Archiving all files\n");
            nwfused = ddrecptr->nwfused;
        }

		res = dd_readstatus(DBR_TIME_INT, &status, ts, 1, 1);
#ifdef DD_SIMUL
		status = DD_DETECTED;
#endif
		if( status != DD_DETECTED ) {
			sprintf(sline,"caput %s %s", MSGPV, "Test mode, no download");
			system(sline);
            		sleep(2);
			continue;
		}

		sprintf(ssline,"At %s, the trigger status is %d DETECTED",utimestring(),status);
		dd_log(1,0,ssline);
		
		nlosses++;
		sprintf(ssline,"So far we have had %d beam losses and downloads",nlosses);
		dd_log(1,0,ssline);

/*
		if( nlosses > 500 ) {
			sprintf(ssline,"Too many losses, program exits at %s",utimestring());
			dd_log(1,0,ssline);
			exit(1);
		}
		dd_log(1,0,ssline);
*/

		sprintf(sline,"caput %s %s", MSGPV, "Beam dumped, Start to download PMs");
		system(sline);
        sleep(2);
        sprintf(sline,"caput %s %s %d", MSGPV, "Start to run the following programs", ddrecptr->nwfused );
        system(sline);
//		sprintf(ssline,"Start to run the following %d programs:",ddrecptr->nwfused);
		dd_log(1,0,ssline);


//		for(i=0;i<ddrecptr->nwfused;i++) {
        for(i=0; i<nwfused; i++) {
			sprintf(sline,"%s -timestamp %s", ddrecptr->wfprogs[i].cmd,
				ddrecptr->trig_timestamp);
			if( ddrecptr->test_mode ) {
				strcat( sline," -test yes");
			}
			sprintf(ssline,"%s: %s",utimestring(),sline);
			dd_log(2,0,ssline);
			system(sline);

			sr = ddsaverec + ddrecptr->icurrent;
			strcpy(sr->prog, ddrecptr->wfprogs[i].prog);
			sprintf(sr->save_dir, "%s/%s",ddrecptr->wfprogs[i].root,
				ddrecptr->datapath);
			strcpy(sr->save_time,ddrecptr->trig_timestamp);
			strcpy(sr->cmd, sline);
			if( ddrecptr->nsaves < ddrecptr->max_saves ) ++ddrecptr->nsaves;
			++ddrecptr->icurrent;
			if( ddrecptr->icurrent >= ddrecptr->max_saves ) {
				ddrecptr->icurrent = 0;
			}

            sprintf(sline,"caput %s %s %d", MSGPV, "Please WAIT! writing to file", nwfused-i);
            system(sline);

		}
/*
		sprintf(sline,"caput %s %s", MSGPV, "Waveform download complete");
		system(sline);
*/
        do {
            res = dd_readstatus(DBR_TIME_INT, &status, ts, 1, 1);
            sleep(1);

            sprintf(sline,"caput %s %s", MSGPV, "Download and Save complete! RESET....");
            system(sline);
        } while (status == DD_DETECTED );


#ifdef RESET_TRIGGER
		dd_log(1,0,"Reset the trigger");
		if( first_reset_status ) {
			dd_resetstatus(0);
			first_reset_status = 0;
		} else {
			dd_resetstatus(1);
		}
#endif /* RESET_TRIGGER */

	}
	sleep(ddrecptr->secs);
  }

    /* Last step: free all channel access resources */
    ca_task_exit();

    return 0;
}

