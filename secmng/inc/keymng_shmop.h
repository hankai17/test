

// keymng_shmop.h

#ifndef _KEYMNG_SHMOP_H_
#define _KEYMNG_SHMOP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifdef __cplusplus 
extern "C" {
#endif


//将网点密钥信息写共享内存， 网点共享内存结构体
typedef struct _NodeSHMInfo
{	
	int 			status;			//密钥状态 0-有效 1无效
	char			clientId[12];	//客户端id
	char			serverId[12];	//服务器端id	
	int				seckeyid;		//对称密钥id
	unsigned char	seckey[128];	//对称密钥 //hash1 hash256 md5
}NodeSHMInfo;


//int KeyMng_ShmInit(int keyid, int keysize, void *shmid )
//打开共享内存 共享内存存在则使用 不存在则创建
int KeyMng_ShmInit(int key, int maxnodenum, int *shmhdl);


	


int KeyMng_ShmWrite(int shmhdl, int maxnodenum, NodeSHMInfo *pNodeInfo);

int KeyMng_ShmRead(int shmhdl, char *clientId, char *serverId,  int maxnodenum, NodeSHMInfo *pNodeInfo);


			


#ifdef __cplusplus
}
#endif
#endif





