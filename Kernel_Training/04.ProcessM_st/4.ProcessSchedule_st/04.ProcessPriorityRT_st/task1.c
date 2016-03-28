#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include <sched.h>


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
	int i;
	struct sched_param param;

	//nice(10);
	//nice(-5);

	..............	//0~99, big val : high priority
	..................

	//SCHED_OTHER:0, SCHED_FIFO:1, SCHED_RR:2
	printf("------------------->Task1_Policy:%d\n", sched_getscheduler(0));

	for(i=1; ; i++) {
		printf("\t---->Task1_%d!!\n", i);
		file_test();
		sleep(1);
	}

	return 0;
}
