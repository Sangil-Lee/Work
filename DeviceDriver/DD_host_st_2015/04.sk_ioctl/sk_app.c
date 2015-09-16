#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#define	DEVICE_FILENAME	"/dev/sk"

int main(void)
{
	int fd;
	int tx_data=33, rx_data;
	int ret;
	unsigned int flag;

	fd = open(DEVICE_FILENAME, O_RDWR);
	if(fd < 0) {
		perror("/dev/sk file open error!!");
		exit(-1);
	} else printf("/dev/sk file open ok!!\n");

	printf("app => write request(tx_data:%d)!!\n", tx_data);
	ret = write(fd, &tx_data, sizeof(int));
	//ret = write(fd, &tx_data, 4);
	printf("app => write done(ret:%d)!!\n", ret);

	lseek(fd, 0, SEEK_SET);

	printf("app => read request!!\n");
	ret = read(fd, &rx_data, sizeof(int));
	printf("app => read done(rx_data:%d, ret:%d)!!\n", rx_data, ret);

	printf("app => ioctl request(cmd:1)!!\n");
	flag = 111;

	//int ioctl(int d, int request, ...);
	//request -> flag
	ret = ioctl(fd, 2, flag);
	printf("app => ioctl done(ret:%d)!!\n", ret);

	close(fd);
	return 0;
}

