#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct _INFO {
	char name[20];
	int age;
	float height;
} INFO;

int main(int argc, char *argv[])
{
	INFO data[5] = {"kim",20,170.1F,
					"lee",25,180.3F,
					"park",21,176.2F,
					"choi",28,167.9F,
					"kang",25,172.5F};
	int fd;
	int i;

	//Direct use system call insterface.
	if((fd=open(argv[1], O_WRONLY|O_CREAT))==-1)
	{
		perror("open");
		exit(1);
	}
	for(i=0; i<5; i++)
	{
		if(write(fd, &data[i], sizeof(INFO)) != sizeof(INFO)) {
			perror("write");
			close(fd);
			exit(2);
		}
	}
	close(fd);
	return 0;
}
