#include<thread>
#include<iostream>
#include<vector>

std::vector<std::thread> g_list;
std::vector<std::shared_ptr<std::thread>> g_list2;

void func()
{
	std::cout<<"this is func \n";
	return;
}

void create_thread()
{
	std::thread t(func);
	g_list.push_back(std::move(t)); //push_back时 会发生拷贝和西沟动作 
                                    //如果直接push t则发生西沟和拷贝构造 这样显然是不行的
                                    //so 把这个左值编程右值直接push进去 c11的thread里面肯定有move语义 不会发生构造和西沟
	g_list2.push_back(std::make_shared<std::thread>(func));
}

int main()
{
	create_thread();
	for(auto& thread : g_list)
	{
		thread.join();
	}
	for(auto& thread : g_list2)
	{
		thread->join();
	}
	return 0;
}

