#include<iostream>

void print_value(int& i)
{
	std::cout<<"lvalue="<<i<<std::endl;
}

void print_value(int&& i)
{
	std::cout<<"rvalue="<<i<<std::endl;
}

void forward(int&& i)
{
	print_value(i);
}

int main()
{
	int i = 0;
	print_value(i); //l
	print_value(1); //r
	forward(2); //l 右值变成命名对象 是个左值
	return 0;
}
