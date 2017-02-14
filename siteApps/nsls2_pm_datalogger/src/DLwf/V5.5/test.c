#include <stdio.h>
main(argc, argv)
int argc;
char *argv[];
{
  int i;

  fprintf(stderr,"Number of args: %d\n",argc);
  for(i=0;i<argc;i++) {
	fprintf(stderr,"%d: %s\n",i,argv[i]);
  }
  sleep(3);
}
