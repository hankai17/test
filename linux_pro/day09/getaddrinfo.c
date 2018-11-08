#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char **argv) {
  if (argc != 2) {
	printf("a.out domain\n");
	exit(1);
  }
  struct addrinfo hints;
  struct addrinfo* res,*cur;
  int ret;
  struct sockaddr_in* addr;
  char ipbuf[16];
  memset(&hints, 0, sizeof(struct addrinfo));

  hints.ai_family = AF_INET; /* Allow IPv4 */
  hints.ai_flags = AI_PASSIVE; /* For wildcard IP address */
  hints.ai_protocol = 0; /* Any protocol */
  hints.ai_socktype = SOCK_STREAM;

  ret = getaddrinfo(argv[1], NULL, &hints, &res/*out*/);
  if (ret == -1) {
	perror("getaddrinfo");
	exit(1);
  }

  for (cur = res; cur != NULL; cur = cur->ai_next) { //遍历传出结果
	addr = (struct sockaddr_in *)cur->ai_addr;
	printf("%s\n", inet_ntop(AF_INET, &addr->sin_addr, ipbuf, 16));
  }
  freeaddrinfo(res);
  exit(0);
}
