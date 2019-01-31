#ifndef _mtl_code_hh_
#define	_mtl_code_hh_

//#include "mtl_apidefs.h"
#include <openssl/md5.h>

typedef MD5_CTX MTL_DIGEST_CTX; //mtl_md5 context/

//all depericated
int mtl_code_md5(unsigned char *input, int input_length, unsigned char *sixteen_byte_hash_pointer);
char *mtl_code_md5_stringify(char *dest33, const size_t destSize, const char *md5);
char *mtl_code_md5_stringify_fast(char *dest33, const char *md5);

int mtl_code_incr_md5_init(MTL_DIGEST_CTX * context);
int mtl_code_incr_md5_update(MTL_DIGEST_CTX * context, const char *input, int input_length);
int mtl_code_incr_md5_final(char *sixteen_byte_hash_pointer, MTL_DIGEST_CTX *context);
#endif
