#include <iostream>
using namespace std;
#include "vector"
#include "algorithm"

void main11()
{
	vector<int> v1;//1定义容器v1 盛放int类型
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);//元素考到了容器中
	v1.push_back(-1);
	// 1  3  5
	//2迭代器：相当于一个指针 
	for (vector<int>::iterator it = v1.begin(); it != v1.end(); it++)
	{
		cout << *it << " ";
	}
	//3算法 算法和迭代器进行无缝连接
	//int num1 = count(v1.begin(), v1.end, 3);//从头到尾的遍历 超找3的个数 
	//cout << "mum1:" << num1 << endl;
}

class Teacher
{
public:
	int age;
	char name[64];
public:
	void printT()
	{
		cout << "age:" << age << endl;
	}
};

void main12()
{
	Teacher t1, t2, t3;
	t1.age = 31;
	t2.age = 32;
	t3.age = 33;


	vector<Teacher> v1;//1定义容器v1 盛放Teacher类型//容器实现了 数据类型和算法的有效分离
	v1.push_back(t1);
	v1.push_back(t2);
	v1.push_back(t3);//元素考到了容器中
	
	//2迭代器：相当于一个指针 
	for (vector<Teacher>::iterator it = v1.begin(); it != v1.end(); it++)
	{
		cout << it->age << " ";
	}
	//3算法 算法和迭代器进行无缝连接
	//int num1 = count(v1.begin(), v1.end, 3);//从头到尾的遍历 超找3的个数 
	//cout << "mum1:" << num1 << endl;
}

void main13()
{
	Teacher t1, t2, t3;
	t1.age = 31;
	t2.age = 32;
	t3.age = 33;
	Teacher * p1, *p2, *p3;
	p1 = &t1;
	p2 = &t2;
	p3 = &t3;


	vector<Teacher *> v1;//把t123的内存首地址放到了容器
	v1.push_back(p1);
	v1.push_back(p2);
	v1.push_back(p3);//元素考到了容器中
	// 1  3  5
	//2迭代器：相当于一个指针 指向p1 p2 p3
	for (vector<Teacher *>::iterator it = v1.begin(); it != v1.end(); it++)
	{
		cout << (*it)->age<< " ";
	}
	//3算法 算法和迭代器进行无缝连接
	//int num1 = count(v1.begin(), v1.end, 3);//从头到尾的遍历 超找3的个数 
	//cout << "mum1:" << num1 << endl;
}
void main()
{
	main11(); 
	//main12();
	//main13();
	system("pause");
	return;
}