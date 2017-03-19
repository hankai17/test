#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<algorithm>
#include<vector>
using namespace std;

//1. for_each算法
/*
template<class _InIt,class _Fn1>
inline _Fn1 for_each(_InIt _First, _InIt _Last, _Fn1 _Func)
{
for (; _First != _Last; ++_First)
_Func(*_First);
}
*/
struct MyPrint01{
	MyPrint01(){
		mCallNums = 0;
	}
	void operator()(int v){
		mCallNums++;
		cout << v << " ";
	}
public:
	int mCallNums;
};

struct MyPrint02{
	void operator()(const int& v){
		//v += 100;
		cout << v << " ";
	}
};
void test01(){

	vector<int> v;
	for (int i = 0; i < 10;i++){
		v.push_back(i + 1);
	}

	MyPrint01 print;
	MyPrint01 m = for_each(v.begin(), v.end(), print);

	cout << "调用次数:" << print.mCallNums << endl;
	cout << "调用次数:" << m.mCallNums << endl;

	for_each(v.begin(), v.end(), MyPrint02());

	for_each(v.begin(), v.end(), MyPrint02());
}

//2. transform 搬运
/*
template<class _InIt, class _OutIt, class _Fn1> inline
_OutIt _Transform(_InIt _First, _InIt _Last,_OutIt _Dest, _Fn1 _Func)
{

for (; _First != _Last; ++_First, ++_Dest)
*_Dest = _Func(*_First);
return (_Dest);
}

template<class _InIt1,class _InIt2,class _OutIt,class _Fn2> inline
_OutIt _Transform(_InIt1 _First1, _InIt1 _Last1,_InIt2 _First2, _OutIt _Dest, _Fn2 _Func)
{
for (; _First1 != _Last1; ++_First1, ++_First2, ++_Dest)
*_Dest = _Func(*_First1, *_First2);
return (_Dest);
}
*/

struct Transform1{
	int operator()(int v){
		return v + 100;
	}
};
void test02(){
	
	vector<int> v;
	for (int i = 0; i < 10; i++){
		v.push_back(i + 1);
	}

	vector<int> vTarget; //目标容器
	//vTarget.reserve(10); 不可行
	vTarget.resize(v.size());
	transform(v.begin(), v.end(), vTarget.begin(), Transform1());
	for_each(vTarget.begin(), vTarget.end(), [](int val){cout << val << " "; });
}

struct Transform2{
	int operator()(int v1,int v2){
		return v1 + v2;
	}
};

void test03(){

	vector<int> v1;
	vector<int> v2;
	for (int i = 0; i < 10; i++){
		v1.push_back(i + 100);
		v2.push_back(i + 200);
	}

	vector<int> vTarget; //目标容器
	vTarget.resize(v1.size());

	transform(v1.begin(), v1.end(), v2.begin(), vTarget.begin(), Transform2());
	for_each(vTarget.begin(), vTarget.end(), [](int val){cout << val << " "; });
}

int main(){

	//test01();
	//test02();
	test03();


	system("pause");
	return EXIT_SUCCESS;
}