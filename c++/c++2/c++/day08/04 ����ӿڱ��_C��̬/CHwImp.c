#include"CHwImp.h"
#include"des.h"


int EncDataHw(unsigned char* inBuf, int inBufLen, unsigned char* outBuf, int* outBufLen){
	return DesEnc(inBuf, inBufLen, outBuf, outBufLen);
}
int DecDataHw(unsigned char* inBuf, int inBufLen, unsigned char* outBuf, int* outBufLen){
	return DesDec(inBuf, inBufLen, outBuf, outBufLen);
}