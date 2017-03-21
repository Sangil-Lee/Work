#include  <stdlib.h>
#include  <sys/stat.h>
#include  <dirent.h>
#include  <stdio.h>
#include <signal.h>
static int is_backup_root;

extern char *strcpy(char *,char *);

int isnfs(const char *path)
{
  struct stat buf;

  return (!stat(path, &buf));
}

int isnotempty(const char* path)
{
  int            ret_val;
  DIR            *dirstr;
  struct dirent  *nextent;
  int   i;
  if (!(dirstr = opendir(path))) return 0;
  for (i=0; i<3; ++i) {
	nextent = readdir(dirstr);
  }
  ret_val = nextent ? 1 : 0;
  closedir( dirstr );
  return ret_val;
}


char *ns2wf_root()
{
  char *ret_val;

  ret_val = getenv("NS2WF");
  return ret_val;
}

