#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
#include<algorithm>
#include<functional>
using namespace std;

//1.一元谓词


struct GreaterThan20{
	bool operator()(int val){
		return val >= 20;
	}
};


void test01(){

	vector<int> v;
	v.push_back(20);
	v.push_back(10);
	v.push_back(30);
	v.push_back(40);

	vector<int>::iterator it = find_if(v.begin(), v.end(), GreaterThan20());
	if (it == v.end()){
		cout << "没找到!" << endl;
	}
	else{
		cout << "找到:" << *it << endl;
	}

}

//二元谓词
struct MyComparre{
	bool operator()(int v1,int v2){
		return v1 > v2;
	}
};
void test02(){
	

	vector<int> v;
	v.push_back(20);
	v.push_back(10);
	v.push_back(30);
	v.push_back(40);

	for_each(v.begin(), v.end(), [](int val)->void{cout << val << " "; });
	sort(v.begin(), v.end(),  less<int>() );
	//兰巴达表达式
	for_each(v.begin(), v.end(), [](int val)->void{cout << val << " "; });
	cout << endl;

}

//3.内建函数对象
/*
template<class T> T plus<T>//加法仿函数
template<class T> T minus<T>//减法仿函数
template<class T> T multiplies<T>//乘法仿函数
template<class T> T divides<T>//除法仿函数
template<class T> T modulus<T>//取模仿函数
template<class T> T negate<T>//取反仿函数
*/
void test03(){

	plus<int> plus;
	cout << plus(10, 20) << endl;

	cout << minus<int>()(30, 10) << endl;
}

int main(){

	test01();
	//test02();
	//test03();

	system("pause");
	return EXIT_SUCCESS;
}