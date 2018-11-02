#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>


#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include "poolsocket.h"
#include "keymngserverop.h"
#include "keymnglog.h"
//#include "keymng_dbop.h"

int  g_EndTag ;
       
MngServer_Info				mngServerInfo;

//业务线程框架
 void *mystart_routine (void *arg)
 {
 	int 			ret = 0;
 	int 			timeout = 3;
 	int 			connfd = 0;
 
 	unsigned char 	*out = NULL;
 	int 			outlen = 0;
 	
 	//请求报文结构体
 	
 	MsgKey_Req			*pMsgKeyReq = NULL;
 	int 				iMsgKeyReqType = 0;
 	
 	//应答报文流
 	unsigned char 	*pMsgKeyResData = NULL;
 	int 			iMsgKeyResDataLen = 0;
 	
 	
 	connfd = (int)arg;
 	
 	while (1)
 	{
 		if (g_EndTag == 1)
 		{
 			break;
 		} 
 		out = NULL;
 		outlen = 0;
 		
 		pMsgKeyReq = NULL;
 		iMsgKeyReqType = 0;
 		
 		pMsgKeyResData = NULL;
 		iMsgKeyResDataLen = 0;
 		
	 	//1服务器端 接受 客户端的请求报文 
		ret = sckServer_rev(connfd, timeout, &out, &outlen); //1 //free
		if (ret == Sck_ErrPeerClosed)
		{
			printf("func sckServer_rev() Sck_ErrPeerClosed:%d \n", ret);
			break;
		}
		else if (ret == Sck_ErrTimeOut)
		{
			printf("func sckServer_rev() Sck_ErrTimeOut:%d \n", ret);
			continue;  //特别小心  服务器端 不能主动的把连接给关闭 break;
		}
		else if (ret != 0)
		{
			printf("func sckServer_rev() err:%d \n", ret);
			break;
		}
		
		//2 服务器端解析 客户端的请求报文 
		ret =  MsgDecode(out,  outlen, (void **)&pMsgKeyReq, &iMsgKeyReqType); //free
		if (ret != 0)
		{
			KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[4], ret,"func MsgDecode() err");
			goto End;
		}
	
		//根据请求报文 命令码
		switch (pMsgKeyReq->cmdType)
		{
			case KeyMng_NEWorUPDATE:
				ret = MngServer_Agree(&mngServerInfo, pMsgKeyReq, &pMsgKeyResData, &iMsgKeyResDataLen);  //free
				break;
			case KeyMng_Check:
				ret = MngServer_Check(&mngServerInfo, pMsgKeyReq, &pMsgKeyResData, &iMsgKeyResDataLen);  //free
				break;
			case KeyMng_Revoke:
				ret = MngServer_Revoke(&mngServerInfo, pMsgKeyReq, &pMsgKeyResData, &iMsgKeyResDataLen);  //free
				break;
			default :
				break;		
		}
	
		if (ret != 0)
		{
			//业务流错误处理	
			//MsgKey_Res 		msgKeyRes;
			goto End;
		}
	
		//服务器端发送应答报文
		ret = sckServer_send(connfd, timeout, pMsgKeyResData, iMsgKeyResDataLen);
		if (ret == Sck_ErrPeerClosed)
		{
			if (out != NULL) 		sck_FreeMem((void **) &out);
			if (pMsgKeyReq != NULL)  MsgMemFree((void **) &pMsgKeyReq, iMsgKeyReqType);		
			if (pMsgKeyResData != NULL)  MsgMemFree((void **) &pMsgKeyResData, 0);
			printf("func sckServer_send() Sck_ErrPeerClosed:%d \n", ret);
			break;
		}
		else if (ret == Sck_ErrTimeOut)
		{
			if (out != NULL) 		sck_FreeMem((void **) &out);
			if (pMsgKeyReq != NULL)  MsgMemFree((void **) &pMsgKeyReq, iMsgKeyReqType);		
			if (pMsgKeyResData != NULL)  MsgMemFree((void **) &pMsgKeyResData, 0);
			printf("func sckServer_send() Sck_ErrTimeOut:%d \n", ret);
			break;
		}
		else if (ret != 0)
		{
			if (out != NULL) 		sck_FreeMem((void **) &out);
			if (pMsgKeyReq != NULL)  MsgMemFree((void **) &pMsgKeyReq, iMsgKeyReqType);		
			if (pMsgKeyResData != NULL)  MsgMemFree((void **) &pMsgKeyResData, 0);
			printf("func sckServer_send() err:%d \n", ret);
			break;
		}	
		
		//有些错误 需要break 有些错误需要走End 再while循环
		End:	
			if (out != NULL) 		sck_FreeMem((void **) &out);
			if (pMsgKeyReq != NULL)  MsgMemFree((void **) &pMsgKeyReq, iMsgKeyReqType);		
			if (pMsgKeyResData != NULL)  MsgMemFree((void **) &pMsgKeyResData, 0);
 	}
 
	sckServer_close(connfd);
	
 	return NULL;
 }
 
 int mainxxxx()
 {
 	fork();
 	
 	fork();
 	
 	fork();
 	
 	fork();
 	
 	printf("aaaaaaaaaa\n");
 	return 0;	
 }
 
 #define INIT_DAEMON \
{ \
	if(fork() >0) exit(0); \
	setsid(); \
	if(fork()>0) exit(0); \
}

//怎么样理解回调函数
//角度1 : 谁调用这样的api函数(函数指针做函数参数) 谁提供函数入口地址
//角度2 : 任务的调用者  和  任务的编写者 进行解耦合
//角度3: 




void mysighandler_t(int arg)
{
	g_EndTag = 1;
	printf("keymngsever接收到退出信号 开始退出() arg:%d  \n", arg);
	
	return ;
}


       
int main()
{
	int 			port = 8001, ret = 0;
	int 			listenfd = 0;
	int 			timeout = 3;
	int 			connfd = 0;
	
	//注册信号 
	signal(SIGUSR1, mysighandler_t); //告诉linux内核 当用户向keymngserver发送自定义信号SIGUSE1  请你调用mysighandler_t
	
	//当有管道破裂信号产生时,请忽略 , 不要把我干掉
	signal(SIGPIPE, SIG_IGN);

	INIT_DAEMON
	
	
	g_EndTag = 0;
	//
	pthread_t		pid;  
	
	//连接数据库的配置信息 从配置文件中获取 (SECMNG/SECMNG@orc )
	//其他的所有的配置信息, 都从数据库中获取  最大支持的网点个数 serverid port key 
	ret = MngServer_InitInfo(&mngServerInfo);
	if (ret != 0)
	{
		printf("func MngServer_InitInfo() err:%d \n", ret);
		return ret;
	}
			
			
	//服务器端初始化
	ret =  sckServer_init(port, &listenfd);
	if (ret != 0)
	{
		printf("func sckServer_init() err:%d \n ", ret);
		return ret;
	}
	
	while (1)
	{
		if (g_EndTag == 1)
		{
			break;
		}
		
		ret = sckServer_accept(listenfd, timeout, &connfd);
		if (ret==Sck_ErrTimeOut)
		{
			printf("linux内核中没有已完成的连接\n");
			continue;
		}
		
		//开业务线程
		pthread_create(&pid, NULL, mystart_routine,  (void *)connfd);
		
	}

	//服务器端环境释放 
	printf("keymngserver优雅退出 释放资源开始\n");
	sleep(2);
	sckServer_destroy();
	
	printf("keymngserver优雅退出 释放资源完毕\n");

	return 0;
}