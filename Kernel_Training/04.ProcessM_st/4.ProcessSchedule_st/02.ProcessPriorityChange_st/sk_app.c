#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/types.h>
#include <signal.h>

#include <sys/time.h>
#include <sys/resource.h>

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

	
	//printf("app => read request!!\n");
	ret = read(fd, &rx_data, sizeof(int));
	printf("app => read done(rx_data:%d, ret:%d)!!\n", rx_data, ret);

	close(fd);	
}

int main(void)
{
	int fd;
	int rx_data;
	int ret;
	pid_t pid1, pid2;
	int i;
	
	printf("Main Start(pid:%d)!!\n", getpid());
	
	pid1 = fork();
	switch(pid1) {
		case -1 :
			printf("fork_1 error!!\n");
			exit(-1);
		case 0 :
			execl("./task1", "task1", NULL);
			exit(0);
	}
	
	pid2 = fork();
	switch(pid2) {
		case -1 :
			printf("fork_2 error!!\n");
			exit(-1);
		case 0 :
			execl("./task2", "task2", NULL);
			exit(0);
	}
	
	for(i=1; ; i++) {
		printf("Parent_%d(prio:%d)!!\n", i, getpriority(PRIO_PROCESS, 0));
		file_test();
		sleep(1);
	}
	close(fd);	
	
	kill(pid1, SIGKILL);
	kill(pid2, SIGKILL);

	return 0;
}

