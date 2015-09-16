#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

//int open(const char *pathname, int flags);
//int open(const char *pathname, int flags, mode_t mode);
//ssize_t write(int fd, const void *buf, size_t count);


int main(void)
{
	printf("Main Start!!\n");

	int fd = open("./tmp2", O_RDONLY);

	char name[50];
	int jumsu[3];
	float h;

	if(fd < 0 )
	{
		perror("open failed!!\n");
		exit(1);
	};

	if (read(fd,name, sizeof(char)*50) != 50)
	{
		perror("read1 failed!!");
		close(fd);
		exit(2);
	};

	if (read(fd, jumsu, sizeof(int)*3) != sizeof(int)*3)
	{
		perror("read2 failed!!");
		close(fd);
		exit(3);
	};

	if (read(fd, &h, sizeof(float)) != sizeof(float))
	{
		perror("read3 failed!!");
		close(fd);
		exit(4);
	};

	printf("Name:%s, Jumsu: %d,%d,%d, H: %f !\n", name, jumsu[0],jumsu[1],jumsu[2],h);

	close(fd);
	printf("Successful Main!!\n");

	return 0;
}
