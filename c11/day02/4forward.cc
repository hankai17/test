#include <iostream>

void PrintT(int& t)
{
	std::cout << "lvalue" << std::endl;
}

template <typename T>
void PrintT(T&& t)
{
	std::cout << "rvalue" << std::endl;
}

template <typename T>
void TestForward(T&& v)
{
	PrintT(v);
	PrintT(std::forward<T>(v));
	PrintT(std::move(v));
}

void Test()
{
	TestForward(1);  //l r r
	int x = 1;
	TestForward(x); //l l r
	TestForward(std::forward<int>(x)); //l r r 
}

int main(void)
{
	Test();
	return 0;
}
