#define _CRT_SECURE_NO_WARNINGS

#pragma once

#include<stdlib.h>
#include<stdio.h>
#include<string.h>

//初始化函数
int Init_Socket2(void** handle);
//发送函数
int Send_Socket2(void* handle, unsigned char* buf, int bufLen);
//接收函数
int Recv_Socket2(void* handle, unsigned char* buf, int* bufLen);
//销毁函数
int Destroy_Socket2(void** handle);