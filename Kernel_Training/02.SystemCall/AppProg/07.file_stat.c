#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

int main(int argc, char *argv[])
{
	struct stat buf;
	char *msg;

	if(argc < 2) {
		fprintf(stderr, "Usage : %s filename\n", argv[0]);
		exit(1);
	}
	
	if(lstat(argv[1],&buf)==-1) {
			if(errno==ENOENT) 
				fprintf(stderr, "%s is not found\n", argv[1]);
			else if(errno==EACCES)
				fprintf(stderr, "No permission for %s\n",argv[1]);
			exit(2);
	}
	
	if(S_ISREG(buf.st_mode)) msg = "regular file";
	else if(S_ISDIR(buf.st_mode)) msg = "directory";
	else if(S_ISCHR(buf.st_mode)) msg = "chracter special file";
	else if(S_ISBLK(buf.st_mode)) msg = "block special file";
	else if(S_ISFIFO(buf.st_mode)) msg = "pipe of FIFO";
	else if(S_ISLNK(buf.st_mode)) msg = "symbolic link";
	else if(S_ISSOCK(buf.st_mode)) msg = "socket";
	
	printf("\n%s is %s.\n", argv[1], msg);
		
	return 0;
}
