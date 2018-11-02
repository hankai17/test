
// keymng_dbop.h
#ifndef _KEYMNG_DBOP_H_
#define _KEYMNG_DBOP_H_


#include "keymngserverop.h"
#include "keymng_shmop.h"

#ifdef __cplusplus
extern "C" {
#endif



int KeyMngsvr_DBOp_GenKeyID(void *dbhdl, int *keyid);

int KeyMngsvr_DBOp_WriteSecKey(void *dbhdl, NodeSHMInfo *pNodeInfo); 


#ifdef __cplusplus
}
#endif

#endif



