#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include"CSckImp1.h"
#include"CSckImp2.h"
#include"CSocketProtocol.h"
#include"CEncDesProtocol.h"
#include"CHwImp.h"
#include"Framework.h"

//测试通信框架
void TestSocketProtocol01(){
	
	CSocketProtocol csck;
	//使用第一个厂商的通信接口
	//Init_CSocketProtocol(&csck, Init_SocketClinet1, Send_SocketClient1, 
	//	Recv_SocketClient1, Destroy_SocketClient1);

	//使用第二个厂商的通信接口
	Init_CSocketProtocol(&csck, Init_SocketClinet2, Send_SocketClient2,
		Recv_SocketClient2, Destroy_SocketClient2);

	unsigned char* sendBuf = "hello wrold!";
	int sendBufLen = strlen(sendBuf) + 1;

	unsigned char recvBuf[1024] = { 0 };
	int recvBufLen = 0;

	int ret = framework(&csck, sendBuf, sendBufLen, recvBuf, &recvBufLen);
	if (ret != 0){
		printf("framework error:%d\n", ret);
	}

	printf("接收到的数据:%s\n",recvBuf);
	printf("接收到的数据长度:%d\n",recvBufLen);
}

//增加加密功能通信框架
void TestSocketProtocol02(){

	CSocketProtocol csck;
	//使用第一个厂商的通信接口
	//Init_CSocketProtocol(&csck, Init_SocketClinet1, Send_SocketClient1, 
	//	Recv_SocketClient1, Destroy_SocketClient1);

	//使用第二个厂商的通信接口
	Init_CSocketProtocol(&csck, Init_SocketClinet2, Send_SocketClient2,
		Recv_SocketClient2, Destroy_SocketClient2);

	//使用华为加密接口
	CEncDesProtocol ced;
	Init_CEncDesProtocol(&ced, EncDataHw, DecDataHw);

	unsigned char* sendBuf = "hello wrold!";
	int sendBufLen = strlen(sendBuf) + 1;

	unsigned char recvBuf[1024] = { 0 };
	int recvBufLen = 0;

	unsigned char tempBuf[1024] = { 0 };

	int ret = frameworkPro(&csck, &ced, sendBuf, sendBufLen, recvBuf, &recvBufLen);
	if (ret != 0){
		printf("framework error:%d\n", ret);
	}

	printf("接收到的数据:%s\n", recvBuf);
	printf("接收到的数据长度:%d\n", recvBufLen);
}

int main(){

	//TestSocketProtocol01();
	TestSocketProtocol02();

	system("pause");
	return EXIT_SUCCESS;
}