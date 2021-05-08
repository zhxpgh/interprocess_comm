/* signal send with info code kangle2004@163.com 2019-08-11*/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main(int argc,char**argv)
{
	int sig,i;
	pid_t pid;
	union sigval val;
	if(argc < 3)
	{
		printf("./sigsend pid signum\n");
		return 1;
	}

	char data[11];
	memset(data,0,sizeof(data));
	for(i=0;i < 10;i++)
		data[i]='2';
	val.sival_ptr=data;
	pid=atoi(argv[1]);
	sig=atoi(argv[2]);
	//val.sival_int = 8;
	sigqueue(pid,sig,val);
	usleep(200);
	return 0;
}
