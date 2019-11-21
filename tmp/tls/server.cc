#include <stdio.h>  
#include <stdlib.h>  
#include <errno.h>  
#include <string.h>  
#include <sys/types.h>  
#include <netinet/in.h>  
#include <sys/socket.h>  
#include <sys/wait.h>  
#include <unistd.h>  
#include <arpa/inet.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include <openssl/ssl.h>  
#include <openssl/err.h>  
  
#define MAXBUF 1024  
  
int main(int argc, char **argv)  
{  
  int sockfd, new_fd, fd;  
  socklen_t len;  
  struct sockaddr_in my_addr, their_addr;  
  unsigned int myport, lisnum;  
  char buf[MAXBUF + 1];  
  char new_fileName[50]="/newfile/";  
  SSL_CTX *ctx;  
  mode_t mode;  
  char pwd[100];  
  char* temp;  
  
  /* 在根目录下创建一个newfile文件夹 */  
  mkdir("/newfile",mode);  
  
  if (argv[1])  
    myport = atoi(argv[1]);  
  else  
  {  
    myport = 7838;  
    argv[2]=argv[3]=NULL;  
  }  
  
  if (argv[2])  
    lisnum = atoi(argv[2]);  
  else  {  
    lisnum = 2;  
    argv[3]=NULL;  
  }  
  
  SSL_library_init();  //SSL库初始化
  OpenSSL_add_all_algorithms();  //载入所有SSL算法 
  SSL_load_error_strings();  //载入所有 SSL 错误消息
  ctx = SSL_CTX_new(SSLv23_server_method());  //以 SSL V2 和 V3 标准兼容方式产生一个 SSL_CTX ，即 SSL Content Text
  /* 也可以用 SSLv2_server_method() 或 SSLv3_server_method() 单独表示 V2 或 V3标准 */  

  if (ctx == NULL)  {  
    ERR_print_errors_fp(stdout);  
    exit(1);  
  }

  getcwd(pwd,100);  
  if(strlen(pwd)==1)  
    pwd[0]='\0';  
  if (SSL_CTX_use_certificate_file(ctx, temp=strcat(pwd,"/cacert.pem"), SSL_FILETYPE_PEM) <= 0) {  //载入数字证书， 此证书用来发送给客户端。 证书里包含有公钥
    ERR_print_errors_fp(stdout);  
    exit(1);  
  }  

  getcwd(pwd,100);  
  if(strlen(pwd)==1)  
    pwd[0]='\0';  
  if (SSL_CTX_use_PrivateKey_file(ctx, temp=strcat(pwd,"/privkey.pem"), SSL_FILETYPE_PEM) <= 0)  {  //载入私钥
    ERR_print_errors_fp(stdout);
    exit(1);
  }

  if (!SSL_CTX_check_private_key(ctx)) {  //检查私钥是否正确
    ERR_print_errors_fp(stdout);  
    exit(1);  
  }  
  
  if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {  
    perror("socket");  
    exit(1);  
  }  else  
    printf("socket created\n");  
  
  bzero(&my_addr, sizeof(my_addr));  
  my_addr.sin_family = PF_INET;  
  my_addr.sin_port = htons(myport);  
  if (argv[3])  
    my_addr.sin_addr.s_addr = inet_addr(argv[3]);  
  else  
    my_addr.sin_addr.s_addr = INADDR_ANY;  
  
  if (bind(sockfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr)) == -1) {
    perror("bind");  
    exit(1);  
  }  else  
    printf("binded\n");  
  
  if (listen(sockfd, lisnum) == -1) {  
    perror("listen");  
    exit(1);  
  }  else  
    printf("begin listen\n");  
  
  while (1) {
    SSL *ssl;  
    len = sizeof(struct sockaddr);  
    if ((new_fd = accept(sockfd, (struct sockaddr *) &their_addr, &len)) == -1) {  
      perror("accept");  
      exit(errno);  
    }  else  
      printf("server: got connection from %s, port %d, socket %d\n", inet_ntoa(their_addr.sin_addr), ntohs(their_addr.sin_port), new_fd);  
  
    ssl = SSL_new(ctx);  //基于ctx产生一个新的SSL
    SSL_set_fd(ssl, new_fd);  //将连接用户的socket加入到SSL
    if (SSL_accept(ssl) == -1) {  //建立SSL连接 //1在调用 SSL_accept 之前，可能需要进行以下的设置: a)关联证书信息到SSL对象 b)设置SNI/Cert Callback函数 c)设置Session Ticket Callback函数
      perror("accept");  
      close(new_fd);  
      break;  
    }  
  
    //接受客户端所传文件的文件名并在特定目录创建空文件
    bzero(buf, MAXBUF + 1);  
    bzero(new_fileName+9, 42);  
    len = SSL_read(ssl, buf, MAXBUF);
    if(len == 0)  
      printf("Receive Complete !\n");  
    else if(len < 0)
      printf("Failure to receive message ! Error code is %d，Error messages are '%s'\n", errno, strerror(errno));  
    if((fd = open(strcat(new_fileName,buf),O_CREAT | O_TRUNC | O_RDWR,0666))<0)  {  
      perror("open:");
      exit(1);  
    }  
  
    /* 接收客户端的数据并写入文件 */  
    while(1) {
      bzero(buf, MAXBUF + 1);  
      len = SSL_read(ssl, buf, MAXBUF);  
      if(len == 0) {  
        printf("Receive Complete !\n");  
        break;  
      } else if(len < 0) {  
        printf("Failure to receive message ! Error code is %d，Error messages are '%s'\n", errno, strerror(errno));  
        exit(1);  
      }  

      if(write(fd,buf,len)<0) {  
        perror("write:");  
        exit(1);  
      }  
    }  
  
    /* 关闭文件 */  
    close(fd);  
    /* 关闭 SSL 连接 */  
    SSL_shutdown(ssl);  
    /* 释放 SSL */  
    SSL_free(ssl);  
    /* 关闭 socket */  
    close(new_fd);  
  }  
  
  /* 关闭监听的 socket */  
  close(sockfd);  
  /* 释放 CTX */  
  SSL_CTX_free(ctx);  
  return 0;  
}
