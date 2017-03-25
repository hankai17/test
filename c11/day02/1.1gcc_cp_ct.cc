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
	return A();
}

int main()
{
	//A a = get_A();
	A&& a = get_A();
	return 0;
}
// 如果直接g++ -std=c++11的话 构造与析构都是一次 没有拷贝
//
// -fno-elide-constructors 关闭优化
// construct: 1
// copy construct: 1  //这就是真实的情况，元素是个复杂的东西 需要拷贝构造成匿名
// destruct: 1
// copy construct: 2 //没有优化，匿名不会转有名
// destruct: 2
// destruct: 3
// 也就是说 无缘无故多了1次拷贝 

//若用右值引用来接
//[root@localhost day02]# g++ 1.1gcc_cp_ct.cc -std=c++11 -fno-elide-constructors
//[root@localhost day02]# ./a.out 
//construct: 1
//copy construct: 1
//destruct: 1
//destruct: 2
//少了一次拷贝 有了右值引用，直接匿名转有名 没有拷贝
