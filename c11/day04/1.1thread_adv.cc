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
	g_list.push_back(std::move(t)); //如果直接push一个左值线程对象 就会调拷贝构造 会多出一个线程 这不是我们想要的
					//把这个左值变成右值直接push进去 c11的thread里面肯定有move语义 
									//thread拷贝构造函数(被禁用)
	g_list2.push_back(std::make_shared<std::thread>(func)); //vector的push只会发生拷贝构造
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

