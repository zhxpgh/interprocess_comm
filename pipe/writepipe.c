/*write pipe test code kanglei2004@163.com 2019-08-08*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <signal.h>


void handle_sigpipe(int sig)
{
	printf("Got a signal %d\n",sig);
	(void) signal(SIGPIPE,SIG_DFL);

}
int main(int argc, char** argv)
{
	int pipe_fd[2];
	pid_t pid;
	char r_buf[100];
	char w_buf[4];
	char* p_wbuf;
	int r_num;
	int cmd;
	
	(void) signal(SIGPIPE,handle_sigpipe);
	memset(r_buf,0,sizeof(r_buf));
	memset(w_buf,0,sizeof(w_buf));
	p_wbuf=w_buf;
	
	if(pipe(pipe_fd)<0)
	{
		printf("pipe create error\n");
		return -1;
	}
	if((pid=fork())==0)
	{
		printf("\n");
		close(pipe_fd[0]); // child close read
		printf("child close fd[0](pipe_read)\n");
		close(pipe_fd[1]); // child close write
		printf("child close fd[1](pipe_write)\n");
		close(pipe_fd[1]); // child close write
		sleep(10);
		exit(0);
	}
	else if(pid>0)
	{
		sleep(1); // wait child close pipe read
		close(pipe_fd[0]);//parent close read
		printf("parent close fd[0](pipe read)\n");
		strcpy(w_buf,"111");
		if(write(pipe_fd[1],w_buf,4)!=-1) //parent write data to pipe write fd
			printf("parent write over %s \n",w_buf);
		else
			printf("parent write pipe error\n");
		close(pipe_fd[1]);//parent close write
		printf("parent close fd[1](pipe_write)\n");
		sleep(6);
	}
}
