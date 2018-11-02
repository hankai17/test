#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>
#include <pthread.h>

#include "keymnglog.h"
#include "keymngserverop.h"
#include "poolsocket.h"
#include "keymng_msg.h"
#include "myipc_shm.h"
#include "keymng_shmop.h"
#include "icdbapi.h"
#include "keymng_dbop.h"


int MngServer_InitInfo(MngServer_Info *svrInfo)
{
	int 			ret = 0;
	KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[1], 0,"func MngServer_InitInfo() begin");
	
	//从数据库中获取配置信息
	strcpy(svrInfo->serverId, "0001");
	
	//从配置文件中 获取配置信息
	strcpy(svrInfo->dbuse, "SECMNG");
	strcpy(svrInfo->dbpasswd, "SECMNG");
	strcpy(svrInfo->dbsid, "orcl");
	svrInfo->dbpoolnum = 1;
	
	strcpy(svrInfo->serverip, "127.0.0.1");
	svrInfo->serverport = 8001;
	
	//从数据库中获取配置信息
	svrInfo->maxnode = 20;
	svrInfo->shmkey = 0x0001;
	svrInfo->shmhdl = 0;
	
	//共享内存初始化
	ret = KeyMng_ShmInit(svrInfo->shmkey, svrInfo->maxnode, &svrInfo->shmhdl);
	if (ret != 0)
	{
		printf("func KeyMng_ShmInit() err:%d \n", ret);
		return ret;
	}
	
	//初始化数据库连接池
	ret = IC_DBApi_PoolInit(svrInfo->dbpoolnum, svrInfo->dbsid,svrInfo->dbuse, svrInfo->dbpasswd);
	if (ret != 0)
	{
		printf("func IC_DBApi_PoolInit() err:%d \n", ret);
		return ret;
	}
	else
	{
		printf("初始化数据库连接池 ok\n");
	}
	
	KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[1], 0,"func MngServer_InitInfo() end");
	return ret;
}


//1  组织应答报文 
//		获取keysn
//2  r1 r2 协商密钥
//3  写共享内存
//4  	写网点密钥到 数据库  
//5 编码应答报文

int MngServer_Agree(MngServer_Info *svrInfo, MsgKey_Req *msgkeyReq, unsigned char **outData, int *datalen)
{
	int				ret = 0, i= 0;
	MsgKey_Res 		msgKeyRes;
	
	ICDBHandle 		handle = NULL;
	int 			tmpkeyid = 0;

	KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[1], 0, "func MngServer_Agree() begin");
	memset(&msgKeyRes, 0, sizeof(MsgKey_Res));
	
	//获取数据库连接
	ret = IC_DBApi_ConnGet(&handle, 0, 0);
	if (ret != 0) 
	{
		KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[4], ret,"func IC_DBApi_ConnGet() err");
		return ret;
	}
	
	//开事物
	ret = IC_DBApi_BeginTran(handle);
	if (ret != 0) 
	{
		IC_DBApi_ConnFree(handle, 1); //
		KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[4], ret,"func IC_DBApi_BeginTran() err");
		return ret;
	}
	
	//获取keysn
	ret = KeyMngsvr_DBOp_GenKeyID(handle, &tmpkeyid);
	if (ret != 0) 
	{
		KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[4], ret,"func IC_DBApi_ConnGet() err");
		goto End;
	}
		
	//组织应答报文
	msgKeyRes.rv  = 0;
	strcpy(msgKeyRes.clientId, msgkeyReq->clientId);
	strcpy(msgKeyRes.serverId, svrInfo->serverId);	
	for (i=0; i<64; i++)
	{
		msgKeyRes.r2[i] = 'a' + i;
	}
	msgKeyRes.seckeyid =  tmpkeyid;
	
	
	//编码应答报文
	ret = MsgEncode(&msgKeyRes, ID_MsgKey_Res, outData, datalen);
	//ret = 200;
	if (ret != 0) 
	{
		KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[4], ret,"func MsgEncode() err");
		goto End;
	}
		
	NodeSHMInfo 			nodeShmInfo;
	//组织网点密钥
	memset(&nodeShmInfo, 0, sizeof(NodeSHMInfo));
	nodeShmInfo.status = 0;
	strcpy(nodeShmInfo.clientId, msgkeyReq->clientId);
	strcpy(nodeShmInfo.serverId, msgkeyReq->serverId);
	nodeShmInfo.seckeyid = msgKeyRes.seckeyid; //对称密钥序列号 从数据库中获取
	
	for (i=0; i<64; i++)
	{
		nodeShmInfo.seckey[2*i] = msgkeyReq->r1[i];
		nodeShmInfo.seckey[2*i+1] = msgKeyRes.r2[i];
	}
	
	//写网点密钥
	ret = KeyMng_ShmWrite(svrInfo->shmhdl, svrInfo->maxnode, &nodeShmInfo);
	if (ret != 0)
	{
		KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[4], ret,"func KeyMng_ShmWrite() err");
		goto End;
	}
	
	
	//写数据库
	ret = KeyMngsvr_DBOp_WriteSecKey(handle, &nodeShmInfo) ;
	if (ret != 0)
	{
		KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[4], ret,"func KeyMngsvr_DBOp_WriteSecKey() err");
		goto End;
	}

End:
	
	if (ret == 0)
	{
		IC_DBApi_Commit(handle);
	}
	else
	{
		IC_DBApi_Rollback(handle);
	}

	if (ret == IC_DB_CONNECT_ERR)  //把链接放到连接池中
	{
		IC_DBApi_ConnFree(handle, 0); //需要锻炼修复
	}
	else
	{
		IC_DBApi_ConnFree(handle, 1); //不需要锻炼修复
	}
	
	KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[1], 0,"func MngServer_Agree() end");
	return ret ;
}


int MngServer_Check(MngServer_Info *svrInfo, MsgKey_Req *msgkeyReq, unsigned char **outData, int *datalen)
{
	int				ret = 0, i= 0;
	MsgKey_Res 		msgKeyRes;
	
	KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[1], 0, "func MngServer_Agree() begin");
	memset(&msgKeyRes, 0, sizeof(MsgKey_Res));
	
	
	NodeSHMInfo 	nodeShmInfo;
	memset(&nodeShmInfo, 0, sizeof(NodeSHMInfo));
	
	//读网点密钥
	ret = KeyMng_ShmRead(svrInfo->shmhdl, msgkeyReq->clientId, svrInfo->serverId,  svrInfo->maxnode, &nodeShmInfo);
	if (ret != 0)
	{
		KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[4], ret,"func KeyMng_ShmRead() err");
		return ret;
	}

	//组织应答报文
	msgKeyRes.rv  = 0;
	strcpy(msgKeyRes.clientId, msgkeyReq->clientId);
	strcpy(msgKeyRes.serverId, svrInfo->serverId);	
	memcpy( msgKeyRes.r2, nodeShmInfo.seckey, 8); 
	msgKeyRes.seckeyid =  nodeShmInfo.seckeyid;
	
	if ( memcmp(nodeShmInfo.seckey, msgkeyReq->r1, 8 ) == 0)
	{
		msgKeyRes.rv  = 0;
	}
	else
	{
		KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[4], ret,"服务器端校验网点密钥失败 msgKeyRes.clientId:%s  msgKeyRes.serverId:%s ",  msgKeyRes.clientId, msgKeyRes.serverId);
		msgKeyRes.rv  = 112; //
	}
	
	//编码应答报文
	ret = MsgEncode(&msgKeyRes, ID_MsgKey_Res, outData, datalen);
	if (ret != 0)
	{
		KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[4], ret,"func MsgEncode() err");
		return ret;
	}
	
	KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[1], ret,"func MngServer_Agree() end");
	return ret ;	
}

int MngServer_Revoke(MngServer_Info *svrInfo, MsgKey_Req *msgkeyReq, unsigned char **outData, int *datalen)
{
	return 0;	
}
