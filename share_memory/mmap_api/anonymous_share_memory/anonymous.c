/* anonymous share memory test code, read&write, kanglei2004@163.com 2019-08-15*/
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
	char name[4];
	int age;
	
}people;
int main(int argc, char** argv) // map a normal file as shared mem:
{
	int fd,i;
	people *p_map;
	char temp;
	p_map = (people*)mmap(NULL,sizeof(people)*10,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,fd,0);
        if(fork() == 0)
	{
		// child process
		printf("child process\n");
		sleep(1);
		for(i=0;i<10;i++)
		{
			printf( "child read the %d people, name: %s age %d;\n",i,(*(p_map+i)).name, (*(p_map+i)).age );
		}
		exit(0);

	}
	temp ='a';
	for(i=0;i<10;i++)
	{
		memcpy((*(p_map+i)).name, &temp,2);
		(*(p_map+i)).age = 20 + i;
		temp += 1;
	}
	printf("parent process\n");
	sleep(5);
	printf("parent munmap\n");
	munmap(p_map,sizeof(people)*10);
	return 0;
}

