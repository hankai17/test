/*************************************************************************
	> File Name: domain.cpp
	> Author: Bill
	> Mail: hankai17@126.com 
	> Created Time: 2017-02-04 10:36:52
 ************************************************************************/

#include<iostream>

int pointOfNum(char* str)
{
	 int nSum = 0 ;  
   char * p = str; 
    while (*p != '\0')  
    {  
        ++p ;
        if( *p == '.')
        {
        		++nSum;  
        }        
    }  
    return nSum ; 
}

int in_black_head(char * str_domain)
{
	char* tmp_addr;
	char *p = str_domain;
	while(*p != '\0')
	{
		//std::cout<<"*p:"<<*p<<std::endl;
		p++;
	}
	tmp_addr = p-1;
	//std::cout<<"*tmp_addr:"<<*tmp_addr<<std::endl;
	
	while(tmp_addr != str_domain)
	{
		tmp_addr -= 1;//	error tmp_addr --;	
		if( *tmp_addr == '.')
		{
				//std::cout<<"find:"<<tmp_addr<<std::endl;
		}
	}	

	return 0;
}

int in_black_rear(char * str_domain)
{
	std::cout<<"str_domain:"<<str_domain<<std::endl;
	char *tmp_addr = str_domain;
	char tmp_char;
	while(*tmp_addr != '\0')
	{
		if(*tmp_addr == '.')
		{
			tmp_addr += 1;
			tmp_char = *tmp_addr;  //std::cout<<"tmp_char:"<<tmp_char<<std::endl;
			*tmp_addr = '\0';
			std::cout<<"find:"<<str_domain<<std::endl;//ÕÒµ½Ôò·µ»Ø
			*tmp_addr = tmp_char;		
		}
		tmp_addr += 1;
	}
	return 0;	
}

int main()
{
	//char str_domain[] = "video.sport.sina.com";
	char str_domain[] = "a.b.c.d";
	int ret = in_black_rear(str_domain);
	std::cout<<"ret: "<<ret<<std::endl;
	return 0;
}


