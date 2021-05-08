/*fifo write test code kanglei2004@163.com 2019-08-10*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define FIFO_SERVER "fifoserver"

int main(int argc,char** argv)  //参数为即将写入的字节数
{
	int fd;
	char w_buf[64*1024];
	int real_wnum = 0;
	memset(w_buf,0,64*1024);
	if((mkfifo(FIFO_SERVER,O_CREAT|O_EXCL|S_IRWXU|S_IRGRP|S_IWGRP|S_IRUSR|S_IWUSR|S_IWOTH|S_IROTH)<0)&&(errno!=EEXIST))
		printf("cannot create fifoserver\n");
	else 
		printf("create fifoserver success\n");
	//if(fd==-1)
	//       if(errno==ENXIO)
	//              printf("open error; no reading process\n");
	
	
	sleep(3); // 等待读FIFO进行启动
	//fd=open(FIFO_SERVER,O_WRONLY|O_NONBLOCK,0); //设置非阻塞标志
	
	fd=open(FIFO_SERVER,O_WRONLY,0);  //设置阻塞标志
	//
	if(fd == -1)
		if(errno == ENXIO)
			printf("open error, no reading process\n");

	real_wnum=write(fd,w_buf,64*1024);
	
	if(real_wnum==-1)
	{
		if(errno==EAGAIN)
			printf("write to fifo error: try later\n");
		if(errno == EBADF)
			printf("write to fifo error: bad file descriptor\n");
		return 1;

	}
	else
		printf("real write num is %d\n",real_wnum);
	
	real_wnum=write(fd,w_buf,5000);
	//5000用于测试写入字节大于4096时的非原子性
	//real_wnum=write(fd,w_buf,4096);
	//4096用于测试写入字节不大于4096时的原子性
	if(real_wnum==-1)
		if(errno==EAGAIN)
			printf("try later\n");
	return 0;
}
