#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	printf("Main Start!!\n");

	FILE *fp = fopen("./tmp1","w");
	char name[50] = "Lee";

	int jumsu[3] = {70,70,70};

	float h = 234.3f;

	if(fp==NULL)
	{
		perror("fopen failed!!\n");
		exit(1);
	};

	if (fwrite(name, sizeof(char), 50, fp) != 50)
	{
		perror("fwrite1 failed!!");
		fclose(fp);
		exit(2);
	};

	if (fwrite(jumsu, sizeof(int), 3, fp) != 3)
	{
		perror("fwrite2 failed!!");
		fclose(fp);
		exit(3);
	};

	if (fwrite(&h, sizeof(float), 1, fp) != 1)
	{
		perror("fwrite3 failed!!");
		fclose(fp);
		exit(4);
	};

	fclose(fp);
	printf("Successful Main!!\n");

	return 0;
}
