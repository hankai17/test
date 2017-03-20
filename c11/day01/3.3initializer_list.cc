#include <iostream>
#include <vector>
#include <map>

class FooVector
{
	std::vector<int> content_;

public:
	FooVector(std::initializer_list<int> list)
	{
		for (auto it = list.begin(); it != list.end(); ++it)
		{
			content_.push_back(*it);
		}
	}
};

class FooMap
{
	std::map<int, int> content_;
	using pair_t = std::map<int, int>::value_type;  //using 可以简化对组的定义

public:
	FooMap(std::initializer_list<pair_t> list)
	{
		for (auto it = list.begin(); it != list.end(); ++it)
		{
			content_.insert(*it);
		}
	}
};

int main(void)
{
	FooVector foo1 = { 1, 2, 3, 4, 5 };
	FooMap foo2 = { { 1, 2 }, { 3, 4 }, { 5, 6 } };

	system("pause");
	return 0;
}

/* std::map<std::string,int> mm = {{"1",1},{"2",2},{"3",3}};
 * 如何让自定义的对象 也可以以大括号的方式初始化呢？  就得用initializer_list
 */




