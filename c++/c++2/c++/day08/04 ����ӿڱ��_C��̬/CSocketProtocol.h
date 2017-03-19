#pragma once

#include<stdlib.h>

//初始化函数
typedef int(*Init_Socket)(void** handle);
//发送函数
typedef int(*Send_Socket)(void* handle,unsigned char* buf,int bufLen);
//接收函数
typedef int(*Recv_Socket)(void* handle,unsigned char* buf,int* bufLen);
//销毁函数
typedef int(*Destroy_Socket)(void** handle);


//通信模块
typedef struct _CSocketProtocol{
	//初始化
	Init_Socket init;
	//发送
	Send_Socket send;
	//接收
	Recv_Socket recv;
	//销毁
	Destroy_Socket destroy;
}CSocketProtocol;


//通信模块初始化
int Init_CSocketProtocol(CSocketProtocol* csp, 
	Init_Socket init,
	Send_Socket send,
	Recv_Socket recv,
	Destroy_Socket destroy
	);