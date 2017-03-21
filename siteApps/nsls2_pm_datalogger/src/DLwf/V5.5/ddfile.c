#include "downloadwf.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <hdf5.h>

char *bw_getsavepath()
{
  int i;
  char yearname[8];
  static char savepath[132];

  yearname[0] = 'Y';
  for(i=0;i<4;i++)
	yearname[i+1] = bwrecptr->time_stamp[i];
  yearname[i+1] = 0;

  sprintf(savepath,"%s/%s",bwrecptr->datapath, yearname);
  return savepath;
}

FILE *bw_openregfile(fname,mode)
char *fname;
char *mode;
{
  char sline[132];
  FILE *file;

  file = fopen(fname,mode);
  if( file == NULL ) {
	sprintf(sline,"%s: %s",fname,strerror(errno));
	bw_log(sline);
	fprintf(stderr,"%s\n",sline);
  }
  return file;
}


int bw_savewf_toregfile()
{
  int et1, et2;
  char path[160];
  char *dirname;
  FILE *file;

#ifdef BW_DEB
  fprintf(stderr,"**** Enter bw_savewf_toregfile\n");
#endif

  dirname = bw_getsavepath();
  sprintf(path, "%s/%s.reg",dirname, bwrecptr->time_stamp);
  file = bw_openregfile( path, "w" );
  if( file == NULL ) return -1;
  fwrite(&h5_saverec, sizeof(SAVEREC), 1, file);
  fwrite(bpm_pvnames, sizeof(PV_NAME), bwrecptr->npvs, file);

  et1 = elapsetime(0);
  fwrite(h5_data, h5_saverec.nbytes_per_set, 1, file);
  et2 = elapsetime(1);
  fprintf(stderr,"Elapse time for saving wf to reg file: %d\n", et2);

  fclose(file);
	
#ifdef BW_DEB
  fprintf(stderr,"%s saved\n",path);
#endif
  return 0;
}


int bw_savewf(connected)
int connected;
{
  int i, ipv, itp, ibpm, ier;
  int et1;
  float *fval;
  BW_PV *pvs;

#ifdef BW_DEB
  fprintf(stderr,"**** Enter bw_savewf\n");
#endif

  ipv = 0;
  pvs = bpm_pvs;
  elapsetime(0);
  for( itp = 0; itp < bwrecptr->nbpmreadings; itp++) {
	for( ibpm = 0; ibpm < bwrecptr->nbpms; ibpm++ ) {
		ier = bw_readpvs_intohdf5data(1, pvs, ipv, connected);
		++pvs;
		++ipv;
	}
  }
  et1 = elapsetime(1);
  fprintf(stderr,"Elapse time for reading wf: %d\n", et1);

#if defined(REG_FILE) || defined(BOTH_FILE)
  bw_savewf_toregfile();
#endif
#if defined(HDF_FILE) || defined(BOTH_FILE)
  bw_savewf_toh5file();
#endif

/*
  fval = (float *)(h5_data);
  for(i=200;i<201;i++) {
	fprintf(stderr,"%10.5f ", *(fval+i));
  }
  fprintf(stderr,"\n\n");
  for(i=100000;i<100001;i++) {
	fprintf(stderr,"%10.5f ", *(fval+i));
  }
  fprintf(stderr,"\n");
*/
#ifdef BW_DEB
  fprintf(stderr,"**** Exit bw_savewf\n");
#endif
  return 0;
}
