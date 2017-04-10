#include <iostream>
#include <thread>
#include <mutex>

struct complex
{
	std::mutex mutex;
	int i;

	complex() : i(0){}

	void mul(int x)
	{
		std::lock_guard<std::mutex> lock(mutex);
		i *= x;
	}

	void div(int x)
	{
		std::lock_guard<std::mutex> lock(mutex);
		i /= x;
	}

	void both(int x, int y)
	{
		std::lock_guard<std::mutex> lock(mutex);
		mul(x);
		div(y);
	}
};

int main(void)
{
	complex c;

	c.both(32, 23);

	system("pause");
	return 0;
}
//lock_guard 出了作用域自动解锁 大大简化lock/unlock的写法 
//会出现死锁 在both函数中的mul函数又要获取相同的互斥量 出现死锁

