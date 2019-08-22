#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
using namespace std;

#define SERV_IP "127.0.0.1"
#define SERV_PORT 8001

int main(void)
{
  int sfd, len;
  struct sockaddr_in serv_addr;
  char buf[BUFSIZ]; 

  sfd = socket(AF_INET, SOCK_STREAM, 0);

  bzero(&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  inet_pton(AF_INET, SERV_IP, &serv_addr.sin_addr.s_addr);
  serv_addr.sin_port = htons(SERV_PORT);

  connect(sfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

  while (1) {
	sleep(4);
	cout<<"sleep 4s over"<<endl;
	/*从标准输入获取数据*/
	fgets(buf, sizeof(buf), stdin);
	/*将数据写给服务器*/
	write(sfd, buf, strlen(buf));       //写个服务器
	/*从服务器读回转换后数据*/
	len = read(sfd, buf, sizeof(buf));
    if (len == 0) {
	  cout<<"peer closed"<<endl;
	  close(sfd);
 	  return 0;
    }
	/*写至标准输出*/
	write(STDOUT_FILENO, buf, len);
  }

  close(sfd);
  return 0;
}

