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
	g_list.push_back(std::move(t));
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

