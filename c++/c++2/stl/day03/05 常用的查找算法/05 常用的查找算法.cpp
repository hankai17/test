#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<functional>
using namespace std;

//1. find find_if
//查找基础数据类型
/*
template<class _InIt,
class _Ty> inline
_InIt _Find(_InIt _First, _InIt _Last, const _Ty& _Val, false_type)
{	// find first matching _Val
	for (; _First != _Last; ++_First)
		if (*_First == _Val)
			break;
return (_First);
}
*/
void test01(){

	vector<int> v;
	for (int i = 0; i < 10;i ++){
		v.push_back(i + 1);
	}


	vector<int>::iterator it =  find(v.begin(),v.end(),5);
	if (it == v.end()){
		cout << "没有找到!" << endl;
	}
	else{
		cout << "找到:" << *it << endl;
	}
}

class Person{
public:
	Person(string name,int age){
		this->mName = name;
		this->mAge = age;
	}

	//重载==
	bool operator==(const Person& p){
		if (this->mName == p.mName && this->mAge == p.mAge){
			return true;
		}
		return false;
	}

	//重载<
	bool operator<(const Person& p) const{
		return this->mAge < p.mAge;
	}

public:
	string mName;
	int mAge;
};

void test02(){
	
	vector<Person> v;

	//创建数据
	Person p1("aaa", 10);
	Person p2("bbb", 20);
	Person p3("ccc", 30);
	Person p4("ddd", 40);

	v.push_back(p1);
	v.push_back(p2);
	v.push_back(p3);
	v.push_back(p4);

	vector<Person>::iterator it = find(v.begin(), v.end(), p2);
	if (it == v.end()){
		cout << "没有找到!" << endl;
	}
	else{
		cout << "找到:" << it->mName <<":" << it->mAge << endl;
	}
}

//查找对象指针

struct search_condition : public binary_function<Person*,Person*,bool>{
	bool operator()(Person* p,Person* p2) const{
		if (p->mName == p2->mName && p->mAge == p2->mAge){
			return true;
		}
		return false;
	}
};

void test03(){

	vector<Person*> v;

	//创建数据
	Person p1("aaa", 10);
	Person p2("bbb", 20);
	Person p3("ccc", 30);
	Person p4("ddd", 40);

	v.push_back(&p1);
	v.push_back(&p2);
	v.push_back(&p3);
	v.push_back(&p4);


	Person* p = new Person("bbb",20);
	
	vector<Person*>::iterator it = find_if(v.begin(), v.end(), bind2nd(search_condition(),p));
	if (it == v.end()){
		cout << "找不到!" << endl;
	}
	else{
		cout << "找到:" << (*it)->mName << ":" << (*it)->mAge << endl;
	}

}

//adjacent_find 查找相邻重复元素   1 2 3 3 4 5 6
void test04(){
	
	vector<int> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(5);
	v.push_back(3);
	v.push_back(4);
	v.push_back(4);

	vector<int>::iterator it = adjacent_find(v.begin(),v.end());
	if (it == v.end()){
		cout << "找不到!" << endl;
	}
	else{
		cout << "找到:" << *it << endl;
	}

}

//bool::binary_search 二分查找法 需要序列有序的
void test05(){
	
	vector<int> v;
	for (int i = 0; i < 10; i++){
		v.push_back(i + 1);
	}

	bool flag =  binary_search(v.begin(), v.end(), 5);
	if (flag){
		cout << "找到!" << endl;
	}
	else{
		cout << "没有找到!" << endl;
	}
}

//binary_search

void test06(){
	
	vector<Person> v;

	//创建数据
	Person p1("aaa", 10);
	Person p2("bbb", 20);
	Person p3("ccc", 30);
	Person p4("ddd", 40);

	v.push_back(p1);
	v.push_back(p2);
	v.push_back(p3);
	v.push_back(p4);

	if (binary_search(v.begin(), v.end(), p2)){
		cout << "摘到!" << endl;
	}
	else{
		cout << "摘不到!" << endl;
	}

}

//count count_if 统计元素的个数
struct GreaterTwo{
	bool operator()(int v){
		return v > 2;
	}
};
void test07(){
	

	vector<int> v;
	v.push_back(1);
	v.push_back(2);
	v.push_back(5);
	v.push_back(3);
	v.push_back(4);
	v.push_back(4);
	/*
		for (; _First != _Last; ++_First)
		if (*_First == _Val)
			++_Count;
	*/
	int num = count(v.begin(), v.end(),4);
	cout << "NUM:" << num << endl;
	num = count_if(v.begin(), v.end(), GreaterTwo());
	cout << "NUM:" << num << endl;
}

int main(){

	//test01();
	//test02();
	//test03();
	//test04();
	//test05();
	//test06();
	test07();

	system("pause");
	return EXIT_SUCCESS;
}