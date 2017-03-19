#include <iostream>
using namespace std;
#include "vector"

void main31()//数组元素的 添加删除
{
	vector<int> v1;
	cout << "长度" << v1.size() << endl;

	v1.push_back(1);
	v1.push_back(4);
	v1.push_back(6);
	cout << "长度" << v1.size() << endl;

	cout << "头部元素" << v1.front() << endl;
	
	//修改头部元素的值
	v1.front() = 11;//函数返回值当左值 应该返回一个引用
	v1.back() = 55;//尾部元素改为55

	while (v1.size() > 0)
	{
		cout << "获取尾部元素" << v1.back() << endl;;//获取尾部元素
		v1.pop_back();//删除尾部元素

	}

}

void main32()//vector 初始化
{
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);
	v1.push_back(7);

	vector<int> v2 = v1;//拷贝构造初始化v2
	vector<int> v3(v1.begin(), v1.begin() + 2);//也是对象初始化

}

void printV(vector<int> &v)
{
	for (int i=0; i < v.size(); i++)
	{
		cout << v[i] << " ";
	}
}

void main33()//vector遍历 
{
	vector<int> v1(10);//提前把内存分配好
	for (int i = 0; i < 10; i++)
	{
		v1[i] = i + 1;

	}
	for (int i = 0; i < 10; i++)
	{
		printf("%d ",v1[i]);
	}
	printV(v1);
	

}

void main34()//pushback强化//在尾部添加 长度也会变
{
	vector<int> v1(10);//提前把内存分配好
	v1.push_back(1);
	v1.push_back(2);
	v1.push_back(3);
	printV(v1);

}

void main36()//vector的删除
{
	vector<int> v1(10);//提前把内存分配好
	for (int i = 0; i < 10; i++)
	{
		v1[i] = i + 1;

	}
	v1.erase(v1.begin(), v1.begin() + 3);//区间删除  前三个
	printV(v1);
	cout << endl;
	v1.erase(v1.begin());//根据元素位置删除
	printV(v1);
	cout << endl;
	v1[1] = 2;
	v1[3] = 2;
	printV(v1);
	cout << endl;
	for (vector<int>::iterator it = v1.begin(); it != v1.end(); )
	{
		if (*it == 2)
		{
			it=v1.erase(it);//当 删除迭代器所指向的元素的时候，eraser会自动的将it下移
		}
		else
		{
			it++;
		}

	}
	printV(v1);

	v1.insert(v1.begin(), 100);
	v1.insert(v1.end(), 100);
	printV(v1);

}

void main()
{
	main36();
	
	system("pause");
	return;
}

