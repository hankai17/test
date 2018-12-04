#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include"CSocketProtocol.h"
#include"CSckImp1.h"
#include"CSckImp2.h"
#include"CEncDesProtocol.h"
#include"CHwImp.h"
#include"Framework.h"


void TestFramework01(){

	CSocketProtocol csp;

	//调用厂商一的代码:
	//Init_CSocketProtocol(&csp,Init_Socket1,Send_Socket1,Recv_Socket1,Destroy_Socket1);
	//调用厂商二的代码:
	Init_CSocketProtocol(&csp, Init_Socket2, Send_Socket2, Recv_Socket2, Destroy_Socket2);

	//此时此刻  csp里所有的函数指针，都指向具体函数

	//发送数据
	unsigned char* sendBuf = "hello world";
	int sendBufLen = strlen(sendBuf) + 1;

	//接收数据
	unsigned char recvBuf[1024] = { 0 };
	int recvBufLen = 0;

	FrameWork(&csp, sendBuf, sendBufLen, recvBuf, &recvBufLen);

	printf("接收到的数据长度:%d\n", recvBufLen);
	printf("接收到的数据:%s\n", recvBuf);
}

void TestFramework02(){

	CSocketProtocol csp;

	//调用厂商一的代码:
	//Init_CSocketProtocol(&csp,Init_Socket1,Send_Socket1,Recv_Socket1,Destroy_Socket1);
	//调用厂商二的代码:
	Init_CSocketProtocol(&csp, Init_Socket2, Send_Socket2, Recv_Socket2, Destroy_Socket2);

	//加密接口
	EncDec ed;
	Init_EncDec(&ed, EncDataHw, DecDataHw);

	//此时此刻  csp里所有的函数指针，都指向具体函数

	//发送数据
	unsigned char* sendBuf = "hello world";
	int sendBufLen = strlen(sendBuf) + 1;

	//接收数据
	unsigned char recvBuf[1024] = { 0 };
	int recvBufLen = 0;

	FrameWorkPro(&csp, &ed, sendBuf, sendBufLen, recvBuf, &recvBufLen);

	printf("接收到的数据长度:%d\n", recvBufLen);
	printf("接收到的数据:%s\n", recvBuf);
}

int main(){

	//TestFramework01();
	TestFramework02();

	system("pause");
	return EXIT_SUCCESS;
}