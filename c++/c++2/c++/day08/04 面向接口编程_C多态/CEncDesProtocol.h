#pragma once

#include<stdlib.h>

//加密数据
typedef int(*EncData)(unsigned char* inBuf, int inBufLen, unsigned char* outBuf, int* outBufLen);
typedef int(*DecData)(unsigned char* inBuf, int inBufLen, unsigned char* outBuf, int* outBufLen);

typedef struct _EncDec{
	//加密函数
	EncData enc;
	//解密函数
	DecData dec;
}EncDec;

int Init_EncDec(EncDec* ed, EncData enc, DecData dec);
