/*************************************************************************
	> File Name: 0aio_read.cpp
	> Author: guaz
	> Mail: hankai17@126.com 
	> Created Time: 2017-02-09 16:26:31
 ************************************************************************/

#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<fcntl.h>
#include<aio.h>

#define BUFFER_SIZE 1024

int MAX_LIST = 2;

int main(int argc,char **argv)
{
    //aio��������ṹ��
    struct aiocb rd;
    int fd,ret,couter;

    fd = open("test.txt",O_RDONLY);
    if(fd < 0)
    {
        perror("test.txt");
    }

    bzero(&rd,sizeof(rd));//��rd�ṹ�����
    rd.aio_buf = malloc(BUFFER_SIZE + 1);//Ϊrd.aio_buf����ռ�
    //���rd�ṹ��
    rd.aio_fildes = fd;
    rd.aio_nbytes =  BUFFER_SIZE;
    rd.aio_offset = 0;

    //�����첽������
    ret = aio_read(&rd);
    if(ret < 0)
    {
        perror("aio_read");
        exit(1);
    }

    couter = 0;
//  ѭ���ȴ��첽����������
    while(aio_error(&rd) == EINPROGRESS)
    {
        printf("index:%d\n",++couter);
    }
    //��ȡ�첽������ֵ
    sleep(1);
    ret = aio_return(&rd);//���û�ж���͵����˴˺����ͻ�down��
    printf("return value: %d\n",ret);

    return 0;
}
