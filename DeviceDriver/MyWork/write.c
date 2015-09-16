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

	int fd = open("./tmp2", O_WRONLY | O_CREAT);

	char name[50] = "Lee";
	int jumsu[3] = {70,70,70};
	float h = 234.3f;

	if(fd < 0 )
	{
		perror("open failed!!\n");
		exit(1);
	};

	if (write(fd,name, sizeof(char)*50) != 50)
	{
		perror("write1 failed!!");
		close(fd);
		exit(2);
	};

	if (write(fd, jumsu, sizeof(int)*3) != sizeof(int)*3)
	{
		perror("write2 failed!!");
		close(fd);
		exit(3);
	};

	if (write(fd, &h, sizeof(float)) != sizeof(float))
	{
		perror("write3 failed!!");
		close(fd);
		exit(4);
	};

	close(fd);
	printf("Successful Main!!\n");

	return 0;
}
