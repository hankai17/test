#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<functional>
using namespace std;


//1. 让自己编写的函数对象继承基类,如果是一元函数对象需要继承unary_function,
//如果是二元函数对象继承binary_function

//2. 函数对象operator()函数后增加 const
//3. 使用bind2nd bind1st

//bind2nd其实就将一个二元的函数对象转变为了一元函数对象

struct MyPrint : public binary_function<int,int,void> {
	void operator()(int v,int v2) const{
		cout << "v:" << v << " v2:" << v2 << endl;
		cout << v + v2 << " ";
	}
};

//如何让for_each算法调用二元函数对象，把二元函数对象转变为一元函数对象

void test01(){
	
	vector<int> v;
	for (int i = 0; i < 10;i ++){
		v.push_back(i + 1);
	}
	
	for_each(v.begin(), v.end(), bind1st(MyPrint(),300)); 
	//bind2nd把300绑定为函数对象的第二个参数
	//bind1st把300绑定为函数对象的第一个参数
	cout << endl;
}

//2. 取反适配器 not1 not2
struct GreaterThan5 : public unary_function<int,bool>{
	bool operator()(int v) const{
		return v > 5;
	}
};
void test02(){

	vector<int> v;
	for (int i = 0; i < 10;i++){
		v.push_back(i + 1);
	}

	//vector<int>::iterator it = find_if(v.begin(), v.end(), not1(GreaterThan5()));
	vector<int>::iterator it = find_if(v.begin(), v.end(), not1( bind2nd(greater<int>(),5)) );
	if (it == v.end()){
		cout << "么有找到!" << endl;
	}
	else{
		cout << "找到:" << *it << endl;
	}


	//排序
	sort(v.begin(),v.end(),not2(less<int>()));
	for_each(v.begin(), v.end(), [](int val){cout << val << " "; });


	//not1 对一元谓词取反
	//not2 对二元谓词取反
}

//3. ptr_fun 函数指针适配器
void MyPrint03(int val,int v2){
	cout << val + v2 << " ";
}
//把一个普通的函数指针适配成函数对象
void test03(){

	vector<int> v;
	for (int i = 0; i < 10; i++){
		v.push_back(i + 1);
	}

	for_each(v.begin(), v.end(), bind2nd( ptr_fun(MyPrint03), 100));
}

//4. 成员函数适配器 mem_fun和mem_fun_ref
class Person{
public:
	Person(string name,int age){
		this->mName = name;
		this->mAge = age;
	}

	//打印函数
	void ShowPerson(){
		cout << "成员函数:" << "Name:" << mName << " Age:" << mAge << endl;
	}

	void AgePlus100(){
		this->mAge += 100;
	}

public:
	string mName;
	int mAge;
};

void MyPrint04(Person& p){
	cout << "全局函数:" << "Name:" << p.mName << " Age:" << p.mAge << endl;
}

void test04(){
	

	vector<Person> v1;
	//创建数据
	Person p1("aaa", 10);
	Person p2("bbb", 20);
	Person p3("ccc", 30);
	Person p4("ddd", 40);

	v1.push_back(p1);
	v1.push_back(p2);
	v1.push_back(p3);
	v1.push_back(p4);

	for_each(v1.begin(), v1.end(), MyPrint04);

	cout << "-------------" << endl;

	//调用成员函数打印
	for_each(v1.begin(), v1.end(), mem_fun_ref(&Person::ShowPerson));
	for_each(v1.begin(), v1.end(), mem_fun_ref(&Person::AgePlus100));
	for_each(v1.begin(), v1.end(), mem_fun_ref(&Person::ShowPerson));
}

void test05(){

	vector<Person*> v1;
	//创建数据
	Person p1("aaa", 10);
	Person p2("bbb", 20);
	Person p3("ccc", 30);
	Person p4("ddd", 40);

	v1.push_back(&p1);
	v1.push_back(&p2);
	v1.push_back(&p3);
	v1.push_back(&p4);

	for_each(v1.begin(), v1.end(), mem_fun(&Person::ShowPerson));
}

//mem_fun 如果容器存放的是对象指针，那么用men_fun
//如果容器中存放的是对象实体，那么用mem_fun_ref


int main(){

	//test01();
	//test02();
	//test03();
	//test04();
	test05();

	system("pause");
	return EXIT_SUCCESS;
}