/*system V msgqueue test code   kanglei2004@163.com 2019-08-13*/
#include <sys/types.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

#define MSG_LEN 24
#define MSG_TYPE 10
void msg_stat(int,struct msqid_ds );

int main(int argc, char** argv)
{
	int gflags,sflags,rflags;
	key_t key;
	int msgid;
	int reval;
	struct msgsbuf
	{
		long mtype; // 这个类型一定是与msgrcv 中msgtype的类型一致，否则会报No message of desired type 错误
		char mtext[MSG_LEN];
	}msg_sbuf,msg_rbuf;
	
	struct msqid_ds msg_ginfo,msg_sinfo;
	char* msgpath="/root/queue";
	// 需要注意的是这个文件必须存在，并且权限正确
	key=ftok(msgpath,'a');
	if(key == -1)
	{
		printf("ftok error: %s\n",strerror(errno));
		return 1;
	}
	gflags=IPC_CREAT|IPC_EXCL;
	msgid=msgget(key,gflags|00666);
	if(msgid==-1)
	{
		printf("msgget error: %s\n",strerror(errno));
		printf("msg create error\n");
		return 1;
	}
	//创建一个消息队列后，输出消息队列缺省属性
	msg_stat(msgid,msg_ginfo);
	sflags=IPC_NOWAIT;
	msg_sbuf.mtype=MSG_TYPE;
	char msg[]="Hello World!";
	strncpy(msg_sbuf.mtext,msg,sizeof(msg));
	reval=msgsnd(msgid,&msg_sbuf,sizeof(msg_sbuf),sflags);
	if(reval==-1)
	{
		printf("message send error\n");
		return 1;
	}
	//发送一个消息后，输出消息队列属性
	msg_stat(msgid,msg_ginfo);
	rflags=IPC_NOWAIT|MSG_NOERROR;
	reval=msgrcv(msgid,&msg_rbuf,sizeof(msg_rbuf),MSG_TYPE,rflags);
	if(reval==-1)
	{
		printf("read msg error: %s\n",strerror(errno));
		printf("read msg error\n");
	}
	else
	{
		printf("read from msg queue %d bytes\n",reval);
		printf("msg_rbuf.mtype is %ld \n",msg_rbuf.mtype);
		printf("msg_rbuf.mtext is %s \n",msg_rbuf.mtext);
	}
	//从消息队列中读出消息后，输出消息队列属性
	msg_stat(msgid,msg_ginfo);
	msg_sinfo.msg_perm.uid=8;//just a try
	msg_sinfo.msg_perm.gid=8;//
	msg_sinfo.msg_qbytes=16388;
	//此处验证超级用户可以更改消息队列的缺省msg_qbytes
	//注意这里设置的值大于缺省值
	reval=msgctl(msgid,IPC_SET,&msg_sinfo);
	if(reval==-1)
	{
		printf("msg set info error\n");
		return 1;
	}
	msg_stat(msgid,msg_ginfo);
	//验证设置消息队列属性
	reval=msgctl(msgid,IPC_RMID,NULL);
	//删除消息队列
	if(reval==-1)
	{
		printf("unlink msg queue error\n");
		return 1;
	}
	return 0;
}

void msg_stat(int msgid,struct msqid_ds msg_info)
{
	int reval;
	sleep(1);//只是为了后面输出时间的方便
	reval=msgctl(msgid,IPC_STAT,&msg_info);
	if(reval==-1)
	{
		printf("get msg info error\n");
		return;
	}
	printf("current number of bytes on queue is %ld\n",msg_info.msg_cbytes);
	printf("number of messages in queue is %ld\n",msg_info.msg_qnum);
	printf("max number of bytes on queue is %ld\n",msg_info.msg_qbytes);
	//每个消息队列的容量（字节数）都有限制MSGMNB，值的大小因系统而异。在创建新的消息队列时，//msg_qbytes的缺省值就是MSGMNB
	printf("pid of last msgsnd is %d\n",msg_info.msg_lspid);
	printf("pid of last msgrcv is %d\n",msg_info.msg_lrpid);
	printf("last msgsnd time is %s", ctime(&(msg_info.msg_stime)));
	printf("last msgrcv time is %s", ctime(&(msg_info.msg_rtime)));
	printf("last change time is %s", ctime(&(msg_info.msg_ctime)));
	printf("msg uid is %d\n",msg_info.msg_perm.uid);
	printf("msg gid is %d\n",msg_info.msg_perm.gid);
	printf("\n");
}
