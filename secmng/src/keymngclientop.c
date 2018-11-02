

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>
#include <pthread.h>


#include "keymng_msg.h"
#include "myipc_shm.h"
#include "poolsocket.h"
#include "keymnglog.h"
#include "keymngclientop.h"
#include "keymng_shmop.h"  //网点密钥

int MngClient_InitInfo(MngClient_Info *pCltInfo)
{
	int 			ret = 0;
	KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[1], 0,"func MngClient_InitInfo() begin");
	
	strcpy(pCltInfo->clientId, "1111");
	strcpy(pCltInfo->AuthCode, "1111");
	strcpy(pCltInfo->serverId, "0001");
	strcpy(pCltInfo->serverip, "127.0.0.1");
	pCltInfo->serverport = 8001;
	pCltInfo->maxnode = 1;
	pCltInfo->shmkey = 0x1111;
	pCltInfo->shmhdl = 0; //
	
	//创建共享内存
	ret = KeyMng_ShmInit(pCltInfo->shmkey, pCltInfo->maxnode, &pCltInfo->shmhdl);
	if (ret != 0)
	{
		printf("func KeyMng_ShmInit() err:%d \n", ret);
	}
	else
	{
		printf("func KeyMng_ShmInit() ok:%d \n", ret);
	}
	
	KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[1], 0,"func MngClient_InitInfo() end");
	return ret;	
}


/* 
客户端密钥协商
1  组织请求报文
2 编码请求报文
3 发送请求报文
4 接受应答报文r2
5 协商密钥
6 写共享内存
7 日志和审计信息
*/

int MngClient_Agree(MngClient_Info *pCltInfo)
{
	int 				i = 0, ret =0;
	MsgKey_Req			msgKeyReq;
	int 				mytime = 3;
	
	unsigned char		*outData = NULL;
	int					outLen = 0; 
	
	int 				connfd = 0;
	
	//接受的应答报文 二进制字符流
	unsigned char		*pMsgKeyResData = NULL;
	int 				iMsgKeyResDataLen = 0;
	
	//应答报文结构体
	MsgKey_Res			*pMsgKeyRes = NULL;
	int 				iMsgKeyResType = 0;
	
	
	memset(&msgKeyReq, 0, sizeof(MsgKey_Req));
	
	KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[1], 0,"func MngClient_Agree() begin");
	
	//1  组织请求报文
	msgKeyReq.cmdType  =  KeyMng_NEWorUPDATE;
	strcpy(msgKeyReq.clientId,  pCltInfo->clientId);
	strcpy(msgKeyReq.AuthCode,  pCltInfo->AuthCode);
	strcpy(msgKeyReq.serverId,  pCltInfo->serverId);
	for (i=0; i<64; i++)
	{
		msgKeyReq.r1[i] = 'a' + i;
	}

	//2编码请求报文
	ret = MsgEncode(&msgKeyReq, ID_MsgKey_Req, &outData, &outLen); //free ok
	if (ret != 0)
	{
		KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[4], ret,"func MsgEncode() err");
		return ret;
	}
	
	//3 发送请求报文
	//客户端 初始化
	ret = sckClient_init();
	if (ret != 0)
	{
		KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[4], ret,"func sckClient_init() err");
		goto End;
	}

	//客户端 连接服务器 char	
	ret = sckClient_connect(pCltInfo->serverip, pCltInfo->serverport, mytime, &connfd);
	if (ret != 0)
	{
		KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[4], ret,"func sckClient_connect() err serverip:%s serverport:%d", pCltInfo->serverip,  pCltInfo->serverport);
		goto End;
	}
	
	//客户端 发送报文
	ret = sckClient_send(connfd, mytime, outData, outLen);
	if (ret != 0)
	{
		KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[4], ret,"func sckClient_send() err");
		goto End;
	}
	
	//客户端 接受报文
	ret = sckClient_rev(connfd, mytime, &pMsgKeyResData, &iMsgKeyResDataLen); //1 free  ok
	if (ret != 0)
	{
		KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[4], ret,"func sckClient_rev() err");
		goto End;
	}
	
	//解析服务器端的应答报文
	ret = MsgDecode(pMsgKeyResData,  iMsgKeyResDataLen, (void **)&pMsgKeyRes, &iMsgKeyResType);  //free ok
	if (ret != 0)
	{
		KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[4], ret,"func MsgDecode() err");
		goto End;
	}
	
	if (iMsgKeyResType != ID_MsgKey_Res)
	{
		ret = MngClt_ParamErr;
		//关键步骤:一定要把出错的场景给打印日志  非常重要
		KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[4], ret, "keymngclient校验服务器的应答报文错误:iMsgKeyResType:%d", iMsgKeyResType);
		goto End;
	}
	printf("seckeyid:%d \n", pMsgKeyRes->seckeyid);
	
	NodeSHMInfo 	nodeInfo;
	memset(&nodeInfo, 0, sizeof(NodeSHMInfo));
	
	//组织网点密钥 //协商密钥
	//r1 abc  r2 123 
	//a1b2c3.....
	for (i=0; i<64; i++)
	{
		nodeInfo.seckey[2*i] = msgKeyReq.r1[i];
		nodeInfo.seckey[2*i+1] = pMsgKeyRes->r2[i];
	}
	nodeInfo.status = 0; 			
	strcpy(nodeInfo.clientId, msgKeyReq.clientId);
	strcpy(nodeInfo.serverId, msgKeyReq.serverId);
	nodeInfo.seckeyid = pMsgKeyRes->seckeyid;		
	
	//写共享内存
	ret = KeyMng_ShmWrite(pCltInfo->shmhdl, pCltInfo->maxnode, &nodeInfo);
	if (ret != 0)
	{
		KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[4], ret,"func KeyMng_ShmWrite() err");
		goto End;
	}

End:	
	if (outData != NULL)  MsgMemFree((void **) &outData, 0);
	if (pMsgKeyResData != NULL)	 sck_FreeMem((void **)&pMsgKeyResData);	
	if (pMsgKeyRes != NULL)  MsgMemFree((void **) &pMsgKeyRes, iMsgKeyResType);
	
	//客户端 关闭和服务端的连接
	sckClient_closeconn(connfd);
	
	//客户端 释放
	sckClient_destroy();

	KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[1], 0,"func MngClient_Agree() end");
	return ret;	
}

//密钥校验
int MngClient_Check(MngClient_Info *pCltInfo)
{
	int 				i = 0, ret =0;
	MsgKey_Req			msgKeyReq;
	int 				mytime = 3;
	
	unsigned char		*outData = NULL;
	int					outLen = 0; 
	
	int 				connfd = 0;
	
	//接受的应答报文 二进制字符流
	unsigned char		*pMsgKeyResData = NULL;
	int 				iMsgKeyResDataLen = 0;
	
	//应答报文结构体
	MsgKey_Res			*pMsgKeyRes = NULL;
	int 				iMsgKeyResType = 0;
	
	memset(&msgKeyReq, 0, sizeof(MsgKey_Req));
	
	KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[1], 0,"func MngClient_Agree() begin");
	
	//1  组织请求报文
	msgKeyReq.cmdType  =  KeyMng_Check;  //请求类型
	strcpy(msgKeyReq.clientId,  pCltInfo->clientId);
	strcpy(msgKeyReq.AuthCode,  pCltInfo->AuthCode);
	strcpy(msgKeyReq.serverId,  pCltInfo->serverId);

	NodeSHMInfo   	nodeInfo;
	memset(&nodeInfo, 0, sizeof(NodeSHMInfo));
	//读网点密钥
	ret = KeyMng_ShmRead(pCltInfo->shmhdl, pCltInfo->clientId, pCltInfo->serverId,  pCltInfo->maxnode, &nodeInfo);
	if (ret != 0)
	{
		KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[4], ret,"func KeyMng_ShmRead() err");
		return ret;
	}
	memcpy(msgKeyReq.r1, nodeInfo.seckey, 8);
	

	//2编码请求报文
	ret = MsgEncode(&msgKeyReq, ID_MsgKey_Req, &outData, &outLen); //free ok
	if (ret != 0)
	{
		KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[4], ret,"func MsgEncode() err");
		return ret;
	}
	
	//3 发送请求报文
	//客户端 初始化
	ret = sckClient_init();
	if (ret != 0)
	{
		KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[4], ret,"func sckClient_init() err");
		goto End;
	}

	//客户端 连接服务器 char	
	ret = sckClient_connect(pCltInfo->serverip, pCltInfo->serverport, mytime, &connfd);
	if (ret != 0)
	{
		KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[4], ret,"func sckClient_connect() err serverip:%s serverport:%d", pCltInfo->serverip,  pCltInfo->serverport);
		goto End;
	}
	
	//客户端 发送报文
	ret = sckClient_send(connfd, mytime, outData, outLen);
	if (ret != 0)
	{
		KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[4], ret,"func sckClient_send() err");
		goto End;
	}
	
	//客户端 接受报文
	ret = sckClient_rev(connfd, mytime, &pMsgKeyResData, &iMsgKeyResDataLen); //1 free  ok
	if (ret != 0)
	{
		KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[4], ret,"func sckClient_rev() err");
		goto End;
	}
	
	//解析服务器端的应答报文
	ret = MsgDecode(pMsgKeyResData,  iMsgKeyResDataLen, (void **)&pMsgKeyRes, &iMsgKeyResType);  //free ok
	if (ret != 0)
	{
		KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[4], ret,"func MsgDecode() err");
		goto End;
	}
	
	if (iMsgKeyResType != ID_MsgKey_Res)
	{
		ret = MngClt_ParamErr;
		//关键步骤:一定要把出错的场景给打印日志  非常重要
		KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[4], ret, "keymngclient校验服务器的应答报文错误:iMsgKeyResType:%d", iMsgKeyResType);
		goto End;
	}
	printf("seckeyid:%d \n", pMsgKeyRes->seckeyid);
	
	if (pMsgKeyRes->rv != 0)
	{
		ret = pMsgKeyRes->rv;
		//关键步骤:一定要把出错的场景给打印日志  非常重要
		KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[4], ret, "服务器端校验密钥失败rv:%d", pMsgKeyRes->rv);
		goto End;
	}
	

End:	
	if (outData != NULL)  MsgMemFree((void **) &outData, 0);
	if (pMsgKeyResData != NULL)	 sck_FreeMem((void **)&pMsgKeyResData);	
	if (pMsgKeyRes != NULL)  MsgMemFree((void **) &pMsgKeyRes, iMsgKeyResType);
	
	//客户端 关闭和服务端的连接
	sckClient_closeconn(connfd);
	
	//客户端 释放
	sckClient_destroy();

	KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[1], 0,"func MngClient_Agree() end");
	return ret;	
}

int MngClient_Revoke(MngClient_Info *pCltInfo)
{
	return 0;	
}