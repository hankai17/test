#include<iostream>
#include<thread>

void func(int i,double d,const std::string& s)
{
	std::cout<< i << ", " << d << ", " << s <<std::endl;
}

int main()
{
	std::thread t(func, 1, 2, "test");
	t.join();
	return 0;
}
