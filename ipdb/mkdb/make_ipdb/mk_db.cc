#include <fstream>
#include <iostream>
#include <vector>
#include <string.h>
#include <string>
#include <list>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct block
{
	unsigned int ip_b;
	unsigned int ip_e;
	unsigned int data_ptr;
	unsigned int data_len;
}
block,*pblock;

typedef struct entry
{
	unsigned int ip_b;
	unsigned int ip_e;
	unsigned int data_ptr;
}
entry,*pentry;

typedef struct head_block
{
	unsigned int ip_b;
	unsigned int data_ptr;
}
head_block,*phead_block;

std::list<block*> block_l;
std::list<head_block> head_block_l;

void parse_buf(char* buffer, std::ofstream& f1)
{
	if(buffer == nullptr)
		return;
	std::string buf = {buffer};
	std::string ip = buf.substr(0,buf.find(" "));
	std::string region = buf.substr(buf.find(" ")+1, buf.find(" ",buf.find(" ")+1)-buf.find(" ")-1);
	std::string isp = buf.substr(buf.find_last_of(" ")+1, buf.length());
	std::string detail = region + " " + isp;

	std::string ip_b = ip.substr(0, ip.find("/"));
	std::string ip_mask = ip.substr(ip.find("/")+1, ip.length());
	unsigned int ip_int = inet_network((char*)(ip_b.data()));	
	unsigned int ip_e = ip_int + (2 << (32 - std::stoi(ip_mask) -1)) - 1;

	pblock p = (struct block*)malloc(sizeof(block));
	block_l.push_back(p);
	int pos = f1.tellp();
	p->data_ptr = pos;
	p->data_len = detail.length();
	p->ip_b = ip_int;
	p->ip_e = ip_e;
	f1.write((char*)detail.data(),detail.length());

	return;
}


int main(int argc,char*argv[])
{
    if(argc != 2)
    {
       std::cout<<"use example:\n"
                <<"./mk_db ip.txt\n";
       return 0;
    }
     
	char buffer[256] = {0};
	std::ofstream f1("ip.db",std::ios::binary|std::ios::out|std::ios::trunc);
	std::ifstream file(argv[1],std::ios::in);
	if(!file.is_open() || !f1.is_open())
	{
		std::cerr<<"open error!"<<std::endl;
		abort( );
	}

	while(file.peek() != EOF)
	{
		file.getline(buffer,256);
		parse_buf(buffer,f1);
		memset(buffer,0,sizeof(buffer));
	}
	int first_block = f1.tellp();

	int j = 0;	
	for(auto& b : block_l)
	{
		if(j == 0)
		{
			head_block h;
			h.ip_b = b->ip_b;
			h.data_ptr = b->data_ptr;
			head_block_l.push_back(h);
		}
		unsigned int l = ( b->data_ptr ) | ( (b->data_len << 24) & 0xff000000 ); //写入的region内容不能大于16M
		entry e = {b->ip_b, b->ip_e, l};	
		f1.write((char*)&e,sizeof(e));

		j++;
		if(j == 341)
		{
			j = 0;
		}
	}
	int last_block = (int)f1.tellp() - 12;

	for(auto& h : head_block_l) 
	{
		f1.write((char*)&h,sizeof(h));
	}

	f1.write((char*)&first_block,sizeof(first_block));	
	f1.write((char*)&last_block,sizeof(last_block));	
	
	f1.close();
	file.close();
	for(auto& b : block_l)	
	{
		if(b != nullptr)
		{
			delete b;
			b = nullptr;
		}
	}
	return 0;
}

