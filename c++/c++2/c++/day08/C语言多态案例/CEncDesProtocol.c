#include"CEncDesProtocol.h"

void Init_CEncDesProtocol(CEncDesProtocol* encDec, PENC enc, PDEC dec){
	encDec->dec = dec;
	encDec->enc = enc;
}
