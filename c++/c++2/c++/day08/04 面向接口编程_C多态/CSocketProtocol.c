#include"CSocketProtocol.h"


int Init_CSocketProtocol(CSocketProtocol* csp,
	Init_Socket init,
	Send_Socket send,
	Recv_Socket recv,
	Destroy_Socket destroy
	){

	if (csp == NULL){
		return -1;
	}

	if (init == NULL){
		return -2;
	}
	if (send == NULL){
		return - 3;
	}
	if (recv == NULL){
		return -4;
	}
	if (destroy == NULL){
		return -5;
	}

	csp->init = init;
	csp->send = send;
	csp->recv = recv;
	csp->destroy = destroy;

	return 0;
}