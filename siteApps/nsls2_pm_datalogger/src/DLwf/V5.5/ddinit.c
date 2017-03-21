#include "downloadwf.h"
#include <ctype.h>
#include <errno.h>

int dd_recordhost()
{
  char host[32];
  char path[132];
  FILE *file;

  gethostname(host,32);
  sprintf(path,"%s/%s",ddrecptr->datapath, DD_HOST);
  file = fopen(path,"w");
  if( file == NULL ) {
	perror(path);
	return -1;
  } 
  fprintf(file,"%s\n",host);
  fprintf(file,"%s\n",utimestring());
  return 0;
}


int dd_getpar(file,line) 
FILE *file;
char *line;
{
  int i,lent;
  int msecs;
  char sub[32], sub1[32], sline[320];
  WF_PROG *progs;

  sscanf(line,"%s",sub);
  lent = strlen(sub);
  for(i=0;i<lent;i++)
	sub[i] = tolower(sub[i]);

  if( !strcmp(sub,"version") ) {
	sscanf(line,"%s %s",sub, ddrecptr->version);
  }

  if( !strcmp(sub,"pause") ) {
	sscanf( line, "%s %d", sub, &msecs);
	ddrecptr->secs = msecs/1000;
	ddrecptr->msecs = msecs - ddrecptr->secs * 1000;
	if( ddrecptr->msecs < 0 ) ddrecptr->msecs = 0;
	if( ddrecptr->msecs >= 1000 ) {
		++ddrecptr->secs;
		ddrecptr->msecs -= 1000;
	}
  }

  if( !strcmp(sub,"priority") ) {
	sscanf(line,"%s %d",sub, &ddrecptr->priority);
  }

  if( !strcmp(sub,"delay") ) {
	sscanf(line,"%s %d",sub, &ddrecptr->delay);
  }

/*
  if( !strcmp(sub,"ntrigs") ) {
	sscanf(line, "%s %d", sub, &ddrecptr->ntrigs);
  }
*/

  if( !strcmp(sub,"timeouts") ) {
	sscanf(sline, "%s %lf %lf %lf", sub, &ddrecptr->connect_timeout,
	&ddrecptr->read_timeout, &ddrecptr->write_timeout);
  }

  if( !strcmp(sub,"logfile") ) {
	sscanf(line,"%s %s",sub, ddrecptr->logfile);
  }

  if( !strcmp(sub,"dlprog") ) {
	sscanf(line,"%s %s",sub, ddrecptr->dlprog);
  }

  if( !strcmp(sub,"nwf_progs") ) {
	progs = ddrecptr->wfprogs;
	sscanf(line, "%s %d", sub, &ddrecptr->nwfprogs);
	for(i=0;i<ddrecptr->nwfprogs;i++) {
		dd_getline(file,sline);
		sscanf(sline,"%s %s %s %s %d",progs->prog, progs->root, progs->datapath,
			progs->parfile, &progs->used_flag);
		sprintf(progs->cmd, "%s -root %s -datadir %s -parfile %s",
			ddrecptr->dlprog, progs->root, progs->datapath, progs->parfile);
		++progs;
	}
	ddrecptr->nwfused = 0;
	for(i=0;i<ddrecptr->nwfprogs;i++) {
		if( ddrecptr->wfprogs[i].used_flag == 1 ) {
			ddrecptr->wfused_indices[ddrecptr->nwfused] = i;
			++ddrecptr->nwfused;
		}
	}
  }

  if( !strcmp(sub,"ntrigs") ) {
	sscanf(line, "%s %d", sub, &ddrecptr->ntrigs);
	for(i=0;i<ddrecptr->ntrigs;i++) {
		dd_getline(file,sline);
		sscanf(sline,"%s %d",ddrecptr->trig_pv[i].name, &ddrecptr->trig_value[i]);
	}
  }

  if( !strcmp(sub,"nresets") ) {
	sscanf(line, "%s %d", sub, &ddrecptr->nresets);
	if( ddrecptr->nresets > 0 ) {
		for(i=0;i<ddrecptr->nresets;i++) {
			dd_getline(file,sline);
			sscanf(sline,"%s %d",ddrecptr->reset_pv[i].name,&ddrecptr->reset_value[i]);
		}
		ddrecptr->reset_flag = 1;
		ddrecptr->reset_fileflag = 0;
	}
  }

  if( !strcmp(sub,"max_ioerrors") ) {
	sscanf(line, "%s %d", sub, &ddrecptr->max_ioerrors);
  }

  if( !strcmp(sub,"reset_file") ) {
	sscanf(sline, "%s %s", sub, sub1);
	if( !strcmp(sub1,"null") ) {
		ddrecptr->reset_fileflag = 0;
	} else {
		strcpy( ddrecptr->resetfile, sub1 );
		ddrecptr->reset_flag = 1;
		ddrecptr->reset_fileflag = 1;
	}
  }
  return 0;
}

int dd_init()
{
  int i;
  char str[132], sline[324];
  FILE *file;

/*
#ifdef DD_DEBUG
  fprintf(stderr,"Enter dd_init\n");
#endif
*/

  file = fopen(ddrecptr->parpath,"r");
  if (file==NULL) {
	sprintf(str,"dd_init: %s %s", ddrecptr->parpath, strerror(errno));
#ifdef DD_DEBUG
	fprintf(stderr,"%s\n",str);
#endif
	dd_log(1,0,str);
	exit(1);
  }

  ddrecptr->reset_flag = 0;
  ddrecptr->reset_fileflag = 0;

  ddrecptr->nsaves = ddrecptr->ndls = 0;

  ddrecptr->server_id = -1;
  ddrecptr->pid = getpid();

  while(1) {
	dd_getline(file,sline);
	if( sline[0] == 'E' && sline[1] == 'N' && sline[2] == 'D') break;
	dd_getpar(file, sline);
  }
  fclose(file);

  sprintf(ddrecptr->logpath,"%s/%s/%s",DD_TOPDIR, ddrecptr->logdir, ddrecptr->logfile);

  sprintf(sline,"downloadwf started on %s",ddrecptr->host);
  dd_log(0,1,sline);
  sprintf(sline,"Data root: %s",ddrecptr->root);
  dd_log(1,0,sline);
  sprintf(sline,"Data path: %s",ddrecptr->datapath);
  dd_log(1,0,sline);
  sprintf(sline,"Par  file: %s",ddrecptr->parfile);
  dd_log(1,0,sline);
  sprintf(sline,"Par  path: %s",ddrecptr->parpath);
  dd_log(1,0,sline);


  sprintf(sline,"Trigger PV   : %s and value %d",
	ddrecptr->trig_pv[0].name, ddrecptr->trig_value[0]);
  dd_log(1,0,sline);
  sprintf(sline,"Reset PV     : %s and value %d",
	ddrecptr->reset_pv[0].name,ddrecptr->reset_value[0]);
  dd_log(1,0,sline);
  sprintf(sline,"Total waveforms: %d",ddrecptr->nwfprogs);
  dd_log(1,0,sline);
  for(i=0;i<ddrecptr->nwfprogs;i++) {
	sprintf(sline,"    %2d: %s",i, ddrecptr->wfprogs[i].cmd);
	dd_log(1,0,sline);
  }

#ifdef DD_DEB
  fprintf(stderr,"Trigger PV   : %s and value %d\n",
	ddrecptr->trig_pv[0].name, ddrecptr->trig_value[0]);
  fprintf(stderr,"Reset PV     : %s and value %d\n",
	ddrecptr->reset_pv[0].name,ddrecptr->reset_value[0]);
  fprintf(stderr,"Total waveforms: %d\n",ddrecptr->nwfprogs);
  for(i=0;i<ddrecptr->nwfprogs;i++) {
	fprintf(stderr,"    %2d: %s\n",i, ddrecptr->wfprogs[i].cmd);
  }
#endif


  ddsaverec = (HS_SAVEREC *)realloc( ddsaverec, ddrecptr->max_saves * sizeof(HS_SAVEREC) );
  if( ddsaverec == NULL )
	ddrecptr->mem_flag = 0;
  else
	ddrecptr->mem_flag = 1;

  dd_recordhost();

  return 0;
}
