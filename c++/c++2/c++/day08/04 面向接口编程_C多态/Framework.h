#pragma once

#include"CEncDesProtocol.h"
#include"CSocketProtocol.h"

void FrameWork(CSocketProtocol* csp, 
	unsigned char* sendBuf, 
	int sendBufLen, 
	unsigned char* recvBuf, 
	int* recvBufLen);

void FrameWorkPro(CSocketProtocol* csp,
	EncDec* ed,
	unsigned char* sendBuf, int sendBufLen,
	unsigned char* recvBuf, int* recvBufLen);