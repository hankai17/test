

#include <unistd.h>
#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>


#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "keymnglog.h"
#include "keymng_shmop.h"
#include "myipc_shm.h"

#include "keymngclientop.h"
//#include "poolsocket.h"


//看共享内存是否存在
//若 存在使用旧 
//若 不存在创建
int KeyMng_ShmInit(int key, int maxnodenum, int *shmhdl)
{
	int				ret = 0;
	
	//打开共享内存
	ret = IPC_OpenShm(key, maxnodenum*sizeof(NodeSHMInfo), shmhdl);
	if (ret == MYIPC_NotEXISTErr)
	{
		printf("keymng监测到共享内存不存在 正在创建共享内存...\n");
		ret = IPC_CreatShm(key, maxnodenum*sizeof(NodeSHMInfo), shmhdl);
		if (ret != 0)
		{
			printf("keymng创建共享内存 err:%d \n", ret);
			return ret;
		}
		else
		{
			void 	*mapaddr = NULL;
			printf("keymng创建共享内存 ok...\n");
			
			ret = IPC_MapShm(*shmhdl, (void **) &mapaddr);
			if (ret != 0)
			{
				printf("fun IPC_MapShm() err:%d 清空共享内存失败\n", ret);
				return ret;
			}
			memset(mapaddr, 0, maxnodenum*sizeof(NodeSHMInfo));
			IPC_UnMapShm(mapaddr);
			printf("keymng清空共享内存ok\n");
		}
	}
	else if (ret == 0)
	{
		printf("keymng监测到共享内存存在 使用旧的共享内存...\n");
	}
	else 
	{
		printf("fun IPC_OpenShm() err:%d\n", ret);
	}
	return ret;	
}


//写网点密钥 
//若存在 则修改 
//若不存在 则找一个空的位置写入
int KeyMng_ShmWrite(int shmhdl, int maxnodenum, NodeSHMInfo *pNodeInfo)
{
	int				ret = 0, i = 0;
	NodeSHMInfo  	tmpNodeInfo; //空结点
	NodeSHMInfo		*pNode = NULL;
	
	void 			*mapaddr = NULL;
	
	memset(&tmpNodeInfo, 0, sizeof(NodeSHMInfo));
	//连接共享内存
	ret = IPC_MapShm(shmhdl, (void **) &mapaddr);
	if (ret != 0)
	{
		KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[4], ret,"func IPC_MapShm() err");
		goto End;
	}
	
	//判断传入的网点密钥 是否已经 存在
	for (i=0; i<maxnodenum; i++)
	{
		pNode = mapaddr + sizeof(NodeSHMInfo)*i;
		if ( strcmp(pNode->clientId, pNodeInfo->clientId) == 0 &&
			strcmp(pNode->serverId, pNodeInfo->serverId) == 0 )
		{
			KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[3], ret,"系统检测到 共享内存中已经存在网点信息cliented:%s serverid%s", pNode->clientId, pNode->serverId);
			memcpy(pNode, pNodeInfo, sizeof(NodeSHMInfo));
			goto End;
		} 
	}
	
	//若不存在
	for (i=0; i<maxnodenum; i++)
	{
		pNode = mapaddr + sizeof(NodeSHMInfo)*i;
		if (memcmp(&tmpNodeInfo, pNode, sizeof(NodeSHMInfo)) == 0 )
		{
			KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[3], ret,"系统检测到 有一个空的位置 ");
			memcpy(pNode, pNodeInfo, sizeof(NodeSHMInfo));
			goto End;
		}
	}
	
	if (i == maxnodenum)
	{
		ret = 1111;
		KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[4], ret,"系统检测到共享内存已满 ");
		goto End;
	}
	
End:
	IPC_UnMapShm(mapaddr);
	return ret;
}

//根据clientid和serverid 去读网点信息
int KeyMng_ShmRead(int shmhdl, char *clientId, char *serverId,  int maxnodenum, NodeSHMInfo *pNodeInfo)
{
		int				ret = 0, i = 0;
	NodeSHMInfo  	tmpNodeInfo; //空结点
	NodeSHMInfo		*pNode = NULL;
	
	void 			*mapaddr = NULL;
	
	memset(&tmpNodeInfo, 0, sizeof(NodeSHMInfo));
	//连接共享内存
	ret = IPC_MapShm(shmhdl, (void **) &mapaddr);
	if (ret != 0)
	{
		KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[4], ret,"func IPC_MapShm() err");
		goto End;
	}
	
	//遍历网点信息
	for (i=0; i<maxnodenum; i++)
	{
		pNode = mapaddr + sizeof(NodeSHMInfo)*i;
		
		if ( strcmp(pNode->clientId, clientId) == 0 &&
			strcmp(pNode->serverId, serverId) == 0 )
		{
			KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[3], ret,"系统检测到 有一个空的位置 ");
			memcpy(pNodeInfo, pNode, sizeof(NodeSHMInfo));
			goto End;
		}
	}
	
	if (i == maxnodenum)
	{
		ret = 1111;
		KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[4], ret,"系统检测到共享内存已满 ");
		goto End;
	}
	
End:
	IPC_UnMapShm(mapaddr);
	return ret;
}
