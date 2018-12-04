#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;


//参数：分为有参构造函数和无参构造函数
    //有参构造函数：拷贝构造函数 和  普通构造函数
class Animal{
public:
	Animal(){
		cout << "无参的构造函数！" << endl;
		mName = "undefined";
		mAge = 0;
	}
	Animal(int age){
		cout << "一个参数构造函数!" << endl;
		mName = "undefined";
		mAge = age;
	}
	Animal(string name){
		cout << "一个参数构造函数!" << endl;
		mName = name;
		mAge = 0;
	}
	Animal(string name,int age){
		cout << "两个参数构造函数!" << endl;
		mName = name;
		mAge = age;
	}
	//拷贝(复制)构造函数  用一个对象来初始化另一个对象
	Animal(const Animal& animal){
		cout << "拷贝构造函数!" << endl;
		mName = animal.mName;
		mAge = animal.mAge;
	}
	//打印
	void PrintAnimal(){
		cout << "Name:" << mName << " Age:" << mAge << endl;
	}
	~Animal(){
		cout << "析构函数！" << endl;
	}
private:
	string mName;
	int mAge;
};

//无参构造函数调用
void test01(){

	//1. 调用无参构造函数
	Animal animal;
	animal.PrintAnimal();

	//2. 错误调用 编译器当做函数声明
	//Animal animal2();
	//animal2.PrintAnimal();
}

//调用有参构造函数
void test02(){

#if 0
	//1. 括号法
	Animal animal1("John",30);
	animal1.PrintAnimal();
	Animal animal2("John");
	Animal animal3(animal1);
	animal3.PrintAnimal();

	//2. 显示调用构造函数
	Animal("Smith",30); //匿名函数生命周期仅限于当前行
#endif
	Animal animal4 = Animal(30);
	animal4.PrintAnimal();

	//Animal animal5(Animal("Smith", 30));
	//Animal animal5(Animal(30)); //  Animal animal5 = Animal(30)
	//Animal animal6(Animal(animal4)); // Animal animal6 = Animal(animal4)

	//Animal(animal4); // Animal animal4;
	                  //"匿名对象"如果有变量来接，那么它就是匿名对象
					  // 如果有变量来接，那么就是一个实例化对象

	//3. 等号法 只能针对一个参数的构造函数来使用
	//Animal animal7 = 10; // Animal animal7 = Animal(10);

	Animal animal8 = animal4; // Animal animal7 = Animal(animal4);
}


int main(){

	//test01();
	test02();

	system("pause");
	return EXIT_SUCCESS;
}