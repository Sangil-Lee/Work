#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	printf("Main Start!!\n");

	FILE *fp = fopen("./tmp1","r");
	char name[50];
	int jumsu[3];
	float h;

	if(fp==NULL)
	{
		perror("fopen failed!!\n");
		exit(1);
	};

	if (fread(name, sizeof(char), 50, fp) != 50)
	{
		perror("fread1 failed!!");
		fclose(fp);
		exit(2);
	};

	if (fread(jumsu, sizeof(int), 3, fp) != 3)
	{
		perror("fread2 failed!!");
		fclose(fp);
		exit(3);
	};

	if (fread(&h, sizeof(float), 1, fp) != 1)
	{
		perror("fread3 failed!!");
		fclose(fp);
		exit(4);
	};

	printf("Name:%s, Jumsu: %d,%d,%d, H: %f Main!!\n", name, jumsu[0],jumsu[1],jumsu[2],h);

	fclose(fp);
	printf("Successful Main!!\n");

	return 0;
}
