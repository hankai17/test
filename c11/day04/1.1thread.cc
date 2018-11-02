#include<iostream>
#include<thread>
#include <functional> //bind

void func(int a,double b) {
	std::cout<<"this is callback func "<<a<<b<<"\n";
}

int main1()
{
	std::thread t1(std::bind(func, 1, 2));
	std::thread t2([](int a,double b) {
		std::cout<<"this is lambda func"<<a<<b<< "\n";
					 },1,2);
	t1.join();
	t2.join();
	return 0;
}

void func1() {
	std::cout<<"this is func1 move \n";
}

int main()
{
	std::thread t(func1);
	//std::thread t1(std::move(t));
	std::thread t1(t);
	//t.join();
	t1.join();
	return 0;
}

