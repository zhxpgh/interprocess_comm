/*system v share memory read teet code, kanglei2004@163.com 2019-08-15*/
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct
{
	char name[4];
	int age;
}people;

int main(int argc, char** argv)
{
	int shm_id,i;
	key_t key;
	char temp;
	people *p_map;
	char* name = "/dev/shm/myshm2";
	key = ftok(name,0);
	if(key==-1)
	{
		perror("ftok error");
		printf("%s",strerror(errno));
		return 1;
	}
	shm_id=shmget(key,4096,IPC_CREAT);
	if(shm_id==-1)
	{
		perror("shmget error");
		return 1;
	}
	p_map=(people*)shmat(shm_id,NULL,0);
	for(i = 0;i<10;i++)
	{
		printf("the people name is %s, and the age is %d\n",(*(p_map+i)).name,(*(p_map+i)).age);
	}
	if(shmdt(p_map)==-1)
		perror(" detach error ");
	return 0;
}
