#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void * thread_function(void *);

int commonCounter=0;

int main(void)
{
	pthread_t tid1, tid2;
	void *thread_result;

	if(pthread_create(&tid1, NULL, thread_function,  "thread1")!=0) {
		perror("pthread_create");
		exit(1);
	}
	if(pthread_create(&tid2, NULL, thread_function, "thread2")!=0) {
		perror("pthread_create");
		exit(1);
	}

	if(pthread_join(tid1, &thread_result)!=0) {
		perror("pthread_join");
		exit(1);
	}
	if(pthread_join(tid2, &thread_result)!=0) {
		perror("pthread_join");
		exit(1);
	}

	printf(" thread result : %s\n", (char *) thread_result);
	return 0;
}

void * thread_function(void * arg)
{
	int temp;
	int i, j;
	char buffer[80];

	printf("thread_function called\n");
	for(i=0; i<20; i++) {
		sprintf(buffer, "%s : commonCounter : from %d to ", (char*)arg, commonCounter);
		write(1, buffer, strlen(buffer));

		temp=commonCounter;

// delay
		for(j=0; j<500000; j++);

		commonCounter=temp+1;

		sprintf(buffer, " %d\n", commonCounter);
		write(1, buffer, strlen(buffer));

		for(j=0; j<500000; j++);

	}
	pthread_exit("thread end");
}
