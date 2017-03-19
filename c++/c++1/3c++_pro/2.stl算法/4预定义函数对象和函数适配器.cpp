#include <iostream>
using namespace std;
#include "string"
#include<vector>
#include<list>
#include"set"
#include<algorithm>
#include"functional"

//plus<int>预定义好的函数对象 能实现不同类型的+运算
//实现了 数据类型和函数算法的分离 是通过函数对象技术实现的
//怎么知道 plus<type>是两个参数
void main21()
{
	plus<int>intAdd;
	/*template<class _Ty = void>
	struct plus
		: public binary_function<_Ty, _Ty, _Ty>
	{	// functor for operator+
	_Ty operator()(const _Ty& _Left, const _Ty& _Right) const
		{	// apply operator+ to operands
		return (_Left + _Right);
		}
	};*/
	int x = 10;
	int y = 20;
	int z=intAdd(x, y);//+
	cout << "z:" << z << endl;

	//还有stringAdd(s1,s2) ,string s1="aaa"

	vector<string> v1;
	v1.push_back("bbb");
	v1.push_back("aaa");
	v1.push_back("ccc");
	v1.push_back("ccc");
	v1.push_back("ccc");
	v1.push_back("zzz");
	/*template<class _Ty = void>
	struct greater
	: public binary_function<_Ty, _Ty, bool>
	{	// functor for operator>
	bool operator()(const _Ty& _Left, const _Ty& _Right) const
	{	// apply operator> to operands
	return (_Left > _Right);                         从大到小
	}
	};

	// TEMPLATE STRUCT less
	// defined in <xstddef>
	*/
	sort(v1.begin(), v1.end(), greater<string>());//参数是 迭代器位置 迭代器位置 谓词
	for (vector<string>::iterator it = v1.begin(); it != v1.end(); it++)
	{
		cout << *it << endl;// 从大到小
	}

	//求ccc出现个数
	string sc = "ccc";
	//equal_to有两个参数 左参数来自容器 右参数来自sc  
	//通过bind2nd函数适配器 把预定义对象和第二个参数进行绑定
	int num=count_if(v1.begin(), v1.end(),bind2nd (equal_to<string>(),sc));
	cout << "num:" << num << endl;


}

class IsGreat
{
public:
	IsGreat(int i)
	{
		m_num = i;
	}
	bool operator()(int &num)
	{
		if (num>m_num)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
private:
	int m_num;
};


void main22()
{
	vector<int> v1;
	for (int i = 0; i < 10; i++)
	{
		v1.push_back(i+1);
	}
	for (vector<int>::iterator it = v1.begin(); it != v1.end(); it++)
	{
		cout << *it << " ";
	}
	int num1 = count(v1.begin(), v1.end(), 3);
	cout << "num1:" << num1 << endl;
	//通过谓词求大于2的个数
	int num2 = count_if(v1.begin(), v1.end(), IsGreat(2));
	cout << "num2:" << num2<<endl;

	//通过预定义的函数对象求大于的个数  预定义+绑定 能实现分层
	//看greater函数
	/*template<class _Ty = void>
	struct greater
		: public binary_function<_Ty, _Ty, bool>
	{	// functor for operator>
	bool operator()(const _Ty& _Left, const _Ty& _Right) const
		{	// apply operator> to operands
		return (_Left > _Right);
		}
	};*/
	int num3 = count_if(v1.begin(), v1.end(), bind2nd(greater<int>(),2));//greater函数左参数来自容器中每一个数 右参数绑定成2
	cout << "num3:" << num3 << endl;

	//求奇数的个数
	int num4 = count_if(v1.begin(), v1.end(), bind2nd(modulus<int>(), 2));
	cout << "num4:" << num4 << endl;
	//求偶数的个数  取反是一数字一
	int num5 = count_if(v1.begin(), v1.end(),not1(bind2nd(modulus<int>(), 2)));
	cout << "num5:" << num5 << endl;

}


void main()
{
	//main21();
	main22();//函数适配器综合案例
	system("pause");
	return;

}