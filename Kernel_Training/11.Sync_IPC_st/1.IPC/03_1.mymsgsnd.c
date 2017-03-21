#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int main(void)
{
	int msgid,n;
	char buf[1024];
	struct mymsgbuf {
		long	m_type;
		char m_str[1024];
	} sendbuf;
	
	if((msgid=msgget(123400,IPC_CREAT|0666))==-1)
	{
		perror("msgget");
		exit(1);
	}
	while (1) 
	{
		printf(" Input --> ");
		fgets(buf, 1024, stdin);
		sendbuf.m_type=1;
		strcpy(sendbuf.m_str,buf);
		msgsnd(msgid,&sendbuf, strlen(sendbuf.m_str),0);
		if(!strncmp(buf,"end",3))
				break;
	}
	sleep(1);
	return 0;
}
