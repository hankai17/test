//重要
#define  _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
#include "set"
#include "functional"

void main91()//集合 元素是1 自动排序 不能按照数组的方式插入元素//理论基础 红黑树
{				//默认从小到大排序
	set<int> set1;
	for (int i = 0; i < 5; i++)
	{
		int tmp = rand();//随机的插入
		set1.insert(tmp);
	}
	set1.insert(100);//只插入一个 一共6个元素！！
	set1.insert(100);
	set1.insert(100);

	for (set<int>::iterator it = set1.begin(); it != set1.end(); it++)
	{
		cout << *it << " ";
	}

	while (!set1.empty())//删除集合
	{
		set<int>::iterator it = set1.begin();
		cout << *it << " ";
		set1.erase(set1.begin());
	}

}

void main92()//对基本的数据类型这样排序 对于复杂的数据类型Teacher Student就需要自定义了需要仿函数和函数对象
{
	set<int> set1;
	set<int, less<int>> set2;//相当于set1
	set<int, greater<int>> set3;//从最大开始排序
	for (int i = 0; i < 5; i++)
	{
		int tmp = rand();//随机的插入
		set3.insert(tmp);
	}
	set1.insert(100);//只插入一个 一共6个元素！！
	set1.insert(100);
	set1.insert(100);

	for (set<int>::iterator it = set3.begin(); it != set3.end(); it++)
	{
		cout << *it << " ";
	}

}

class Student
{
public:
	Student(char * name, int age)
	{
		strcpy(this->name, name);
		this->age = age;
	}
public:
	char name[64];
	int age;
};

//仿函数
struct FuncStudent
{
	bool operator()(const Student &left, const Student &right)
	{
		if (left.age < right.age)//左边小 返回真 即从小到大排序年龄
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};
void main93()
{
	
	Student s1("s1", 31);
	Student s2("s2", 22);
	Student s3("s3", 44);
	Student s4("s4", 11);
	Student s5("s5", 31);

	set<Student, FuncStudent> set1;//仿函数用法 写一个类重载
	set1.insert(s1);
	set1.insert(s2);
	set1.insert(s3);
	set1.insert(s4);
	set1.insert(s5);//两个31岁 插入不成功！！！


	//遍历
	for (set<Student, FuncStudent>::iterator it = set1.begin(); it != set1.end(); it++)
	{
		cout << it->age << "\t" << it->name << endl;
	}

}
//typedef pair<iterator,bool> _Pairib
//如何判断set1.insert函数返回值
//pair的用法
void main94()//stl的入门！！！！！！！！！！！！！！
{

	Student s1("s1", 31);
	Student s2("s2", 22);
	Student s3("s3", 44);
	Student s4("s4", 11);
	Student s5("s5", 31);

	set<Student, FuncStudent> set1;//仿函数用法 写一个类重载
	pair<set<Student, FuncStudent>::iterator, bool> pair1 = set1.insert(s1);//pair<iterator, bool>是一个数据类型 iterator也是数据类型
	//iterator是迭代器的位置 bool迭代器的结果
	if (pair1.second == true)
	{
		cout << "插入s1成功" << endl;
	}
	else
	{
		cout << "插入失败" << endl;
	}
	pair<set<Student, FuncStudent>::iterator, bool> pair5=set1.insert(s5);//两个31岁 插入不成功！！！
	if (pair5.second == true)
	{
		cout << "插入s5成功" << endl;
	}
	else
	{
		cout << "插入失败" << endl;
	}
}

void main95()//查找
{
	set<int> set1;
	
	for (int i = 0; i < 10; i++)
	{
		
		set1.insert(i+1);
	}
	
	for (set<int>::iterator it = set1.begin(); it != set1.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;

	set<int>::iterator it0=set1.find(5);//
	cout << "it0:" << *it0 << endl;
	
	int num1 = set1.count(5);//
	cout << "num1:" << num1 << endl;

	set<int>::iterator it1= set1.lower_bound(5);//大于等于5的元素的迭代器的位置
	cout << "it1:" << *it1 << endl;
	
	set<int>::iterator it2 = set1.lower_bound(5);//大于5的
	cout << "it2:" << *it2 << endl;

	//typedef pair<iterator, bool> _Pairib;
	//typedef pair<iterator, iterator> _Pairii;
	//typedef pair<const_iterator, const_iterator> _Paircc;
	pair<set<int>::iterator, set<int>::iterator> mypair = set1.equal_range(5);//返回两个迭代器 一个大于5的 一个大于等于5
	set<int>::iterator it3=mypair.first;
	cout << "it3:" << *it3 << endl;//大于等于5的元素的迭代器的位置 是5
	
	set<int>::iterator it4 = mypair.second;
	cout << "it4:" << *it4 << endl;//大于5的元素的迭代器的位置 是6

	set1.erase(5);//把5这个元素删掉
}

void main()
{
	//main91();
	//main93();
	main95();
	system("pause");
	return;
}


//错误	1	error C4996: 'strcpy': This function or variable may be unsafe. Consider using strcpy_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.	h:\c++\1.stl\1.stl\9set容器.cpp	59	1	1.stl
