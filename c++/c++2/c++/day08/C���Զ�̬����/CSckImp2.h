#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include<stdlib.h>
#include<string.h>

int Init_SocketClinet2(void** handle);
int Send_SocketClient2(void* handle, unsigned char* buf, int bufLen);
int Recv_SocketClient2(void* handle, unsigned char* buf, int* bufLen);
int Destroy_SocketClient2(void** handle);