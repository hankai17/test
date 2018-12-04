#include"CEncDesProtocol.h"
#include"des.h"

int EncDataHw(unsigned char* src, int srcLen, unsigned char* dst, int* dstLen){
	return DesEnc(src, srcLen, dst, dstLen);
}
int DecDataHw(unsigned char* src, int srcLen, unsigned char* dst, int* dstLen){
	return DesDec(src, srcLen, dst, dstLen);
}