#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/ip.h>

void myhandle(int sig)
{
	printf("�ͻ��˸������˳�,parent recv sig=%d\n",sig);
	exit(0);
}

int main(void)
{
	int sock;
	sock =  socket(AF_INET, SOCK_STREAM , 0);
	struct sockaddr_in servaddr;
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(8001);
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if( connect(sock,  (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
	{
		perror("connect:");
		exit(0);
	}
	pid_t pid;
	pid = fork(); //�����̸��� �ӽ�����
	if(pid == -1)
	{
		perror("fork error:");
		close(sock);
		exit(0);
	}
	if(pid == 0) //��
	{
		char recvbuf[1024] = {0};
		while(1) 
		{
			memset(recvbuf, 0, sizeof(recvbuf));
			int ret = read(sock,recvbuf,sizeof(recvbuf));
			if(ret == -1)
			{
				perror("read err\n");
				break;
			}
			else if(ret == 0)
			{
				printf("peer close\n");
				break;
			}
			fputs(recvbuf,stdout);
		}
		close(sock);
		kill(getppid(),SIGUSR1);  //������� ���߸������� //���⽩ʬ����
		printf("�ͻ����ӽ����˳�\n");
		exit(0);
	}
	else
	{
		char sendbuf[1024] = {0};
		signal(SIGUSR1,myhandle);
		printf("�ͻ��˸�����Ҫ���������ݣ�\n");
		while( fgets(sendbuf,sizeof(sendbuf),stdin) != NULL)  //�ӽ������ˣ������������ڴ�
		{
			printf("�ͻ��˻�ȡ���ݳ���:%d\n",strlen(sendbuf));
			write(sock,sendbuf,strlen(sendbuf));
			memset(sendbuf,0,sizeof(sendbuf));
		}
		printf("�ͻ��˸������˳�\n");
	}
	return 0;
}
//���� ���Ǹ�ͼ ������һЩ
