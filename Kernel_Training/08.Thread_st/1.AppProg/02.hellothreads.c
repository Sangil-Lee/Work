#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREAD	3

void *hello_thread(void *arg)
{
	int i;
	
	for(i=1; i<=3; i++) {
		printf("Thread %d: Hello, World(%d)!\n", (int)arg, i);
	}
	
	return arg;
}

int main(void)
{
	pthread_t tid[NUM_THREAD];
	int status, i;
	
	for(i=0; i<NUM_THREAD; i++) {
		status = pthread_create(&tid[i], NULL, hello_thread, (void *)i);
		if(status != 0) {
			perror("thread create");
			exit(1);
		}
	}
	
	printf("Main Thread End!!\n");
	pthread_exit(NULL);
	
    return 0;
}