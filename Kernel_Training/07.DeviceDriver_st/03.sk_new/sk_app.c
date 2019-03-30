#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

//#define	APP_TEST
#ifdef APP_TEST
#include <android/log.h>
#define	LOG_TAG	"SOO"
#define	LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#else 
#define	LOGI(...) printf(__VA_ARGS__)
#endif


#define	DEVICE_FILENAME	"/dev/sk0"

int main(void)
{
	int fd;
	int tx_data=33, rx_data;
	int ret;
	unsigned int flag;

	fd = open(DEVICE_FILENAME, O_RDWR);
	if(fd < 0) {
		perror("/dev/sk0 file open error!!");
		exit(-1);
	} else LOGI("/dev/sk0 file open ok!!\n");

	LOGI("app => write request(tx_data:%d)!!\n", tx_data);
	ret = write(fd, &tx_data, sizeof(int));
	LOGI("app => write done(ret:%d)!!\n", ret);

	lseek(fd, 0, SEEK_SET);

	LOGI("app => read request!!\n");
	ret = read(fd, &rx_data, sizeof(int));
	LOGI("app => read done(rx_data:%d, ret:%d)!!\n", rx_data, ret);

	LOGI("app => ioctl request(cmd:1)!!\n");
	ret = ioctl(fd, 1, flag);
	LOGI("app => ioctl done(ret:%d)!!\n", ret);

	close(fd);
	return 0;
}

