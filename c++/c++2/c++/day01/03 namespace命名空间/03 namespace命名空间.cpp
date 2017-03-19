#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

namespace A{
	int a = 10;
	void func(){}
	class className{};
	struct structName{};
	className cn;
}
namespace B{
	int a = 20;
}

//1.命名空间只能在全局范围内定义
void test(){
	//namespace A{}
}

//2. 命名空间可以嵌套命名空间
namespace Test2A{
	int a = 10;
	namespace Test2AA{
		int b = 10;
	}
}
void test02(){
	cout << Test2A::a << endl;
	cout << Test2A::Test2AA::b << endl;
}

//3. 命名空间是开放
namespace Test3A{
	int a = 10;
};
namespace Test3A{
	void func(){
		cout << "hello world!" << endl;
	}
}

void test03(){
	cout << Test3A::a << endl;
	Test3A::func();
}

//4. namespace分文件编写
//5. 匿名命名空间/无名名空间 ,默认无名命名空间中函数、变量等等只在当前文件内有效,就相当于给每个变量或者函数加上static
namespace{
	int c = 10;
	int d = 20;
}
void test05(){
	cout << c << endl;
	cout << d << endl;
}


int main(){

	//test02();
	//test03();
	test05();

	system("pause");
	return EXIT_SUCCESS;
}