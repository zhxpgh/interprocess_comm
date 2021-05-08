#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>

void new_op(int,siginfo_t*,void*);

int main(int argc,char**argv)
{

	struct sigaction act;

	union sigval mysigval;

	int i;

	int sig;

	pid_t pid;

	char data[11];

	memset(data,0,sizeof(data));

	for(i=0;i < 10;i++)

		data[i]='2';

	mysigval.sival_ptr=data;

	if(argc < 2)

	{

		printf("sigsend2self signum\n");

		return 1;

	}

	sig=atoi(argv[1]);

	pid=getpid();

	sigemptyset(&act.sa_mask);

	act.sa_sigaction=new_op;//三参数信号处理函数

	act.sa_flags=SA_SIGINFO;//信息传递开关

	if(sigaction(sig,&act,NULL) < 0)

	{

		printf("install sigal error\n");

	}

	while(1)

	{

		sleep(2);

		printf("wait for the signal\n");

		sigqueue(pid,sig,mysigval);//向本进程发送信号，并传递附加信息

	}

	return 0;

}

void new_op(int signum,siginfo_t *info,void *myact)//三参数信号处理函数的实现
{
	int i;
	printf("\n");
	for(i=0;i<10;i++)
	{
		printf("%c\n ",(*( (char*)((*info).si_ptr)+i))); //注意这个地方的指针名，
                // 这个与siginfo_t的结构有关，具体请参看man sigaction中的相关信息
	}
	printf("handle signal %d over;\n",signum);
}
