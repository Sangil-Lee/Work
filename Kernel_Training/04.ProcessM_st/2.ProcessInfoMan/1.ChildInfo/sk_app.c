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
			for(i=1; ; i++) {
				printf("\tChild1(%d)_%d!!\n", getpid(), i);
				sleep(1);
			}
			exit(0);
	}
	
	pid2 = fork();
	switch(pid2) {
		case -1 :
			printf("fork_2 error!!\n");
			exit(-1);
		case 0 :
			for(i=1; ; i++) {
				printf("\t\tChild2(%d)_%d!!\n", getpid(), i);
				sleep(2);
			}
			exit(0);
	}
	
	pid3 = fork();
	switch(pid3) {
		case -1 :
			printf("fork_3 error!!\n");
			exit(-1);
		case 0 :
			for(i=1; ; i++) {
				printf("\t\t\tChild3(%d)_%d!!\n", getpid(), i);
				sleep(3);
			}
			exit(0);
	}
	

	sleep(1);
	file_test();
	
	for(i=1; i<5; i++) {
		printf("Parent(%d)_%d!!\n", getpid(), i);
		sleep(1);
	}
	
	
	kill(pid1, SIGKILL);
	kill(pid2, SIGKILL);
	kill(pid3, SIGKILL);
	return 0;
}

