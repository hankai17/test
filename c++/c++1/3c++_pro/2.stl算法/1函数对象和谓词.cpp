#include <iostream>
using namespace std;
#include "string"
#include<vector>
#include<list>
#include"set"
#include<algorithm>
#include"functional"

//函数对象 类重载了（）
template<typename T>//template泛型编程 typename
class ShowElemt
{
public:
	void operator()(T &t)
	{
		n++;
		cout << t << " ";
	}
	ShowElemt()
	{
		n = 0;
	}
	void printN()
	{
		cout << "n:" << n << endl;
	}
protected:
private:
	int n;
};
//函数对象 定义 函数对象和普通函数区别
template<typename T>
void FuncShowElemt(T &t)//函数模板
{
	cout << t << endl;
}

void FuncShowElemt2(int & t)//普通函数
{
	cout << t << " ";
}



void main01()
{
	int a = 10;
	ShowElemt<int> showElemt;
	showElemt(a);//函数对象（）的执行 很像一个函数//仿函数

	FuncShowElemt<int>(a);
	FuncShowElemt2(a);
}
//函数对象是属于类对象 能突破函数概念 能保持调用状态信息
//函数对象的好处
void main02()
{
	vector<int> v1;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);

	for_each(v1.begin(), v1.end(), ShowElemt<int>());//匿名函数对象 匿名仿函数
	for_each(v1.begin(), v1.end(), FuncShowElemt2);//通过回调函数 谁使用for_each谁去写回调函数的入口地址

	ShowElemt<int> show1;
	//函数对象 做函数参数
	/*template<class _InIt,
	class _Fn1> inline
	_Fn1 for_each(_InIt _First, _InIt _Last, _Fn1 _Func)
	{	// perform function for each element
	_DEBUG_RANGE(_First, _Last);
	_DEBUG_POINTER(_Func);
	_For_each(_Unchecked(_First), _Unchecked(_Last), _Func);

	return (_STD move(_Func));
	}*/
	//for_each算法的函数对象传递 _Fn1 _Func是元素传递不是引用传递
	for_each(v1.begin(), v1.end(), show1);
	show1.printN();

	cout << "通过for_each算法的返回值看调用的次数" << endl;
	show1 = for_each(v1.begin(), v1.end(), show1);
	show1.printN();
	//for_each返回的是函数对象 find返回的迭代器
}
//分清算法的返回值是迭代器还是谓词（函数对象）是算法的入门

template<typename T>//泛型编程 看到T不要报错
class IsDiv
{
public:
	IsDiv(const T&divisor)
	{
		this->divisor = divisor;
	}
	//谓词
	bool operator()(T& t)
	{
		return(t%divisor == 0);
	}
private:
	T divisor;
};

void main03()
{
	vector<int> v2;
	for (int i = 10; i < 33; i++)
	{
		v2.push_back(i);
	}
	int a = 4;
	IsDiv<int>myDiv(a);
	/*template<class _InIt,
	class _Pr> inline
	_InIt find_if(_InIt _First, _InIt _Last, _Pr _Pred)迭代器 迭代器 谓词 返回的是迭代器
	{	// find first satisfying _Pred
	_DEBUG_RANGE(_First, _Last);
	_DEBUG_POINTER(_Pred);
	return (_Rechecked(_First,
		_Find_if(_Unchecked(_First), _Unchecked(_Last), _Pred)));
	}*/


	//find_if(v2.begin(), v2.end(), myDiv);
	
	vector<int>::iterator it;
	it=find_if(v2.begin(), v2.end(), IsDiv<int>(a));
	if (it == v2.end())
	{
		cout << "容器中没被4整除的元素" << endl;
	}
	else
	{
		cout << "第一个被4整除的元素是：" << *it << endl;
	}

}


//二元函数对象
template <typename T>
class SunAdd
{
public:
	T operator()(T t1, T t2)
	{
		return t1 + t2;
	}
private:

};

void main04()
{
	vector<int> v1, v2;
	vector<int> v3;
	v1.push_back(1);
	v1.push_back(3);
	v1.push_back(5);

	v2.push_back(2);
	v2.push_back(4);
	v2.push_back(6);

	v3.resize(10);
	transform(v1.begin(), v1.end(), v2.begin(), v3.begin(), SunAdd<int>());

	/*template<class _InIt1,
	class _InIt2,
	class _OutIt,
	class _Fn2> inline
	_OutIt transform(_InIt1 _First1, _InIt1 _Last1,
	_InIt2 _First2, _OutIt _Dest, _Fn2 _Func)
	{	// transform [_First1, _Last1) and [_First2, ...) with _Func
	_DEBUG_RANGE(_First1, _Last1);
	_DEBUG_POINTER(_Dest);
	_DEBUG_POINTER(_Func);
	if (_First1 != _Last1)
	return (_Transform2(_Unchecked(_First1), _Unchecked(_Last1),
	_First2, _Dest, _Func,
	_Is_checked(_Dest)));
	return (_Dest);
	}*/  //由此可见 transform把运算结果的迭代器的开始位置返回出来

	for (vector<int>::iterator it = v3.begin(); it != v3.end(); it++)
	{
		cout << *it << endl;
	}
}


bool MyCompare(const int &a, const int &b)
{
	return a < b;//从小到大为真
}



//二元谓词
void main05()
{
	vector<int> v1(10);

	for (int i = 0; i < 10; i++)
	{
		int tmp = rand() % 100;
		v1[i] = tmp;

	}
	for (vector<int>::iterator it = v1.begin(); it != v1.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;
	for_each(v1.begin(), v1.end(), FuncShowElemt2);//也是遍历 第三个参数是函数对象
	cout << endl;
	//排序
	sort(v1.begin(), v1.end(), MyCompare);
	for_each(v1.begin(), v1.end(), FuncShowElemt2);//也是遍历 第三个参数是函数对象
	cout << endl;





}

struct CompareNoCase
{
	bool operator()(const string &str1, const string &str2)
	{
		string str1_;
		str1_.resize(str1.size());
		transform(str1.begin(), str1.end(), str1_.begin(), tolower);//str1变成小写放在——里面

		string str2_;
		str2_.resize(str1.size());
		transform(str2.begin(), str2.end(), str2_.begin(), tolower);
		return(str1 < str2);//从小到大排序
	}
};



//二元谓词应用
void main06()
{
	set<string> set1;
	set1.insert("bbb");
	set1.insert("aaa");
	set1.insert("ccc");
	set<string>::iterator it = set1.find("aaa");//find函数区分大小写 若写出Aaa就找不到了
	if (it == set1.end())
	{
		cout << "没有查找到 aaa" << endl;
	}
	else
	{
		cout << "找到了 aaa" << endl;
	}

	set<string, CompareNoCase>set2;
	set2.insert("bbb");
	set2.insert("aaa");
	set2.insert("ccc");
	set<string, CompareNoCase>::iterator it2 = set2.find("aAa");
	if (it == set2.end())
	{
		cout << "没有查找到 aaa" << endl;
	}
	else
	{
		cout << "不区分大小写的找到了 aaa" << endl;
	}


}



void main()
{
	main06();
	system("pause");
	return;
}
//在类中重载了函数调用运算符 用这个类定义对象就是 函数对象
