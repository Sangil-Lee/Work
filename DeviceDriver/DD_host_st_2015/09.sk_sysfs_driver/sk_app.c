#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#include <string.h>		//memset()
#include "sk.h"

#define	DEVICE_FILENAME	"/dev/sk0"

int main(void)
{
	int fd;
	int tx_data=33, rx_data;
	int ret;
	unsigned int flag;

	SK_INFO info;

	fd = open(DEVICE_FILENAME, O_RDWR);
	if(fd < 0) {
		perror("/dev/sk0 file open error!!");
		exit(-1);
	} else printf("/dev/sk0 file open ok!!\n");

	printf("\nIOCTL Test Start..\n");

	memset(&info, 0, sizeof(info));
	ioctl(fd, SK_LED_OFF, &info);
	printf("Press Enter to continue...\n");
	getchar();

	ioctl(fd, SK_LED_ON, &info);
	printf("Press Enter to continue...\n");
	getchar();
	ioctl(fd, SK_GETSTATE, &info);
	printf("Press Enter to continue...\n");
	getchar();

	memset(&info, 0, sizeof(info));
	info.size = 7;
	ioctl(fd, SK_READ, &info);
	printf("IOCTL Read Data => %s\n", info.buff);
	printf("Press Enter to continue...\n");
	getchar();

	memset(&info, 0, sizeof(info));
	info.size = 9;
	strcpy(info.buff, "123456789");
	ioctl(fd, SK_WRITE, &info);
	printf("Press Enter to continue...\n");
	getchar();

	memset(&info, 0, sizeof(info));
	info.size = 5;
	strcpy(info.buff, "abcd");
	ioctl(fd, SK_RW, &info);

	printf("\nIOCTL Test End..\n");

	close(fd);
	return 0;
}

