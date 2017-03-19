#include <iostream>
using namespace std;
#include "string"
#include<vector>
#include<list>
#include"set"
#include<algorithm>
#include"functional"
#include "iterator"
#include<numeric>

void printV(vector<int> &v)
{
	for (vector<int>::iterator it = v.begin(); it != v.end(); it++)
	{
		cout << *it << " ";
	}
}

void printList(list<int> &v)
{
	for (list<int>::iterator it = v.begin(); it != v.end(); it++)
	{
		cout << *it << " ";
	}
}

void showElem(int &n)
{
	cout << n << " ";
}

class CMyShow
{
public:
	CMyShow()
	{
		num = 0;
	}
	void operator()(int &n)
	{
		num++;
		cout << n << " ";
	}
	void printNum()
	{
		cout << "num:" << num << endl;
	}
private:
	int num;

};


void main41_foreach_transform()
{
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);

	printV(v1);
	cout << endl;
	//for_each的定义
	/*template<class _InIt,
	class _Fn1> inline
	_Fn1 for_each(_InIt _First, _InIt _Last, _Fn1 _Func)//返回的是一个函数对象 参数是容器 容器 函数对象
	{	// perform function for each element            //返回函数对象的精髓是：能返回记录状态
	_DEBUG_RANGE(_First, _Last);                        //返回的是一个函数对象 这个对象的去或留看怎么接 用my1接相当给my1初始化（小三转正）
	_DEBUG_POINTER(_Func);
	_For_each(_Unchecked(_First), _Unchecked(_Last), _Func);

	return (_STD move(_Func));                           
	}
*/

	//函数对象 函数入口地址（即回调函数）
	for_each(v1.begin(), v1.end(), showElem);
	cout << endl;
	//函数对象
	for_each(v1.begin(), v1.end(), CMyShow());
	cout << endl;

	CMyShow mya;
	CMyShow my1 = for_each(v1.begin(), v1.end(), mya);
	mya.printNum();//是0
	my1.printNum();//是3

	my1 = for_each(v1.begin(), v1.end(), mya);//给my1赋值 不是初始化
	my1.printNum();//是3

}

int increase(int i)
{
	return i + 100;

}

void main42_transform()
{
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);

	printV(v1);
	cout << endl;
	//使用函数对象increase（回调函数）
	transform(v1.begin(), v1.end(), v1.begin(), increase);
	printV(v1);
	cout << endl;

	//使用预定义的函数对象
	transform(v1.begin(), v1.end(), v1.begin(),negate<int>());
	printV(v1);
	cout << endl;
	//使用函数适配器和函数对象
	list<int> mylist;
	mylist.resize(v1.size());
	transform(v1.begin(), v1.end(),mylist.begin(),bind2nd(multiplies<int>(),10));//数据存放在list中
	printList(mylist);
	cout << endl;
	//transform也可以把运算结果直接输出到屏幕
	transform(v1.begin(), v1.end(), ostream_iterator<int>(cout, " "), negate<int>());
	cout << endl;

}

int showElem2(int n)
{
	cout << n << " ";
	return n;
}

void main_foreach_pk_transform()
{
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);

	vector<int> v2=v1;

	for_each(v1.begin(), v1.end(), showElem);
	transform(v2.begin(), v2.end(), v2.begin(), showElem2);//transform对回调函数（函数对象）的要求：有返回值 参数得是直接传递
	/*template<class _InIt,
	class _OutIt,
	class _Fn1> inline
	_OutIt transform(_InIt _First, _InIt _Last,
		_OutIt _Dest, _Fn1 _Func)
	{	// transform [_First, _Last) with _Func
	_DEBUG_RANGE(_First, _Last);
	_DEBUG_POINTER(_Dest);
	_DEBUG_POINTER(_Func);
	if (_First != _Last)
		return (_Transform1(_Unchecked(_First), _Unchecked(_Last),
			_Dest, _Func, _Is_checked(_Dest)));
	return (_Dest);
	}*/


}

void main44_adjacent_find()
{
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(2);
	v1.push_back(2);
	v1.push_back(4);
	v1.push_back(5);
	vector<int> ::iterator it=adjacent_find(v1.begin(), v1.end());
	if (it == v1.end())
	{
		cout << "没有找到重复元素" << endl;
	}
	else
	{
		cout << *it << endl;
	}
	int index = distance(v1.begin(), it);//输出下表位置
	cout << index << endl;
}

void main45_binary_search()
{
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);
	v1.push_back(7);
	v1.push_back(9);
	
	bool b=binary_search(v1.begin(), v1.end(), 7);
	if (b==true)
	{
		cout << "找到" << endl;
	}
	else
	{
		cout << "找不到" << endl;
	}

}

void main45_count()
{
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);
	v1.push_back(7);
	v1.push_back(9);
	
	bool b =count(v1.begin(), v1.end(), 7);
	
		cout << b << endl;
}

bool GreatThree(int iNum)
{
	if (iNum>3)
	{
		return true;
	}
	return false;
}

void main46_count_if()
{
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);
	v1.push_back(7);
	v1.push_back(9);

	int num = count_if(v1.begin(), v1.end(), GreatThree);//函数（谓词）入口地址
	cout << num << endl;

}

void main47_find()
{
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);
	v1.push_back(7);
	v1.push_back(9);

	vector<int>::iterator it=find(v1.begin(), v1.end(), 5);
	cout << *it << endl;
}
void main47_find_if()
{
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);
	v1.push_back(7);
	v1.push_back(9);

	
	vector<int>::iterator it2 = find_if(v1.begin(), v1.end(), GreatThree);//第一个大于3的位置的元素 是5
	cout << *it2 << endl;

}
void main_merge()
{
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);

	vector<int> v2;
	v2.push_back(2);
	v2.push_back(4);
	v2.push_back(6);

	vector<int> v3;
	v3.resize(v1.size() + v2.size());

	merge(v1.begin(), v1.end(), v2.begin(), v2.end(), v3.begin());
	printV(v3);

}

class Student
{
public:
	Student(string name, int id)
	{
		m_name = name;
		m_id = id;
	}
	void printT()
	{
		cout << "name:" << m_name << "id:" << m_id << endl;
	}
public:
	string m_name;
	int m_id;
};

bool CompareS(Student &s1, Student &s2)
{
	return (s1.m_id < s2.m_id);
	
}




void main_sort()
{
	Student s1("老大", 1);
	Student s2("老二", 2);
	Student s3("老三", 3);
	Student s4("老四", 4);
	vector<Student> v1;
	v1.push_back(s4);
	v1.push_back(s2);
	v1.push_back(s1);
	v1.push_back(s3);
	for (vector<Student>::iterator it = v1.begin(); it != v1.end(); it++)
	{
		it->printT();
	}
	//sort 根据自定义函数对象 进行自定义数据类型排序
	//用函数对象 实现了算法和数据类型的分离 
	sort(v1.begin(), v1.end(), CompareS);
	for (vector<Student>::iterator it = v1.begin(); it != v1.end(); it++)
	{
		it->printT();
	}
}

void main_ranndom_shuffle()
{
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);

	random_shuffle(v1.begin(), v1.end());
		printV(v1);//随机排序显示

}

void main_reverse()
{
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);
	reverse(v1.begin(), v1.end());
	printV(v1);

}

void main52_copy()
{
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);

	vector<int> v2;
	v2.resize(v1.size());
	copy(v1.begin(), v1.end(),v2.begin());
	printV(v2);
}

void main53_replace()
{
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);

	replace(v1.begin(), v1.end(), 1, 5);//所有1变5
	printV(v1);
}

bool great_equal_5(int &n)
{
	if (n >= 5)
	{
		return true;
	}
	return false;
}

void main53_replaceif()
{
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);
	v1.push_back(1);
	v1.push_back(1);
	v1.push_back(1);

	replace(v1.begin(), v1.end(), 1, 9);//所有1变9

	replace_if(v1.begin(), v1.end(), great_equal_5, 5);//所有大于等于5的变成5
	printV(v1);
}

void main54_swap()
{
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);

	vector<int> v2;
	v2.push_back(1);
	v2.push_back(1);
	v2.push_back(1);


	swap(v1, v2);
	printV(v1);
}

void main55_accumulate()
{
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);
	int tmp = accumulate(v1.begin(), v1.end(), 100);
	cout << tmp << endl;
}

void main56_fill()
{
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);
	fill(v1.begin(), v1.end(), 8);
	printV(v1);

}
void main57_union()
{
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);

	vector<int> v2;
	v2.push_back(1);
	v2.push_back(3);
	v2.push_back(5);
	
	vector<int> v3;
	v3.resize(v1.size() + v2.size());

	set_union(v1.begin(), v1.end(), v2.begin(), v2.end(), v3.begin());
	printV(v3);
}


void main()
{
	main41_foreach_transform();
	//main42_transform();
	//main_foreach_pk_transform();//一般 for_each 没有返回值参数为引用 transform得有返回值 有直接的参数
	                            //for_each快 但不灵活 transform慢但灵活
	//main44_adjacent_find();
	//main45_binary_search();
	//main45_count();
	//main46_count_if();
	//main47_find();
	//main47_find_if();
	//main_merge();
	//main_sort();
	//main_ranndom_shuffle();
	//main_reverse();
	//main52_copy();
	//main53_replace();
	//main53_replaceif();
	//main54_swap();
	//main55_accumulate();
	//main56_fill();
	//main57_union();
	system("pause");
	return;
}