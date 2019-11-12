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

/*
char def_urlhash[41] = "8FA5809B819520D401E9B1927E2FE533F7CA0D40";
char def_infohash[41] = "CAE12441C384D15B723ACD37683B4D1D98C8AF61";
char	def_ppcip[64] = "10.0.140.173";
int	def_ppcport = 5001;
char	def_ppsip[64] = "1.1.1.1"; //用户解析出来的ip
int	def_messageid = 1;
int	def_siteid = 1;
char	def_sitename[64] = "baidu.com";
char	def_url[2048] = "soft.duote.org:81/down/ludashi2015setup@2345_4093.exe";
char	def_lbe2_url[2048] = "114.114.114.114:80/2Q2W8FA5809B819520D401E9B1927E2FE533F7CA0D40_0_CAE12441C384D15B723ACD37683B4D1D98C8AF61_4/soft.duote.org:81/down/ludashi2015setup@2345_4093.exe";
char	def_referer[2048] = "";
*/

char def_urlhash[41] = "4BCD9EE130CE6F77C87AB5889F295B0135764D84";
char def_infohash[41] = "B027291A116A9D15F6D843CB0AA301025676EE4A";
char	def_ppcip[64] = "10.0.140.173";
int	def_ppcport = 5001;
char	def_ppsip[64] = "1.1.1.1"; //用户解析出来的ip
int	def_messageid = 1;
int	def_siteid = 119;
char	def_sitename[64] = "baidu.com";
char	def_url[2048] = "";
char	def_referer[2048] = "";
char    def_lbe2_url[2048] = "114.114.114.114:80/2Q2W4BCD9EE130CE6F77C87AB5889F295B0135764D84_0_B027291A116A9D15F6D843CB0AA301025676EE4A_4/live1.cp21.ott.cibntv.net/lb.videocache.lecloud.com/102/47/21/letv-uts/14/ver_00_22-303258513-avc-479497-aac-32002-3087560-200885850-3b7c8c33d484d5113582b112ab96df04-1421982228770_mp4/ver_00_22_195_578_3_498388_127473964_0.ts?stream_id=lb_loop_jdjc_1000&amltag=99&mltag=99&platid=10&splatid=1060704002&qos=3&fcheck=0&lsbv=2hF&uid=301667752&keyitem=iDipdd1vuVFcNrewScXV_qA9anmyuuMxAiFPNbvcYujMEdbjduvlnqfflyUxsCMm&ntm=1573543200&nkey2=54ed2a91a8d8bc27b15901c3316262dd&nkey=5293cd304b3614f4e6fe882def9b1ba3&auth_key=1573543200-2-0-10-1060704002-lb_loop_jdjc_1000-a8988059384ebf3d8feb4d11ccc69067&tag=live&video_type=m3u8&useloc=0&mslice=5&ver=live_3&uidx=0&errc=0&gn=50048&ndtype=2&lrtmcd=104&buss=99&cips=114.113.88.214&geo=CN-1-0-174&tmn=1573524929&ext=m3u8&sign=live_tv&playid=1&termid=2&pay=0&tm=1573525228&ostype=andriod&hwtype=un&app_name=tvlive&app_ver=4.9.3.050&devid=macC80E7796B155&uuid=C80E7796B1551573524929140_0&station=lb_loop_jdjc&sign_dev=FEB9152AE24289F221DE42E778D8DA9E&app_code=2019101617&vid=2087019&p1=2&p2=20&p3=20_tvlive_cibn&wmac=c8:0e:77:d6:14:b0&lsse=WqWDV&cde=1121&cdeid=00b18b6cb1d8fb808d1c4c4c00000025&appid=3110&cdetm=1573525935&cdekey=6ed5580482a0b5dc1b1e393c89f098a0&tss=tvts&m3v=1&utp=701&key=4e57b4710f175c7bd8ecd55486d274fe&cdernd=779884&ffgname=streamGroup_323";

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

typedef struct _LBE_TO_TASK {
        unsigned char cmd;
        unsigned int messageid;
        unsigned char urlhash[40];
        unsigned char infohash[40];
        unsigned int serverip;
        unsigned char sitename[64];
        unsigned int siteid_lbe;
        unsigned char data[1]; //LBE_URL_FLAG url LBE_END_FLAG
}LBE_TO_TASK;

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
/*	
 	if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0)
	{
		close(sock);
		return -1;
	}
*/
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


	if(0) {
	int ret = sendto(sock,sendbuf, sendlen, 0, (struct sockaddr*)&target,sizeof(struct sockaddr_in)); //发送ppc
	if(ret < 0) {
		printf("send error\n");
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

}

	LBE_TO_TASK packet_dl;
	bzero((void*)&packet_dl, sizeof(LBE_TO_TASK));
	packet_dl.cmd = 0x8e; //lbe_to_lbe
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
	strcpy(p, def_lbe2_url); ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    printf("----------->p: %s len: %d\n", p, (int)strlen(def_lbe2_url));
	p = p + strlen(def_lbe2_url);
	*p = 0x0f;
	p++;
	*p = 0x03;
	p++;
	strcpy(p, def_referer);
	p = p + strlen(def_referer);
	*p = 0x0f;
	memcpy(sendbuf, (void*)&packet_dl, sizeof(LBE_TO_TASK));

	sendlen = 0;
	if(strlen(def_referer) != 0) {
		memcpy(sendbuf+(sizeof(LBE_TO_TASK)-1), (void*)&dat, (2+strlen(def_lbe2_url)+strlen(def_referer)+2));
		sendlen = sizeof(LBE_TO_TASK)+1+strlen(def_lbe2_url)+strlen(def_referer)+2;
	} else {
		memcpy(sendbuf+(sizeof(LBE_TO_TASK)-1), (void*)&dat, (2+strlen(def_lbe2_url)));
		sendlen = sizeof(LBE_TO_TASK)+1+strlen(def_lbe2_url);
	}

	printf("send lbe_2_lbe\n");
	int ret = sendto(sock,sendbuf, sendlen, 0, (struct sockaddr*)&target,sizeof(struct sockaddr_in));
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

