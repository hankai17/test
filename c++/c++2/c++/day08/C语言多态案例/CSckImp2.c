#include"CSckImp2.h"

typedef struct _HD{
	char* buf;
	int len;
}HD;

int Init_SocketClinet2(void** handle){
	if (handle == NULL){
		return -1;
	}

	HD* hd = malloc(sizeof(HD));
	hd->buf = NULL;
	hd->len = 0;

	*handle = hd;

	return 0;
}
int Send_SocketClient2(void* handle, unsigned char* buf, int bufLen){

	if (handle == NULL){
		return -1;
	}

	if (buf == NULL){
		return -2;
	}

	HD* hd = (HD*)handle;

	hd->buf = malloc(strlen(buf) + 1);
	if (hd->buf == NULL){
		return -3;
	}
	strcpy(hd->buf, buf);
	hd->len = bufLen;

	return 0;
}
int Recv_SocketClient2(void* handle, unsigned char* buf, int* bufLen){

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
	strcpy(buf, hd->buf);
	*bufLen = hd->len;

	return 0;
}
int Destroy_SocketClient2(void** handle){

	if (handle == NULL){
		return -1;
	}

	HD* hd = (HD*)(*handle);

	if (hd->buf != NULL){
		free(hd->buf);
		hd->buf = NULL;
		hd->len = 0;
	}

	free(hd);
	*handle = NULL;

	return 0;
}