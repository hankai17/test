#include<iostream>
using namespace std;

int g_construct_count = 0;
int g_copy_construct_count = 0;
int g_destruct_count = 0;

struct A
{
	A()
	{
		cout<<"construct: "<<++g_construct_count<<endl;
	}

	A(const A& a)
	{
		cout<<"copy construct: "<<++g_copy_construct_count<<endl;
	}

	~A()
	{
		cout<<"destruct: "<<++g_destruct_count<<endl;
	}
};

A get_A()
{
    A a;
	return a;
}

int main()
{
	A a = get_A();
	return 0;
}
//不关闭优化  结果是一样的
