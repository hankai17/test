

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

#include <pthread.h>

#include "poolsocket.h"
#include "keymnglog.h"
#include "keymngclientop.h"
#include "keymng_msg.h"

int Usage()
{
    int nSel = -1;
    
    system("clear");    
    printf("\n  /*************************************************************/");
    printf("\n  /*************************************************************/");
    printf("\n  /*     1.密钥协商                                            */");
    printf("\n  /*     2.密钥校验                                            */");
    printf("\n  /*     3.密钥注销                                            */");
    printf("\n  /*     4.密钥查看                                            */");
    printf("\n  /*     0.退出系统                                            */");
    printf("\n  /*************************************************************/");
    printf("\n  /*************************************************************/");
    printf("\n\n  选择:");

    scanf("%d", &nSel);  //
    while(getchar() != '\n');
    
    return nSel;
}

int mainxxxxxx()
{
	//void KeyMng_Log(const char *file, int line, int level, int status, const char *fmt, ...);
	KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[0], 0,"func main() 0000000000000 ");
	KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[1], 1,"func main() 11111111111111");
	KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[2], 2,"func main() 222222222222222222");
	KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[3], 3,"func main() 333333333333333333");
	KeyMng_Log(__FILE__, __LINE__,KeyMngLevel[4], 4,"func main()44444444444");	
	printf("hello keymngclient...\n");
	return 0;
}


int main()
{
	int 					ret = 0, nSel = 0;
	MngClient_Info  		mngClientInfo; 
	memset(&mngClientInfo, 0, sizeof(MngClient_Info));
	
	ret = MngClient_InitInfo(&mngClientInfo);
	if (ret != 0)
	{
		printf("func MngClient_InitInfo() err:%d \n", ret);
		return ret;
	}
	
	while (1)
	{
		nSel = Usage();
		switch (nSel)
		{
		case 0:
			return 0;	
		case KeyMng_NEWorUPDATE:
			//密钥协商流程
			ret = MngClient_Agree(&mngClientInfo);
			break;
		case KeyMng_Check:
			//密钥校验流程
			ret = MngClient_Check(&mngClientInfo);
			break;	
		case KeyMng_Revoke:
			//密钥注销
			ret = MngClient_Revoke(&mngClientInfo);
			break;
		default:
			printf("输入的数值不合法");
			break;
		}
		
		if (ret)
		{
			printf("\n!!!!!!!!!!!!!!!!!!!!ERROR!!!!!!!!!!!!!!!!!!!!");
			printf("\n错误码是：%x\n", ret);
		}
		else
		{
			printf("\n!!!!!!!!!!!!!!!!!!!!SUCCESS!!!!!!!!!!!!!!!!!!!!\n");
		}	
		getchar(); 
	}
	printf("hello keymngclient...\n");
	
	return 0;
}








