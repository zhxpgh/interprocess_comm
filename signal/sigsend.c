/* signal recevie code kangle2004@163.com 2019-08-11*/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc,char**argv)
{
	int sig;
	pid_t pid;
	union sigval val;
	if(argc < 3)
	{
		printf("./sigsend pid signum\n");
		return 1;
	}
	pid=atoi(argv[1]);
	sig=atoi(argv[2]);
	sigqueue(pid,sig,val);
	usleep(200);
	return 0;
}
