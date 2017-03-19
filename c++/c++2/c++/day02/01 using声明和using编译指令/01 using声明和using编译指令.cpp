#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

namespace A{
	int paramA = 10;
	int paramB = 20;
	void func(){
		cout << "hello world!" << endl;
	}
}

//using声明
void test01(){

	A::paramA;
	A::func();

	//2. 偷懒 不写域作用符
	using A::func;
	//注意在同一作用域内发生命名冲突
	using A::paramA;
	func();
	//3. 命名冲突
	//int paramA = 20;
}

namespace B{
	int paramA = 10;
	int paramC = 20;
	void func1(){
		cout << "hello world!" << endl;
	}
}


//using编译指令
void test02(){

	using namespace A;
	using namespace B;
	
	//func();

	//如果使用using编译指令，那么命名冲突会隐藏命名空间A中的变量
	//int paramA = 20;
	//cout << paramA << endl;

}

namespace VeryLongName{
	int param = 10;
}

//给命名空间取个别名  namespace 别名 = 命名空间名称
void test03(){

	namespace shorName = VeryLongName;
	cout << shorName::param << endl;
	cout << VeryLongName::param << endl;
}
int main(){

	//test02();
	test03();

	system("pause");
	return EXIT_SUCCESS;
}