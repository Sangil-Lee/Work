#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

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
	int j, sum;
	double prod;

	for(i=1; ; i++) {
		printf("\t---->Task2_%d!!\n", i);
		file_test();
		for(j=1, sum=0, prod=1; j<5000000; j++) {
			sum += j;
			prod *= j;
		}
		//sleep(1);
	}

	return 0;
}
