#include <stdio.h>
#include <stdlib.h>

typedef struct _INFO {
	char name[20];
	int age;
	float height;
} INFO;

int main(int argc, char *argv[])
{
	INFO data;
	FILE *fp;
	int cnt=0;

	if((fp=fopen(argv[1], "r"))==NULL)
	{
		perror("fopen");
		exit(1);
	}
	while(1)
	{
		//if(fread(&data[i], sizeof(INFO), 1, fp) != 1) {
		fread(&data, sizeof(INFO), 1, fp);
		if(feof(fp)) break;
		printf("name:%s, age:%d, height:%.1f\n", data.name, data.age, data.height);
		cnt++;
	}
	fclose(fp);
	return 0;
}
