#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/types.h>
#include <signal.h>

#define	DEVICE_FILENAME	"/dev/sk"

void file_test(void)
{
	int fd;
	int rx_data;
	int ret;
	
	fd = open(DEVICE_FILENAME, O_RDWR);
	if(fd < 0) {
		perror("/dev/sk file open error!!");
		exit(-1);
	} else printf("/dev/sk file open ok!!\n");

	
	printf("app => read request!!\n");
	ret = read(fd, &rx_data, sizeof(int));
	printf("app => read done(rx_data:%d, ret:%d)!!\n", rx_data, ret);

	close(fd);	
}

int main(void)
{
	pid_t pid1, pid2, pid3;
	int i;
	
	pid1 = fork();
	switch(pid1) {
		case -1 :
			printf("fork_1 error!!\n");
			exit(-1);
		case 0 :
			for(i=1; i<15; i++) {
				printf("\tChild1(%d)_%d!!\n", getpid(), i);
				sleep(1);
			}
			exit(0);
	}

	file_test();
	
	for(i=1; i<10; i++) {
		printf("Parent(%d)_%d!!\n", getpid(), i);
		sleep(1);
	}
	
	
	kill(pid1, SIGKILL);
	return 0;
}

