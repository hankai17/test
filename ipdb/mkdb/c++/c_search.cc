#include "c_search.hh"

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
	return buf;  
}  

static double getTime()
{
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);

    return (tv.tv_sec * 1000 + ((double)tv.tv_usec)/1000);
}

int init_file(std::ifstream& f, char** buf, int* file_s)
{
	std::ifstream file("ip.db",std::ios::in);
    if(!file.is_open())
    {
        std::cout<<"no ip.db file, please get ip.db first!\n";
        return 1;
    }
	int b = file.tellg();
	file.seekg(0,std::ios::end);	
	int e = file.tellg();
	int file_size = e - b;
	file.seekg(0,std::ios::beg);
	char* buffer = (char*)malloc(file_size);
	memset(buffer,0,file_size);
	file.read(buffer,file_size); 

    f = { std::move(file) };
    *buf = buffer;
    *file_s = file_size;
    return 0;
}

int search_ip(char* buffer, int file_size, std::string& ip, std::string& result)
{
    if(ip.length() == 0)
    {
        std::cout<<"ip can not is null!\n";
        return 1;
    }
    if(buffer == nullptr || file_size == 0)
    {
        std::cout<<"search buffer is null! or file_size is 0!\n";
        return 1;
    }
    result = "";

	char* tmp = buffer;
	int first_block = (int)get_uint(tmp+file_size-8,0);
	int last_block = (int)get_uint(tmp+file_size-8,4);
	int block_num = (last_block-first_block)/12 + 1;

    //char *ip = (char*)ip.data();
	unsigned int query_ip = inet_network((char*)ip.data());	
	int l = 0;
	int h = block_num;
	int data_ptr = 0;
	int m = 0;
	int p = 0;
	char* buf = buffer;

	unsigned int sip = 0;
	unsigned int eip = 0;

	double s_time, c_time;
	s_time = getTime();
	while(l <= h)
	{
		m = (l + h) >> 1; 	
		p = m*12 + first_block;
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
				break;
			}
		}
	}
	c_time = getTime() - s_time;
	std::cout<<"use "<<c_time<<" millseconds"<<std::endl;
	if(l > h)		
	{
		std::cout<<"not find!\n";
        return 1;
	}
	unsigned int d_len = ((data_ptr >> 24) & 0xFF);	
	unsigned int d_ptr = (data_ptr & 0x00FFFFFF);
	char dat[d_len + 1] = {0};
    
	strncpy(dat,buffer+d_ptr,d_len);
    result = {dat};
	return 0;
}

int close_file(std::ifstream& file, char* buffer)
{
	file.close();
    if(buffer != nullptr)
    {
        delete buffer;
        buffer = nullptr;
    }
    return 0;
}

