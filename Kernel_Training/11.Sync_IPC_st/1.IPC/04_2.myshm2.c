#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char **argv) { 
	void *shmaddr;
	struct shmid_ds shm_stat;
	int i,j;
	int shmid, semid;

	if(argc!=2) {
		fprintf(stderr, "Usage : myshmtest1 shmkey\n");
		exit(1);
	}
	printf("myshmtest2 is started.......\n");
	if((shmid=shmget((key_t)atoi(argv[1]),128, IPC_CREAT|0666)) == -1 )  {
		perror("shmget failed !!");         
		exit(1);
	}
	if((shmaddr=shmat(shmid,(void *)0,0))==(void *)-1) {
		perror("shmat failed !!");        
		exit(1);
	}

	for(i=0;i<500;i++) {
		sprintf((char *) shmaddr,"%d", getpid());
		for(j=0; j<30000000; j++);
		if(getpid() == atoi(shmaddr))
			putchar('O');
		else
			putchar('X');
		fflush(stdout);
	}

	/* shmid 공유 메모리 정보를 얻어 shm_stat에 저장
	if(shmctl(shmid,IPC_STAT,&shm_stat) == -1 ){
		perror("shmctl error ");
		exit(1);
	}
	*/

	if(shmdt(shmaddr)  == -1) {
		perror("shmdt failed !!");        
		exit(1);
	}

/* shm_stat.shm_cpid 프로세스에게 SIGINT시그널 보냄
	kill(shm_stat.shm_cpid, SIGINT);
	*/
 
	exit(0);
}

