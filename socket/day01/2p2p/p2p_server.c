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

void handle(int sig)
{
	printf("child recv sig=%d\n",sig);
	exit(0);
}

int main(void)
{
	int lfd = 0;
	signal(SIGUSR1,handle);
	lfd =  socket(AF_INET, SOCK_STREAM , 0);

	struct sockaddr_in addrin;
	addrin.sin_family = AF_INET;
	addrin.sin_port = htons(8001);//��һ�������͵ı����ֽ���ת�������ֽ���   ͨ��man�ɲ鵽�����Ϣ
	addrin.sin_addr.s_addr = inet_addr("127.0.0.1");

	int optval = 1;
	if (setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
	{
		perror("setsockopt bind\n");
		exit(0);
	}
	//�󶨶˿�
	if(bind(lfd, (const struct sockaddr *)&addrin,  sizeof(addrin)) == -1)
	{
		perror("fun bind\n");
		exit(0);
	}
	if ( listen(lfd, SOMAXCONN) <0  )
	{
		perror("fun listen\n");
		exit(0);
	}
	struct sockaddr_in  peeraddr;
	socklen_t peerlen = sizeof(peeraddr);

	unsigned int cfd = 0;
	cfd = accept(lfd,  (struct sockaddr *)&peeraddr, (socklen_t *)&peerlen);
	if (cfd == -1)
	{
		perror("fun listen\n");
		exit(0);
	}
	char revbuf[1024] = {0};

	pid_t pid;
	pid = fork(); //�����̸����� �ӽ��̷�
	if(pid == -1)
	{
		perror("fork error:");
		close(lfd);
		close(cfd);
		exit(0);
	}
	close(lfd);
	if(pid == 0) //��
	{
		//���ն���������ݣ����͵��ͻ�����
		char sendbuf[1024] = {0};
		while( fgets(sendbuf, sizeof(sendbuf), stdin) != NULL)
		{
			write(cfd, sendbuf, strlen(sendbuf));
			memset(sendbuf, 0, sizeof(sendbuf));
		}
		close(cfd);
	}
	else if( pid > 0)  //��
	{
		// ���տͻ�����Ϣ����ʾ���ն�
		char recvbuf[1024] = {0};
		while(1)
		{
			int ret = read(cfd, recvbuf, sizeof(revbuf));
			if(ret == 0)
			{ 
				printf("�Է��ѹر�");
				break; //�ͻ��˹ر� ��ȥ�� �ȹر�cfd Ȼ��֪ͨ�ӽ����������� 
			} //�����֪ͨ ����������˳� �ӽ����������б�ɹ¶����� 
			else if(ret < 0)
			{
				printf("��ȡʧ��");
				exit(0);
			}

			fputs(recvbuf, stdout);
			memset(recvbuf, 0, sizeof(recvbuf));
		}
		kill(pid,SIGUSR1);
		sleep(1); //��ֹ��ʬ���̣�����֤�ӽ���ִ���� 
		close(cfd);
		exit(0);
	}
	return 0;
}
//���� ���Ǹ�ͼ ������һЩ
