#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

unsigned int get_uint(char* buffer, int offset)
{
    return (
        ((buffer[offset  ]) & 0x000000FF) | 
        ((buffer[offset+1] <<  8) & 0x0000FF00) | 
        ((buffer[offset+2] << 16) & 0x00FF0000) |
        ((buffer[offset+3] << 24) & 0xFF000000)
    );
}

char* int_2_str(int ip, char *buf)  
{  
    sprintf(buf, "%u.%u.%u.%u",  
            (unsigned char )*((char *)&ip + 3),  
            (unsigned char )*((char *)&ip + 2),  
            (unsigned char )*((char *)&ip + 1),  
            (unsigned char )*((char *)&ip + 0));  
    //std::cout<<"buf="<<buf<<std::endl;
    return buf;  
}  

int main()
{
	//std::ifstream file("ip.db",std::ios::in|std::ios::binary);
	std::ifstream file("ip.db",std::ios::in);
	int b = file.tellg();
	file.seekg(0,std::ios::end);	
	int e = file.tellg();
	int file_size = e - b;
	file.seekg(0,std::ios::beg);	 //傻吊
	std::cout<<"file_size="<<file_size<<std::endl;
	char* buffer = (char*)malloc(file_size);
	memset(buffer,0,file_size);
	//读入内存
	file.read(buffer,file_size);  //写入的有0所以malloc的大小是无法求出来的

	//根据两个头 得出偏移地址	
	char* tmp = buffer;
	int first_block = (int)get_uint(tmp+file_size-8,0);  //第一个块 所在文件的位置处 单位为字节
	int last_block = (int)get_uint(tmp+file_size-8,4);
	std::cout<<"first_block="<<first_block<<std::endl;
	std::cout<<"last_block="<<last_block<<std::endl;
	int big_block_num = (last_block-first_block)/12/341 + 1;
	int block_num = (last_block-first_block)/12 + 1;
	std::cout<<"block_num="<<block_num<<std::endl;
	
	//试试吧
	//char ip[16] = "101.4.104.0";
	char ip[16] = "201.4.104.0";
	unsigned int query_ip = inet_network(ip);	
	int l = 0;
	int h = block_num;
	int data_ptr = 0;
	int m = 0;
	int p = 0;
	char* buf = buffer;

	//unsigned int t = get_uint((char*)((unsigned long)buffer+(int)first_block+12*10976), 0);
	//std::cout<<"pos="<<(unsigned long)buffer+(int)first_block + 8 + big_block_num*12 <<std::endl;
	//std::cout<<"t="<<t<<std::endl;
	//char tmp1[32] = {0};
	//int_2_str(t,tmp1);
	//std::cout<<"tmp1="<<tmp1<<std::endl;

	unsigned int sip = 0;
	unsigned int eip = 0;

	while(l <= h)
	{
		m = (l + h) >> 1; //取中间行	
		p = m*12 + first_block; //没行12字节 
		buf = buffer + p; 	
		sip = get_uint(buf,0);
		if(query_ip < sip)
		{
			h = m - 1;
		}
		else
		{
			eip = get_uint(buf,4);	
			if(query_ip > eip)
			{
				l = m + 1;
			}
			else
			{
				data_ptr = get_uint(buf,8);
				std::cout<<"find!!!!!!\n";
				break;
			}
		}
	}
	if(l > h)		
	{
		std::cout<<"not find!!!!!!\n";
	}
	//buf = buffer + data_ptr + 8 + big_block_num*12;
	//std::cout<<"buf="<<buf<<std::endl;

	file.close();
	return 0;
}

