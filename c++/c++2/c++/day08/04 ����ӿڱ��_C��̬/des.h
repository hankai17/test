/*********************************************************
 *  des.h
 *  用户使用des算法头文件
 *	
 *********************************************************/
#ifndef _OPENDESS_H_
#define _OPENDESS_H_

#ifdef __cplusplus
extern "C" {
#endif

//加密数据（encrypt：加密）
//pInData：明文数据
//nInDataLen：明文数据长度
//pOutData：加密后的数据
//pOutDataLen：加密数据的长度
int DesEnc(
		unsigned char *pInData,
		int            nInDataLen,
		unsigned char *pOutData,
		int           *pOutDataLen);


//解密数据(decrypt：解密)
//pInData：密文数据
//nInDataLen：密文数据长度
//pOutData：解密后的数据
//pOutDataLen：解密数据的长度
int DesDec(
	   unsigned char *pInData,
	   int            nInDataLen,
	   unsigned char *pOutData,
	   int           *pOutDataLen);

#ifdef __cplusplus
}
#endif

#endif

