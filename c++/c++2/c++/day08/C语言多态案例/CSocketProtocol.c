#include"CSocketProtocol.h"

void Init_CSocketProtocol(CSocketProtocol* sp, Init_CSocket init, Send_CSocket send, Recv_CSocket recv, Destroy_CSocket destroy){
	sp->init = init;
	sp->send = send;
	sp->recv = recv;
	sp->destroy = destroy;
}