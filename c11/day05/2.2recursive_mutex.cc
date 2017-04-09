#include <iostream>
#include <thread>
#include <mutex>

struct complex
{
	std::recursive_mutex mutex;
	int i;

	complex() : i(0){}

	void mul(int x)
	{
		std::lock_guard<std::recursive_mutex> lock(mutex);
		i *= x;
	}

	void div(int x)
	{
		std::lock_guard<std::recursive_mutex> lock(mutex);
		i /= x;
	}

	void both(int x, int y)
	{
		std::lock_guard<std::recursive_mutex> lock(mutex);
		mul(x);
		div(y);
	}
};

int main(void)
{
	complex c;

	c.both(32, 23);  //因为同一线程可以多次获取同一互斥量，不会发生死锁

	system("pause");
	return 0;
}
//解决同一线程 多次(连续)获取互斥量 而防止例子2那种死锁问题 
//但是尽量别使用 效率低 晦涩 有限制
