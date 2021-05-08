/*atomicity pipe test code kanglei2004@163.com 2019-08-09*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>


#define PIPE_BUFFER_SIZE 64*1024 //the pipe buffer size is different in diff OS
#define PIPE_READ_SIZE 16*1024
int main(int argc, char** argv)
{
	int pipe_fd[2];
	pid_t pid;
	char r_buf[PIPE_BUFFER_SIZE];
	char w_buf[PIPE_BUFFER_SIZE];
	char* p_wbuf;
	int r_num;
	int w_num;
	int cmd;
	
	
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
		printf("child close fd[1](pipe_write)\n");
		close(pipe_fd[1]); // child close write
		sleep(1);//确保父进程关闭写端
		while(1)
		{
			r_num = read(pipe_fd[0],r_buf,PIPE_READ_SIZE);
			printf("child read num is %d\n",r_num);
			sleep(1);
			if(r_num == 0)
				break;
		}
		close(pipe_fd[0]); // child close read
		printf("child close fd[0](pipe_read)\n");
		exit(0);
	}
	else if(pid>0)
	{
		close(pipe_fd[0]);//parent close read
		printf("parent close fd[0](pipe read)\n");
		strncpy(w_buf,"1",PIPE_BUFFER_SIZE);
		while(1)
		{
			if((w_num = write(pipe_fd[1],w_buf,PIPE_BUFFER_SIZE))!=-1) //parent write data to pipe write fd
				printf("parent write num is %d\n",w_num);
			else
			{
				printf("parent write pipe error\n");
				break;
			}

		}
		close(pipe_fd[1]);//parent close write
		printf("parent close fd[1](pipe_write)\n");
		sleep(6);
	}
}
