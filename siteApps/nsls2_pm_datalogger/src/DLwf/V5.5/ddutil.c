#include "downloadwf.h"
#include <sys/types.h>
#include <sys/time.h>

static int microsec;

int elapsetime( val )
int val;
{
  struct timeval tp;
  struct timezone tzp;

  gettimeofday(&tp,&tzp);
  if( !val )
        return (microsec = 1000000*tp.tv_sec + tp.tv_usec );
  else
        return (1000000*tp.tv_sec + tp.tv_usec - microsec);
}


void cutnewline(line)
char *line;
{
  char *pnt;

  pnt = line ;
  while( *pnt ) {
        if( *pnt == '\n' ) {
                *pnt = 0;
                break;
        }
        ++pnt;
  }
}


char *utimestring()
{
  long i;
  char *ch;

  i = time(0);
  ch = ctime( &i );
  cutnewline( ch );
  return ch;
}


void dd_getline( file, sline )
FILE *file;
char *sline;
{
  fgets( sline, 320, file );
  while( sline[0]==';' || sline[0] =='/' || sline[0]=='*' || sline[0]=='#' )
        fgets( sline, 320, file );
}

void dd_log(layer, time_flag, str)
int layer;
int time_flag;
char *str;
{
  char sp[16];
  FILE *errfile;

  switch (layer) {
	case 0:
		strcpy(sp,"\n\n");
		break;
	case 1:
		strcpy(sp, "    ");
		break;
	case 2:
		strcpy(sp, "        ");
		break;
	case 3:
		strcpy(sp, "            ");
		break;
  }

  if( (errfile = fopen( ddrecptr->logpath, "a") ) != NULL ) {
	if( time_flag == 1) {
        	fprintf(errfile,"%s%s: %s\n", sp, utimestring(), str);
	} else {
        	fprintf(errfile,"%s %s\n", sp, str);
	}
        fclose(errfile);
  }
}


int dd_realloc( ptr, size )
void **ptr;
size_t size;
{
  char sline[128];
  *ptr = realloc(*ptr,size);
  if( *ptr == NULL ) {
	sprintf(sline, "Memory allocation (%d bytes) error", (int)size);
	dd_log(2,1,sline);
	return -1;
  } else return 0;
}


int dd_malloc( ptr, size )
void **ptr;
size_t size;
{
  char sline[128];
  *ptr = malloc(size);
  if( *ptr == NULL ) {
	sprintf(sline, "Memory allocation (%d bytes) error", (int)size);
	dd_log(2,1,sline);
	return -1;
  } else return 0;
}

