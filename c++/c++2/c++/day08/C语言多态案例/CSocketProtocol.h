#pragma once

//初始化
typedef int(*Init_CSocket)(void** handle);
//发送
typedef int(*Send_CSocket)(void* handle,unsigned char* buf,int bufLen);
//接收
typedef int(*Recv_CSocket)(void* handle, unsigned char* buf, int* bufLen);
//销毁
typedef int(*Destroy_CSocket)(void** handle);


typedef struct _CSocketProtocol{
	//初始化
	Init_CSocket init;
	//发送
	Send_CSocket send;
	//接收
	Recv_CSocket recv;
	//销毁
	Destroy_CSocket destroy;
}CSocketProtocol;

//初始化CSocketProtocol
void Init_CSocketProtocol(CSocketProtocol* sp, Init_CSocket init, Send_CSocket send, Recv_CSocket recv, Destroy_CSocket destroy);