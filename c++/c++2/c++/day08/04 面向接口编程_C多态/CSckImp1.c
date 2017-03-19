#include"CSckImp1.h"


typedef struct _HD{
	unsigned char* data;
	int len;
}HD;

//初始化函数
int Init_Socket1(void** handle){

	if (handle == NULL){
		return -1;
	}
	HD* hd = (HD*)malloc(sizeof(HD));
	if (hd == NULL){
		return -2;
	}
	hd->data = NULL;
	hd->len = 0;

	*handle = hd;

	return 0;
}
//发送函数
int Send_Socket1(void* handle, unsigned char* buf, int bufLen){

	if (handle == NULL){
		return -1;
	}

	if (buf == NULL){
		return -2;
	}
	
	HD* hd = (HD*)handle;

	if (hd->data != NULL){
		free(hd->data);
		hd->data = NULL;
	}

	hd->data = malloc(bufLen);
	if (hd->data == NULL){
		return -3;
	}
	strcpy(hd->data, buf);
	hd->len = bufLen;

	return 0;
}
//接收函数
int Recv_Socket1(void* handle, unsigned char* buf, int* bufLen){

	if (handle == NULL){
		return -1;
	}

	if (buf == NULL){
		return -2;
	}

	if (bufLen == NULL){
		return -3;
	}

	HD* hd = (HD*)handle;

	strcpy(buf, hd->data);
	*bufLen = hd->len;

	return 0;
}
//销毁函数
int Destroy_Socket1(void** handle){
	if (handle == NULL){
		return -1;
	}

	HD* hd = (HD*)(*handle);

	if (hd->data != NULL){
		free(hd->data);
		hd->data = NULL;
	}

	free(hd);
	hd = NULL;

	return 0;
}