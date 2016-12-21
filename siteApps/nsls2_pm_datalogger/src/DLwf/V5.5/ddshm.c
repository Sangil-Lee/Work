#include "downloadwf.h"
#include <sys/types.h>
#include <sys/time.h>
#include <errno.h>

int dd_setshm()
{
  char sline[132];
  int siz;

  siz = HS_SHMSIZE;
  ddrecptr->shm_flag = 0;
  if( (ddrecptr->shmid = shmget( (key_t)DL_SHMKEY, siz, IPC_CREAT | 0666 )) < 0 ) {
	sprintf(sline,"shmget: %s",strerror(errno));
	dd_log(0,0,sline);
	fprintf(stderr,"%s\n",sline);
	return -1;
  }
  if( (ddrecptr->shm = shmat(ddrecptr->shmid, NULL, 0)) < 0 ) {
	sprintf(sline,"shmat: %s",strerror(errno));
	dd_log(0,0,sline);
	fprintf(stderr,"%s\n",sline);
	return -1;
  }
  ddrecptr->shm_flag = 1;
  return 0;
}
