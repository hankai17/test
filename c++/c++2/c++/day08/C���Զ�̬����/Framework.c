#include"Framework.h"

int framework(CSocketProtocol* socketProtocol,
	unsigned char* sendBuf, int sendBufLen,
	unsigned char* recvBuf, int* recvBufLen){

	if (socketProtocol == NULL){
		return -1;
	}

	if (sendBuf == NULL){
		return -2;
	}

	if (recvBuf == NULL){
		return -3;
	}

	if (recvBufLen == NULL){
		return -4;
	}

	//创建句柄
	void* handle = NULL;
	int ret = 0;
	//初始化
	if ((ret = socketProtocol->init(&handle)) != 0){
		printf("socketProtocol->init error: %d\n", ret);
		goto End;
	}

	//发送数据
	if ((ret = socketProtocol->send(handle, sendBuf, sendBufLen)) != 0){
		printf("socketProtocol->send error: %d\n", ret);
		goto End;
	}

	//接收数据
	if ((ret = socketProtocol->recv(handle, recvBuf, recvBufLen)) != 0){
		printf("socketProtocol->recv error: %d\n", ret);
		goto End;
	}

End:
	if ((ret = socketProtocol->destroy(&handle)) != 0){
		printf("socketProtocol->destroy error: %d\n", ret);
		return ret;
	}
	handle = NULL;

	return 0;
}


int frameworkPro(CSocketProtocol* socketProtocol,
	CEncDesProtocol* cEncDec,
	unsigned char* sendBuf, int sendBufLen,
	unsigned char* recvBuf, int* recvBufLen){

	if (socketProtocol == NULL){
		return -1;
	}

	if (sendBuf == NULL){
		return -2;
	}

	if (recvBuf == NULL){
		return -3;
	}

	if (recvBufLen == NULL){
		return -4;
	}

	//创建句柄
	void* handle = NULL;
	int ret = 0;
	//初始化
	if ((ret = socketProtocol->init(&handle)) != 0){
		printf("socketProtocol->init error: %d\n", ret);
		goto End;
	}

	unsigned char tempBuf[1024] = { 0 };
	int tempBufLen = 0;
	cEncDec->enc(sendBuf, sendBufLen, tempBuf, &tempBufLen);
	printf("加密前的数据:%s \n", sendBuf);
	printf("加密后的数据:%s \n", tempBuf);

	//发送数据
	if ((ret = socketProtocol->send(handle, tempBuf, tempBufLen)) != 0){
		printf("socketProtocol->send error: %d\n", ret);
		goto End;
	}

	memset(tempBuf, 0, 1024);
	tempBufLen = 0;

	//接收数据
	if ((ret = socketProtocol->recv(handle, tempBuf, &tempBufLen)) != 0){
		printf("socketProtocol->recv error: %d\n", ret);
		goto End;
	}

	//数据解密
	cEncDec->dec(tempBuf, tempBufLen, recvBuf, recvBufLen);

End:
	if ((ret = socketProtocol->destroy(&handle)) != 0){
		printf("socketProtocol->destroy error: %d\n", ret);
		return ret;
	}
	handle = NULL;

	return 0;
}