#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/types.h>
#include <signal.h>

#define	DEVICE_FILENAME	"/dev/sk"

int main(void)
{
	int fd;
	int rx_data;
	int ret;
	pid_t pid1, pid2, pid3;
	int i;
	
	printf("Main Start(pid:%d)!!\n", getpid());
	
	fd = open(DEVICE_FILENAME, O_RDWR);
	if(fd < 0) {
		perror("/dev/sk file open error!!");
		exit(-1);
	} else printf("/dev/sk file open ok!!\n");
	
	pid1 = fork();
	switch(pid1) {
		case -1 :
			printf("fork_1 error!!\n");
			exit(-1);
		case 0 :
			for(i=1; i<2; i++) {
				sleep(1);
				printf("\tChild1(%d)_%d!!\n", getpid(), i);
				printf("app_chld1 => read request!!\n");
				ret = read(fd, &rx_data, sizeof(int));
				printf("app_chld1 => read done(rx_data:%d, ret:%d)!!\n", rx_data, ret);				
			}
			exit(0);
	}
	
	pid2 = fork();
	switch(pid2) {
		case -1 :
			printf("fork_2 error!!\n");
			exit(-1);
		case 0 :
			for(i=1; i<2; i++) {
				sleep(2);
				printf("\t\tChild2(%d)_%d!!\n", getpid(), i);
				printf("app_chld2 => read request!!\n");
				ret = read(fd, &rx_data, sizeof(int));
				printf("app_chld2 => read done(rx_data:%d, ret:%d)!!\n", rx_data, ret);
			}
			exit(0);
	}
	
	
	for(i=1; i<5; i++) {
		sleep(1);
		printf("Parent(%d)_%d!!\n", getpid(), i);
		printf("app_parent => read request!!\n");
		ret = read(fd, &rx_data, sizeof(int));
		printf("app_parent => read done(rx_data:%d, ret:%d)!!\n", rx_data, ret);
	}
	close(fd);	
	
	kill(pid1, SIGKILL);
	kill(pid2, SIGKILL);

	return 0;
}

