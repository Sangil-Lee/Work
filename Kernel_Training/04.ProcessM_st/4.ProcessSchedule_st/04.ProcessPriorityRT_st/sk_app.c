#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/types.h>
#include <signal.h>

#include <sched.h>
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

	struct sched_param param;
	
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


	setpriority(PRIO_PROCESS, 0, -10);
 	//setpriority(PRIO_PROCESS, pid1, -10);
	setpriority(PRIO_PROCESS, pid2, -10);

#if 1
	for(i=1; ; i++) {
		printf("Parent_%d!!\n", i);

		//SCHED_OTHER:0, SCHED_FIFO:1, SCHED_RR:2
		.............
		printf("-------------->Parent_Policy:%d, prio:%d\n", ............. );
		sched_getparam(pid1, &param);
		printf("-------------->Child1_Policy:%d, prio:%d\n", sched_getscheduler(pid1), param.sched_priority);
		sched_getparam(pid2, &param);
		printf("-------------->Child2_Policy:%d, prio:%d\n", sched_getscheduler(pid2), param.sched_priority);

#if 1
		param.sched_priority = i%100;	//0~99, big val : high priority
		........... (pid1, &param);
#endif

		file_test();
		sleep(1);
	}
#endif
	close(fd);	
	
	kill(pid1, SIGKILL);
	kill(pid2, SIGKILL);

	return 0;
}

