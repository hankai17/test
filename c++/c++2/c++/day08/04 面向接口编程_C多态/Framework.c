#include"Framework.h"


//发送和接收数据
void FrameWork(CSocketProtocol* csp, unsigned char* sendBuf, int sendBufLen, unsigned char* recvBuf, int* recvBufLen){

	void* handle = NULL;
	int ret = 0;
	//通信初始化
	if ((ret = csp->init(&handle)) != 0){
		printf("csp->init error: %d\n", ret);
		return;
	}

	//发送数据
	if ((ret = csp->send(handle, sendBuf, sendBufLen) != 0)){
		printf("csp->send error: %d\n", ret);
		csp->destroy(&handle);
		return;
	}

	//接收数据
	if ((ret = csp->recv(handle, recvBuf, recvBufLen)) != 0){
		printf("csp->recv error: %d\n", ret);
		csp->destroy(&handle);
		return;
	}

	//销毁数据
	if ((ret = csp->destroy(&handle)) != 0){
		printf("csp->destroy error: %d\n", ret);
	}
}

//发送和接收数据
void FrameWorkPro(CSocketProtocol* csp,
	EncDec* ed,
	unsigned char* sendBuf, int sendBufLen,
	unsigned char* recvBuf, int* recvBufLen){

	void* handle = NULL;
	int ret = 0;
	//通信初始化
	if ((ret = csp->init(&handle)) != 0){
		printf("csp->init error: %d\n", ret);
		return;
	}

	//保存加密后的数据
	unsigned char encData[1024] = { 0 };
	int encLen = 0;
	//加密
	ed->enc(sendBuf, sendBufLen, encData, &encLen);

	//发送数据
	if ((ret = csp->send(handle, encData, encLen) != 0)){
		printf("csp->send error: %d\n", ret);
		csp->destroy(&handle);
		return;
	}


	unsigned char decData[1024] = { 0 };
	int decLen = 0;
	//接收数据
	if ((ret = csp->recv(handle, decData, &decLen)) != 0){
		printf("csp->recv error: %d\n", ret);
		csp->destroy(&handle);
		return;
	}
	//数据解密
	ed->dec(decData, decLen, recvBuf, recvBufLen);

	printf("加密数据:%s\n", decData);

	//销毁数据
	if ((ret = csp->destroy(&handle)) != 0){
		printf("csp->destroy error: %d\n", ret);
	}
}
