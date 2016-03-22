#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/types.h>
#include <string.h>

#define	DEVICE_FILENAME	"/dev/sk"

static int my_data_1=10;
static int null_data[2000]={0,};
static int my_data_2=100;

void file_test(int *addr)
{
	int fd;
	int ret;
	
	fd = open(DEVICE_FILENAME, O_RDWR);
	if(fd < 0) {
		perror("/dev/sk file open error!!");
		exit(-1);
	} else printf("/dev/sk file open ok!!\n");

	
	printf("app => read request!!\n");
	ret = read(fd, addr, sizeof(int));
	printf("app => read done(rx_data:%d, ret:%d)!!\n", *addr, ret);

	close(fd);	
}

int main(void)
{
	int i;

	memset(null_data, 22, sizeof(null_data));
	file_test(&my_data_1);
	file_test(&null_data[1000]);
	file_test(&my_data_2);
	
	while(1);
	
	return 0;
}

