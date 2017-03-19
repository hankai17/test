#pragma once

#include<stdlib.h>
#include<stdio.h>
#include"CSocketProtocol.h"
#include"CEncDesProtocol.h"

int framework(CSocketProtocol* socketProtocol,
	unsigned char* sendBuf, int sendBufLen,
	unsigned char* recvBuf, int* recvBufLen);

int frameworkPro(CSocketProtocol* socketProtocol,
	CEncDesProtocol* cEncDec,
	unsigned char* sendBuf, int sendBufLen,
	unsigned char* recvBuf, int* recvBufLen);