#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

//int open(const char *pathname, int flags);
//int open(const char *pathname, int flags, mode_t mode);
//ssize_t write(int fd, const void *buf, size_t count);


int main(void)
{
	//printf("Main Start!!\n");
	fwrite("Main Start!!\n", sizeof(char), (size_t)strlen("Main Start!!"), stdout);
	write(1, "Main Start!!\n",strlen("Main Start!!"));

	int fd = open("/dev/pts/2", O_WRONLY);

	char name[50] = "Hello Lee!!";
	float h = 234.3f;

	if(fd < 0 )
	{
		perror("open failed!!\n");
		exit(1);
	};

	if (write(fd,name, strlen(name)) != strlen(name))
	{
		perror("write1 failed!!");
		close(fd);
		exit(2);
	};

	if (write(fd,"Hi~~\n", 5) != 5)
	{
		perror("write1 failed!!");
		close(fd);
		exit(2);
	};

	close(fd);
	printf("Successful Main!!\n");

	return 0;
}
