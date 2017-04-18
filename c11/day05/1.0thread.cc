#include<iostream>
#include<thread>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void func(int i,double d,const std::string& s)
{
	std::cout<< i << ", " << d << ", " << s <<std::endl;
	int fd = open("file", O_RDWR|O_TRUNC|O_CREAT, 0664);
}

int main1()
{
	std::thread t(func, 1, 2, "test");
	t.join();
	return 0;
}

int main()
{
	std::thread t(func, 1, 2, "test");
	t.detach();
	std::cout<<"other"<<std::endl;
	while(1);
	return 0;
}
