#include<iostream>
#include<thread>

void func(int a,double b)
{
	std::cout<<"this is callback func \n";
}

int main1()
{
	std::thread t1(std::bind(func,1,2));
	std::thread t2([](int a,double b){
		std::cout<<"this is lambda func \n";
					 },1,2);
	t1.join();
	t2.join();
	return 0;
}

void func1()
{
	std::cout<<"this is func1 move \n";
}

int main2()
{
	std::thread t(func1);
	std::thread t1(std::move(t));
	//t.join();
	t1.join();
	return 0;
}

