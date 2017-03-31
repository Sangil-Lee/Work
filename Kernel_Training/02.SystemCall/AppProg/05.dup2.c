#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[])
{
	int fd;
	char *buf = "This is a test output file.\n";
	
	if(argc<2) {
		fprintf(stderr, "Usage: %s filename\n", argv[0]);
		exit(1);
	}
	
	if((fd=open(argv[1] ,O_WRONLY|O_CREAT|O_TRUNC, 0644))==-1) {
		perror("open");
		exit(2);
	}
	
	//File redirection.
	dup2(fd, 1);
	close(fd);
		
	write(1, buf, strlen(buf));
	printf("Hello...\n");
	fprintf(stdout, "Hi~~~\n");
	
	return 0;
}
