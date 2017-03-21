#include <sys/types.h>	
#include <stdio.h>	
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>	
#include <semaphore.h>	

#define LOOP_MAX 20	

pthread_mutex_t mutexid;
int commonCounter= 0;

sem_t semid;

void * thread_function(void *);

int main(void) 
{
  pthread_t tID_1;
  pthread_t tID_2;
  int res;

#ifndef NOSEM
    res = sem_init(&semid, 0, 1);
    if (res != 0) {
        perror("sem_init");
        exit(1);
    }
#endif

  if ((pthread_create( &tID_1, NULL, thread_function, (void *)1)) ||
      (pthread_create( &tID_2, NULL, thread_function, (void *)2))) {
    perror("pthread_create");
    exit (errno);
  }

  pthread_join(tID_1, (void **)NULL);
  pthread_join(tID_2, (void **)NULL);

#ifndef NOSEM
    sem_destroy(&semid);
#endif
}

void * thread_function(void *arg) 
{
  int loopCount;
  int temp;	
  char buffer[80];
  int i;

  for (loopCount = 0; loopCount < LOOP_MAX; loopCount++) {
#ifndef NOSEM
	sem_wait(&semid);
#endif

    sprintf (buffer, "Common counter(%d) :  from %d to ", (int)arg, commonCounter);
    write(1, buffer, strlen(buffer));

    temp = commonCounter;
	for(i=0; i<500000; i++);		// for delay
    commonCounter = temp + 1;

    sprintf (buffer, "%d\n", commonCounter);
    write(1, buffer, strlen(buffer));

	for(i=0; i<500000; i++);		// for delay
#ifndef NOSEM
	sem_post(&semid);
#endif
  }
}
