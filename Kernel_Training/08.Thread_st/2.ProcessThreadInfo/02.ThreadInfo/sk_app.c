#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/types.h>
#include <signal.h>

#include <pthread.h>

#define	DEVICE_FILENAME	"/dev/sk"

int fd;

void *th_func_1(void *arg)
{
	int rx_data;
	int ret;
	int i;
	for(i=1; i<2; i++) {
		sleep(1);
		printf("\tChild1(%d, %d)_%d!!\n", getpid(), (int)pthread_self(), i);
		printf("app_chld1 => read request!!\n");
		ret = read(fd, &rx_data, sizeof(int));
		printf("app_chld1 => read done(rx_data:%d, ret:%d)!!\n", rx_data, ret);				
	}
}

void *th_func_2(void *arg)
{
	int rx_data;
	int ret;
	int i;
	for(i=1; i<2; i++) {
		sleep(3);
		printf("\tChild2(%d, %d)_%d!!\n", getpid(), (int)pthread_self(), i);
		printf("app_chld2 => read request!!\n");
		ret = read(fd, &rx_data, sizeof(int));
		printf("app_chld2 => read done(rx_data:%d, ret:%d)!!\n", rx_data, ret);				
	}
}

int main(void)
{
	int rx_data;
	int ret;
	pthread_t tid1, tid2;
	int i;
	
	printf("Main Start(pid:%d)!!\n", getpid());
	
	fd = open(DEVICE_FILENAME, O_RDWR);
	if(fd < 0) {
		perror("/dev/sk file open error!!");
		exit(-1);
	} else printf("/dev/sk file open ok!!\n");

	for(i=1; i<2; i++) {
		//sleep(1);
		printf("Parent(%d, %d)_%d!!\n", getpid(), (int)pthread_self(), i);
		printf("app_parent => read request!!\n");
		ret = read(fd, &rx_data, sizeof(int));
		printf("app_parent => read done(rx_data:%d, ret:%d)!!\n", rx_data, ret);
	}
	
	pthread_create(&tid1, NULL, th_func_1, NULL);
	pthread_create(&tid2, NULL, th_func_2, NULL);


	for(i=1; i<2; i++) {
		//sleep(1);
		printf("Parent(%d, %d)_%d!!\n", getpid(), (int)pthread_self(), i);
		printf("app_parent => read request!!\n");
		ret = read(fd, &rx_data, sizeof(int));
		printf("app_parent => read done(rx_data:%d, ret:%d)!!\n", rx_data, ret);
	}

	sleep(2);

	for(i=1; i<2; i++) {
		//sleep(1);
		printf("Parent(%d, %d)_%d!!\n", getpid(), (int)pthread_self(), i);
		printf("app_parent => read request!!\n");
		ret = read(fd, &rx_data, sizeof(int));
		printf("app_parent => read done(rx_data:%d, ret:%d)!!\n", rx_data, ret);
	}

	pthread_join(tid1, (void**)NULL);
	pthread_join(tid2, (void**)NULL);

	close(fd);	
	printf("Parent(%d, %d) Doned!!\n", getpid(), (int)pthread_self());
	
	return 0;
}

