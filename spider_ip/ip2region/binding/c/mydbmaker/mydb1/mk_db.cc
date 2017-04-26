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
	//char* ip_b;	
	//char* ip_e;	
	unsigned int ip_b;
	unsigned int ip_e;
	int data_ptr;
	int data_len;
}block,*pblock;

typedef struct entry
{
	unsigned int ip_b;
	unsigned int ip_e;
	int data_ptr;
}entry,*pentry;

typedef struct head_block  //4k head
{
	unsigned int ip_b;
	int data_ptr;
}head_block,*phead_block;

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

	std::string ip_b = ip.substr(0, ip.find("/"));
	std::string ip_mask = ip.substr(ip.find("/")+1, ip.length());
	unsigned int ip_int = inet_network((char*)(ip_b.data()));	
	unsigned int ip_e = ip_int + (2 << (32 - std::stoi(ip_mask) -1));

	pblock p = (struct block*)malloc(sizeof(block));
	block_l.push_back(p);
	int pos = f1.tellp();
	p->data_ptr = pos;
	p->data_len = region.length();
	p->ip_b = ip_int;
	p->ip_e = ip_e;
	f1.write((char*)region.data(),region.length());
	return;
}


int main()
{
	char buffer[256] = {0};
	//std::ofstream f1("ip.db",std::ios::binary|std::ios::out|std::ios::trunc);  //must use write other << !
	std::ofstream f1("ip.db",std::ios::out|std::ios::trunc);  //must use write other << !
	std::ifstream file("ip.txt",std::ios::in);
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
		entry e = {b->ip_b, b->ip_e, b->data_ptr};	
		f1.write((char*)&e,sizeof(e));

		j++;
		if(j == 341)
		{
			j = 0;
		}
	}
	//std::cout<<f1.tellp()<<std::endl;	
	int last_block = (int)f1.tellp() - 12;
	//std::cout<<"head_block_l.size="<<head_block_l.size()<<std::endl;
	// 写4k头		
	//f1.seekp(0,std::ios::beg);
	std::cout<<"4k head="<<f1.tellp()<<std::endl;	
	for(auto& h : head_block_l) //写到开头 因为他是对全局的索引 而且写到开头很好找到这个位置 这样好  而写到结尾不好
	{
		f1.write((char*)&h,sizeof(h));
	}
	//std::cout<<f1.tellp()<<std::endl;	
	int num = (int)last_block-(int)first_block;
	//std::cout<<"last_block-first_block="<<num<<std::endl;
	//std::cout<<"block_num="<<num/12/341 + 1<<std::endl;

	//写头
	//f1.seekp(0,std::ios::beg);
	std::cout<<"head="<<f1.tellp()<<std::endl;	
	f1.write((char*)&first_block,sizeof(first_block));	
	f1.write((char*)&last_block,sizeof(last_block));	
	
	//
	

	f1.close();
	file.close();
	//释放内存
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

int main1()
{
	char buffer[256] = {0};
	std::ifstream file("ip.txt",std::ios::in);
	if(!file.is_open())
	{
		std::cerr<<"open error!"<<std::endl;
		abort( );
	}

	while(file.peek() != EOF)
	{
		//if(file.eof() == true)
		//	break;
		file.getline(buffer,256);
		std::cout<<"buffer="<<buffer<<std::endl;
		memset(buffer,0,sizeof(buffer));
	}
	


	file.close();
	return 0;
}

