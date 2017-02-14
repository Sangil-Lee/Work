#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

void main()
{
  key_t key = 45678;
  int id, size;
  char *shm;

  size = 65536;
  if( (id = shmget( (key_t)key, size, IPC_CREAT | 0666 )) < 0 ) {
	perror("shmget");
	exit(1);
  }
  if( (shm = shmat(id, NULL, 0)) < 0 ) {
	perror("shmat");
	exit(1);
  }
  sleep(4);

  printf("shared memory formed\n");
}


