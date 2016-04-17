#include <stdio.h>

int main (void)
{
	int a = 9;

	printf("Mod:%d, Div:%d\n", (unsigned char)a%256, (unsigned char)a/256);
	return 0;
}
