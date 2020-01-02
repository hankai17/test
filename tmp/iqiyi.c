#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>
#include <sys/resource.h>
#include <pthread.h>
#include <sys/epoll.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

char def_urlhash[41] = "CD2F1D450A705551AE8F2C5D245B244700A41F46";
char def_infohash[41] = "CAE12441C384D15B723ACD37683B4D1D98C8AF61";
char	def_ppcip[64] = "172.16.3.99";
int	def_ppcport = 5001; //发给ppc的那个端口 80的ppc开得是5001
char	def_ppsip[64] = "172.16.3.99";
int	def_messageid = 1;
int	def_siteid =996;
char	def_sitename[64] = "baidu.com";
char	def_url[2048] = "103.216.43.56/videos/v0/20181003/27/e6/3fedc5ac7f38f5a17a635c58074a1dcf.f4v?key=0f7fa576d32c1646c9e5314b55c6d9315&dis_k=2e5bfa0aaa4661dfdaa29e218ced16cf0&dis_t=1577950517&dis_dz=CNIX-BeiJing&dis_st=103&src=iqiyi.com&dis_hit=0&uuid=727158b4-5e0d9d35-119&qd_k=a133be20a9287b6b1a2ffcebde0e059a&pv=0.1&client=&su=918e5202dbc1f5c9b45dfdd23f1df8c8&qd_stert=0&mi=tv_231024701_1391936900_6c60f16ec308c1316eda1dbff101eb1b&z=&qd_uid=&qd_tm=1577950503391&e=&cross-domain=1&ct=2&qd_src=01010031010000000000&qd_aid=231024701&qd_tvid=1391936900&qypid=1391936900_02020031010000000000&qd_p=674e7dc2&bt=&qd_vipdyn=0&qd_index=1&qd_vip=0&ve=&qyid=918e5202dbc1f5c9b45dfdd23f1df8c8&qd_vipres=0&tn=0.29866387139059025&range=0-8191";
char	def_referer[2048] = "";

#pragma pack(1)

typedef struct {
	unsigned char cmd;
	unsigned int ppcip;
	unsigned int port;
} LBE_REQ;

typedef struct _LBE_DET_TASK {
        unsigned char cmd;
        unsigned int messageid;
        unsigned char urlhash[40];
        unsigned int serverip;
        unsigned char sitename[64];
        unsigned int siteid_lbe;
        unsigned char data[1]; //LBE_URL_FLAG url LBE_END_FLAG
} LBE_DET_TASK;

typedef struct _LBE_SEND_DETECT {
        unsigned char cmd;
        unsigned int messageid;
        unsigned char urlhash[40];
        unsigned char infohash[40];
        unsigned int ppcip;
} LBE_SEND_DETECT;

typedef struct _LBE_DL_TASK {
        unsigned char cmd;
        unsigned int messageid;
        unsigned char urlhash[40];
        unsigned char infohash[40];
        unsigned int serverip;
        unsigned char sitename[64];
        unsigned int siteid_lbe;
        unsigned char data[1]; //LBE_URL_FLAG url LBE_END_FLAG
}LBE_DL_TASK;

typedef struct _LBE_ACK {
        unsigned char cmd;
        unsigned int messageid;
}LBE_ACK;

typedef struct _LBE_SEND_WHITE {
	unsigned char cmd;
	unsigned int messageid;
	unsigned char urlhash[40];
	unsigned char infohash[40];
	unsigned int ppcip;
	unsigned int port;
	unsigned char diskpath;
}LBE_SEND_WHITE;

typedef struct _LBE_SEND_BLACK {
	unsigned char cmd;
	unsigned int messageid;
	unsigned char urlhash[40];
	unsigned int ppcip;
	unsigned int port;
}LBE_SEND_BLACK;

typedef struct _LBE_SEND_DEL {
	unsigned char cmd;
	unsigned int messageid;
	unsigned char urlhash[40];
	unsigned char infohash[40];
	unsigned int ppcip;
}LBE_SEND_DEL;
#pragma pack()

int setfd_nonblock(int socket) {
	int f_old;
	f_old = fcntl(socket,F_GETFL,0);  
	if( f_old < 0 ) return -1;  
	f_old |= O_NONBLOCK;  
	return (fcntl(socket,F_SETFL,f_old));
}

int main() {
	int sock;
	struct sockaddr_in addr;

	memset(&addr, 0, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(5005);

	if((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0) return -1;
	int option = 1;

 	if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0)
	{
		close(sock);
		return -1;
	}

	if(bind(sock, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)) < 0) {
		close(sock);
		printf("bind error\n");
		return -1;
	}
/*
 	if(setfd_nonblock(sock) < 0) {
		close(sock);
		printf("setfd error\n");
		return -1;
	}
*/
	char buf[500];
	memset(buf, 0, sizeof(buf));

	struct sockaddr_in recv_addr;
	socklen_t socklen = (socklen_t)sizeof(recv_addr);
	while(1) { //接受包头开胃菜
		int r = recvfrom(sock, (char*)buf, sizeof(buf), 0, (struct sockaddr*)&recv_addr, &socklen);
		LBE_REQ* cmd = (LBE_REQ *)buf;
		printf("recv:%x,%d,%d\n", cmd->cmd, buf[0], cmd->port);
		if(cmd->cmd == 0x80)	break;
	}

	struct sockaddr_in target;
	bzero(&target, sizeof(struct sockaddr_in));
	target.sin_family = AF_INET;
	target.sin_addr.s_addr = inet_addr(def_ppcip);
	target.sin_port = htons(def_ppcport);

	char sendbuf[2048];
	memset(sendbuf, 0, sizeof(sendbuf));

	LBE_DET_TASK packet; //组织请求ppc报文
	bzero((void*)&packet, sizeof(LBE_DET_TASK));
	packet.cmd = 0x82;  //探测
	packet.messageid = def_messageid++;
	memcpy(packet.urlhash, def_urlhash, 40);
	packet.serverip = inet_addr(def_ppsip);
	strcpy(packet.sitename, def_sitename);
	packet.siteid_lbe = htonl(def_siteid);

	char dat[2048] = {0};
	char *p = dat;
	*p = 0x01;
	p++;
	strcpy(p, def_url);
	p = p + strlen(def_url);
	*p = 0x0f;
	p++;
	*p = 0x03;
	p++;
	strcpy(p, def_referer);
	p = p + strlen(def_referer);
	*p = 0x0f;
	memcpy(sendbuf, (void*)&packet, sizeof(LBE_DET_TASK));
	int sendlen = 0;
	if(strlen(def_referer) != 0) {
		memcpy(sendbuf+(sizeof(LBE_DET_TASK)-1), (void*)&dat, (2+strlen(def_url)+strlen(def_referer)+2));
		sendlen = sizeof(LBE_DET_TASK)+1+strlen(def_url)+strlen(def_referer)+2;
	} else {
		memcpy(sendbuf+(sizeof(LBE_DET_TASK)-1), (void*)&dat, (2+strlen(def_url)));
		sendlen = sizeof(LBE_DET_TASK)+1+strlen(def_url);
	}
	printf("---------------------->will send to\n");
	int ret = sendto(sock,sendbuf, sendlen, 0, (struct sockaddr*)&target,sizeof(struct sockaddr_in)); //发送ppc
	if(ret < 0) {
		printf("send error, errno: %s\n", strerror(errno));
		return -1;
	}
	printf("send det\n");

	int dl_flag = 0;
	while(1) {
		ret = recvfrom(sock, (char*)buf, sizeof(buf), 0, (struct sockaddr*)&recv_addr, &socklen); //阻塞等待
		unsigned char cmdtype = buf[0];
		if(cmdtype == 0x83) { //det ack
			LBE_SEND_DETECT * cmd_det_rec = (LBE_SEND_DETECT *)buf;
			printf("recv det ack:%x,%d\n", cmd_det_rec->cmd, cmd_det_rec->messageid);
		}
		if(cmdtype == 0x51) { //rec del
			LBE_SEND_DEL * cmd_del_rec = (LBE_SEND_DEL *)buf;
			char urlhash[41] = {0};
			char infohash[41] = {0};
			strncpy( urlhash , (char*)cmd_del_rec->urlhash , 40 );
			strncpy( infohash , (char*)cmd_del_rec->infohash , 40 );
			printf("recv del:%x,%d,%s,%s\n", cmd_del_rec->cmd, cmd_del_rec->messageid, urlhash, infohash);

			LBE_ACK packet_ack;
			bzero((void*)&packet_ack, sizeof(LBE_ACK));
			packet_ack.cmd = 0x52;
			packet_ack.messageid = cmd_del_rec->messageid;
			memcpy(sendbuf, (void*)&packet_ack, sizeof(LBE_ACK));
			sendlen = sizeof(LBE_ACK);

			ret = sendto(sock,sendbuf, sendlen, 0, (struct sockaddr*)&target,sizeof(struct sockaddr_in));
			if(ret < 0)
			{
				printf("send error\n");
				return -1;
			}

			dl_flag = 1;
		}
		if(cmdtype == 0x84) //det res
		{
			LBE_SEND_DETECT * cmd_det_rec = (LBE_SEND_DETECT *)buf;
			char urlhash[41] = {0};
			char infohash[41] = {0};
			strncpy( urlhash , (char*)cmd_det_rec->urlhash , 40 );
			strncpy( infohash , (char*)cmd_det_rec->infohash , 40 );
			strncpy( def_infohash , (char*)cmd_det_rec->infohash , 40 );
			printf("recv:%x,%d,%s,%s\n", cmd_det_rec->cmd, cmd_det_rec->messageid, urlhash, infohash);

			LBE_ACK packet_ack;
			bzero((void*)&packet_ack, sizeof(LBE_ACK));
			packet_ack.cmd = 0x85;
			packet_ack.messageid = cmd_det_rec->messageid;
			memcpy(sendbuf, (void*)&packet_ack, sizeof(LBE_ACK));
			sendlen = sizeof(LBE_ACK);

			ret = sendto(sock,sendbuf, sendlen, 0, (struct sockaddr*)&target,sizeof(struct sockaddr_in));
			if(ret < 0)
			{
				printf("send error\n");
				return -1;
			}

			dl_flag = 1;
		}
		if(dl_flag == 1 && cmdtype == 0x86)	//dl req
		{
			LBE_REQ * cmd = (LBE_REQ *)buf;
			printf("recv:%x,%d,%d\n", cmd->cmd, buf[0], cmd->port);
			break;
		}
		if(cmdtype == 0x8c)		//rec black
		{
			LBE_SEND_BLACK * cmd_black_rec = (LBE_SEND_BLACK *)buf;
			char urlhash[41] = {0};
			strncpy( urlhash , (char*)cmd_black_rec->urlhash , 40 );
			printf("recv black:%x,%d,%s\n", cmd_black_rec->cmd, cmd_black_rec->messageid, urlhash);

			LBE_ACK packet_ack;
			bzero((void*)&packet_ack, sizeof(LBE_ACK));
			packet_ack.cmd = 0x8d;
			packet_ack.messageid = cmd_black_rec->messageid;
			memcpy(sendbuf, (void*)&packet_ack, sizeof(LBE_ACK));
			sendlen = sizeof(LBE_ACK);

			ret = sendto(sock,sendbuf, sendlen, 0, (struct sockaddr*)&target,sizeof(struct sockaddr_in));
			if(ret < 0)
			{
				printf("send error\n");
				return -1;
			}

			dl_flag = 1;
		}
	}

	LBE_DL_TASK packet_dl;
	bzero((void*)&packet_dl, sizeof(LBE_DL_TASK));
	packet_dl.cmd = 0x88;
	packet_dl.messageid = def_messageid;
	memcpy(packet_dl.urlhash, def_urlhash, 40);
	memcpy(packet_dl.infohash, def_infohash, 40);
	packet_dl.serverip = inet_addr(def_ppsip);
	strcpy(packet_dl.sitename, def_sitename);
	packet_dl.siteid_lbe = htonl(def_siteid);
	memset(dat, 0, sizeof(dat));
	memset(sendbuf, 0, sizeof(sendbuf));
	p = dat;
	*p = 0x01;
	p++;
	strcpy(p, def_url);
	p = p + strlen(def_url);
	*p = 0x0f;
	p++;
	*p = 0x03;
	p++;
	strcpy(p, def_referer);
	p = p + strlen(def_referer);
	*p = 0x0f;
	memcpy(sendbuf, (void*)&packet_dl, sizeof(LBE_DL_TASK));

	sendlen = 0;
	if(strlen(def_referer) != 0) {
		memcpy(sendbuf+(sizeof(LBE_DL_TASK)-1), (void*)&dat, (2+strlen(def_url)+strlen(def_referer)+2));
		sendlen = sizeof(LBE_DL_TASK)+1+strlen(def_url)+strlen(def_referer)+2;
	} else {
		memcpy(sendbuf+(sizeof(LBE_DL_TASK)-1), (void*)&dat, (2+strlen(def_url)));
		sendlen = sizeof(LBE_DL_TASK)+1+strlen(def_url);
	}

	printf("send dl\n");
	ret = sendto(sock,sendbuf, sendlen, 0, (struct sockaddr*)&target,sizeof(struct sockaddr_in));
	if(ret < 0) {
		printf("send error\n");
		return -1;
	}

	while(1) {
		ret = recvfrom(sock, (char*)buf, sizeof(buf), 0, (struct sockaddr*)&recv_addr, &socklen);
		unsigned char cmdtype = buf[0];
		if(cmdtype == 0x8a) { //下载完毕 收到白名单
			LBE_SEND_WHITE * cmd_white_rec = (LBE_SEND_WHITE *)buf;
			char urlhash[41] = {0};
			char infohash[41] = {0};
			strncpy( urlhash , (char*)cmd_white_rec->urlhash , 40 );
			strncpy( infohash , (char*)cmd_white_rec->infohash , 40 );
			printf("recv:%x,%d,%s,%s\n", cmd_white_rec->cmd, cmd_white_rec->messageid, urlhash, infohash);

			LBE_ACK packet_ack;
			bzero((void*)&packet_ack, sizeof(LBE_ACK));
			packet_ack.cmd = 0x8b;
			packet_ack.messageid = cmd_white_rec->messageid;
			memcpy(sendbuf, (void*)&packet_ack, sizeof(LBE_ACK));
			sendlen = sizeof(LBE_ACK);

			ret = sendto(sock,sendbuf, sendlen, 0, (struct sockaddr*)&target,sizeof(struct sockaddr_in));
			if(ret < 0) {
				printf("send error\n");
				return -1;
			}
		}
		if(cmdtype == 0x51)		//rec del
		{
			LBE_SEND_DEL * cmd_del_rec = (LBE_SEND_DEL *)buf;
			char urlhash[41] = {0};
			char infohash[41] = {0};
			strncpy( urlhash , (char*)cmd_del_rec->urlhash , 40 );
			strncpy( infohash , (char*)cmd_del_rec->infohash , 40 );
			printf("recv del:%x,%d,%s,%s\n", cmd_del_rec->cmd, cmd_del_rec->messageid, urlhash, infohash);

			LBE_ACK packet_ack;
			bzero((void*)&packet_ack, sizeof(LBE_ACK));
			packet_ack.cmd = 0x52;
			packet_ack.messageid = cmd_del_rec->messageid;
			memcpy(sendbuf, (void*)&packet_ack, sizeof(LBE_ACK));
			sendlen = sizeof(LBE_ACK);

			ret = sendto(sock,sendbuf, sendlen, 0, (struct sockaddr*)&target,sizeof(struct sockaddr_in));
			if(ret < 0)
			{
				printf("send error\n");
				return -1;
			}
		}
		if(cmdtype == 0x8c)		//rec black
		{
			LBE_SEND_BLACK * cmd_black_rec = (LBE_SEND_BLACK *)buf;
			char urlhash[41] = {0};
			strncpy( urlhash , (char*)cmd_black_rec->urlhash , 40 );
			printf("recv black:%x,%d,%s\n", cmd_black_rec->cmd, cmd_black_rec->messageid, urlhash);

			LBE_ACK packet_ack;
			bzero((void*)&packet_ack, sizeof(LBE_ACK));
			packet_ack.cmd = 0x8d;
			packet_ack.messageid = cmd_black_rec->messageid;
			memcpy(sendbuf, (void*)&packet_ack, sizeof(LBE_ACK));
			sendlen = sizeof(LBE_ACK);

			ret = sendto(sock,sendbuf, sendlen, 0, (struct sockaddr*)&target,sizeof(struct sockaddr_in));
			if(ret < 0)
			{
				printf("send error\n");
				return -1;
			}
		}
	}

	close(sock);
	return 0;
}

