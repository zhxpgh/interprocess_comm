/*pipe test code kanglei2004@163.com 2019-08-09*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

int handle_cmd(int cmd)
{
	if(cmd < 0 || cmd > 256)
	{
		printf("child got a invaild cmd %d\n",cmd);
		return -1;
	}
	else
		printf("child got a cmd %d\n",cmd);
	return 0;
}

int main(int argc, char** argv)
{
	int pipe_fd[2];
	pid_t pid;
	char r_buf[100];
	char* w_buf[4];
	char* p_wbuf;
	int r_num;
	int cmd;
	int child_exit = 1;
	
	
	memset(r_buf,0,sizeof(r_buf));
	
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
		sleep(3);//确保父进程关闭写端
		while(child_exit)
		{
			read(pipe_fd[0],r_buf,4);
			cmd = atoi(r_buf);
			if(cmd == 0)
			{
				printf("child got exit cmd\n");
				child_exit = 0;
			}
			else
			{
				if(handle_cmd(cmd) !=0)
					return 1;
			}
			sleep(1);

		}
		close(pipe_fd[0]); // child close read
		printf("child close fd[0](pipe_read)\n");
		exit(0);
	}
	else if(pid>0)
	{
		close(pipe_fd[0]);//parent close read
		printf("parent close fd[0](pipe read)\n");
		w_buf[0]="003";
		w_buf[1]="005";
		w_buf[2]="777";
		w_buf[3]="000";
		int i = 0;
		for(i=0;i<4;i++)
			write(pipe_fd[1],w_buf[i],4);
		close(pipe_fd[1]);//parent close write
		printf("parent close fd[1](pipe_write)\n");
		sleep(6);
	}
}
