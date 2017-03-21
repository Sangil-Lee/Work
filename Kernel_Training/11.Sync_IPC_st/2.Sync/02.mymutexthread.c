#include <sys/types.h>	
#include <stdio.h>	
#include <stdlib.h>
#include <string.h>		
#include <pthread.h>
#include <errno.h>	

#define LOOP_MAX 20	

pthread_mutex_t mutexid;
int commonCounter= 0;

void * thread_function(void *);

int main(void) 
{
  pthread_t tID_1;
  pthread_t tID_2;

#ifndef NOMUTEX
  if(pthread_mutex_init(&mutexid, NULL) != 0 ) {
  	perror("pthread_mutex_init");
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
  
#ifndef NOMUTEX
  pthread_mutex_destroy(&mutexid);
#endif
}

void * thread_function(void *arg) 
{
  int loopCount;
  int temp;	
  char buffer[80];
  int i;

  for (loopCount = 0; loopCount < LOOP_MAX; loopCount++) {
#ifndef NOMUTEX
    pthread_mutex_lock(&mutexid);
#endif

    //sprintf (buffer, "Common counter(%d) :  from %d to ", (int)arg, commonCounter);
    sprintf (buffer, "Common counter(%u) :  from %d to ", (unsigned int)pthread_self(), commonCounter);
    write(1, buffer, strlen(buffer));

    temp = commonCounter;
	for(i=0; i<500000; i++);		// for delay
    commonCounter = temp + 1;

    sprintf (buffer, "%d\n", commonCounter);
    write(1, buffer, strlen(buffer));

	for(i=0; i<500000; i++);		// for delay
#ifndef NOMUTEX
	pthread_mutex_unlock(&mutexid);
#endif
  }
}
