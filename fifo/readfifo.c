/*fifo read test code kanglei2004@163.com 2019-08-10*/
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
	char r_buf[4096*2];
	int real_rnum = 0;
	int r_size = 0;
	memset(r_buf,0,4096*2);

	if(argc < 2)
	{
		printf("must input read size\n");
		return 1;
	}
	else
		r_size = atoi(argv[1]);

	printf("request read size %d\n",r_size);
	sleep(3); // 等待写FIFO进行启动
	//fd=open(FIFO_SERVER,O_RDONLY|O_NONBLOCK,0); //设置非阻塞标志
	
	fd=open(FIFO_SERVER,O_RDONLY,0);  //设置阻塞标志
	//
	if(fd == -1)
	{
		printf("open %s error\n",FIFO_SERVER);
		printf("error %s \n",strerror(errno));
		return 1;
	}

	while(1)
	{
		memset(r_buf,0,sizeof(r_buf));
		real_rnum = read(fd, r_buf, r_size);
		if(real_rnum == -1)
			if(errno == EAGAIN)
				printf("no data available\n");
		printf("read %d from %s \n",real_rnum, FIFO_SERVER);
		sleep(1);
	}
	
	return 0;
}
