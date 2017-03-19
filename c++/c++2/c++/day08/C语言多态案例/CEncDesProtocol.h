#pragma once

typedef int(*PENC)(unsigned char* src, int srcLen, unsigned char* dst, int* dstLen);
typedef int(*PDEC)(unsigned char* src, int srcLen, unsigned char* dst, int* dstLen);

typedef struct _CEncDesProtocol{
	PENC enc;
	PDEC dec;
}CEncDesProtocol;

void Init_CEncDesProtocol(CEncDesProtocol* encDec, PENC enc, PDEC dec);

