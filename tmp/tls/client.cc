#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>
#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#define FAIL    -1

int OpenConnection(const char* hostname,int port) {  
    int sd;
    struct hostent *host;
    struct sockaddr_in addr;

    if( (host = gethostbyname(hostname)) == NULL ) {
        printf("Eroor: %s\n",hostname);
        perror(hostname);
        abort();
    }
    sd = socket(PF_INET, SOCK_STREAM, 0);
    bzero(&addr,sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = *(long*)(host->h_addr);

    if( connect(sd, (struct sockaddr*)&addr,sizeof(addr)) != 0 ) {
        close(sd);
        perror(hostname);
        abort();
    }
    return sd;
}

SSL_CTX* InitCTX(void) {   
    const SSL_METHOD *method;
    SSL_CTX *ctx;

    OpenSSL_add_all_algorithms(); /* Load cryptos, et.al. */
    SSL_load_error_strings();  /* Bring in and register error messages */
    method = SSLv2_client_method(); /* Create new client-method instance */
    ctx = SSL_CTX_new(method);  /* Create new context */ //1该对象可以用来承载多种属性，如： 加解密的算法 Session Cache Callback 证书 & 私钥 其它控制项

    if( ctx == NULL ) {
        ERR_print_errors_fp(stderr);
        printf("Eroor: %s\n",stderr);
        abort();
    }
    return ctx;
}

void ShowCerts(SSL* ssl) {   
    X509 *cert;
    char*line;
    cert = SSL_get_peer_certificate(ssl);/* get the server's certificate */

    if( cert != NULL ) {
        printf("Server certificates:\n");

        line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
        printf("Subject: %s\n", line);
        free(line);      /* free the malloc'ed string */

        line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
        printf("Issuer: %s\n", line);
        free(line);      /* free the malloc'ed string */

        X509_free(cert);    /* free the malloc'ed certificate copy */
    } else 
        printf("No certificates.\n");
}

int main(int count,char*strings[]) {   
    SSL_CTX *ctx;
    int server;
    SSL *ssl;
    char buf[1024];
    int bytes;
    char*hostname, *portnum;

    if( count != 3 ) {
        printf("usage: %s <hostname> <portnum>\n", strings[0]);
        exit(0);
    }

    SSL_library_init();
    hostname=strings[1]; //www.baidu.com
    portnum=strings[2];  //443

    ctx = InitCTX(); //获取客户端ctx

    server = OpenConnection(hostname, atoi(portnum)); //阻塞建联成功

    ssl = SSL_new(ctx);               /* create new SSL connection state */ //2建联成功后初始化ssl对象

    SSL_set_fd(ssl, server);          /* attach the socket descriptor */

                                    //在调用 SSL_connect 之前，可能需要进行以下的设置: a)关联证书信息到 SSL 对象 b)设置SNI c)设置 NPN/ALPN 参数
    if( SSL_connect(ssl) == FAIL ) {  /* perform the connection */ //3ssl握手整个过程: 发hello包 收hello包成功(rtt0) //验证证书 //协商会话秘钥(rtt1)
        printf("Eroor: %s\n",stderr);
        ERR_print_errors_fp(stderr);
    } else {
        char*msg ="HelloWorld";
        printf("Connected with %s encryption\n", SSL_get_cipher(ssl));
        ShowCerts(ssl);       /* get any certs */ //打印server证书

        SSL_write(ssl, msg, strlen(msg));  /* encrypt & send message */  //从指定内存地址获取信息，写入到 Write BIO，加密后发送
        bytes = SSL_read(ssl, buf,sizeof(buf));/* get reply & decrypt */ //从 Read BIO 读取信息，解密后写入到指定的内存地址

        buf[bytes] = 0;
        printf("Received: \"%s\"\n", buf);
        SSL_free(ssl);       /* release connection state */
    }

    close(server);        /* close socket */
    SSL_CTX_free(ctx);       /* release context */
    return 0;
}

/*
OpenSSL的库函数对于资源的读写都是通过 BIO 抽象层来完成的，例如 SSL_accept 和 SSL_connect SSL_read 和 SSL_write
我们会发现 BIO 与 VIO 非常相似但是又不一样： BIO抽象了对内存块和socket fd的访问，是一个资源描述符 VIO则是连接了socket fd与内存块，同时设置了数据流的方向，是一个数据管道
*/

/*
SSL_ERROR_WANT_READ 表示在调用read()时遇到了EAGAIN 需要在socket fd可读的时候，重新调用
SSL_ERROR_WANT_WRITE 表示在调用write()时遇到了EAGAIN 需要在socket fd可写的时候，重新调用
SSL_ERROR_WANT_ACCEPT 表示在调用SSL_accept()时遇到了阻塞的情况 需要在socket fd可读的时候，重新调用
SSL_ERROR_WANT_CONNECT 表示在调用SSL_connect()时遇到了阻塞的情况 需要在socket fd可写的时候，重新调用
SSL_ERROR_WANT_X509_LOOKUP 表示SNI/CERT Callback要求挂起SSL_accept过程 并且需要再次调用SSL_accept()来触发对SNI/CERT Callback的调用 直到SNI/CERT Callback允许SSL_accept过程继续 根据业务需要，在适当的时候重新调用SSL_accept
上面的错误状态都是遇到了阻塞的情况，只需要在适当的时候重新调用之前调用过的API就可以了。

SSL_ERROR_SYSCALL 表示在执行系统调用时遇到了特殊的错误 可以通过errno值来判断系统调用的错误类型
SSL_ERROR_SSL OpenSSL库函数出错，通常表示协议层出现了错误
SSL_ERROR_ZERO_RETURN 表示对端关闭了SSL连接 但是需要注意，这并不意味着TCP连接也一定关闭了
上面这些是真正的错误，需要相应的错误处理，例如：关闭连接，或者重试？

SSL_ERROR_NONE 表示操作正常完成，没有错误 只有这个，才是没有任何错误的完成了一个方法的调用。
*/
//https://github.com/hankai17/atsinternals/blob/master/CH03-IOCoreSSL/CH03S07-Basic-OpenSSL.md
