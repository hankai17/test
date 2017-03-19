#include"CEncDesProtocol.h"

int Init_EncDec(EncDec* ed, EncData enc, DecData dec){
	if (ed == NULL){
		return -1;
	}
	if (enc == NULL){
		return -2;
	}
	if (dec == NULL){
		return -3;
	}

	ed->enc = enc;
	ed->dec = dec;

	return 0;
}