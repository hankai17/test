#include<thread>
#include<iostream>
#include<vector>

std::vector<std::thread> g_list;
std::vector<std::shared_ptr<std::thread>> g_list2;

void func() {
	std::cout<<"this is func \n";
	return;
}

void create_thread() {
	std::thread t(func);
	g_list.push_back(std::move(t)); //push_back时 会发生拷贝和析构动作 如果直接push t则发生析构和拷贝构造 这样显然是不行的
                                    //so 把这个左值变成右值直接push进去 c11的thread里面肯定有move语义 不会发生构造和析构
									//thread拷贝构造函数(被禁用)
	g_list2.push_back(std::make_shared<std::thread>(func));
}

int main()
{
	create_thread();
	for(auto& thread : g_list) {
		thread.join();
	}

	for(auto& thread : g_list2) {
		thread->join();
	}
	return 0;
}

