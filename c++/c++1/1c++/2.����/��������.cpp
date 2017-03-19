#include <iostream>
using namespace std;

void main()
{
	//普通引用
	int a=10;
	int &b = a;
	printf("b=%d\n", b);

	//常引用
	int x = 20;
	const int &y = x;//常引用是让引用只读属性 不能通过y修改x
	//常引用初始化有两种情况1：用变量初始化常引用 例如上面这个用x变量初始化 常引用
	//2：用字面量初始化常量引用
	
	const int a1 = 10;//c++编译器把a放在符号表中
	
	//int &m = 11;//普通引用 引用一个字面量 字面量没有分配内存地址
				//引用就是给内存取别名 前提必须是内存分配了
	const int &m = 43;//c++编译器会分配内存空间
	
	cout << "hello " << endl;
	system("pause");
}

struct Teacher
{
	char name[64];
	int age;
};

void printTeacher(const Teacher &myt)
{
	//myt.age = 33;//改不了的
	printf("myt.age:%d\n", myt.age);//常引用 让实参拥有只读属性
}

void main3()
{
	Teacher t1;
	t1.age = 366;
	printTeacher(t1);
	system("pause");
}
//常量引用初始化的两种方法
