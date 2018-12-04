#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;

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
	char* name;
	int mAge;
};

//1. 对象以值传递的方式传给函数参数
void DoBussiness(Animal animal){}
void test01(){
	Animal animal; //无参构造函数
	DoBussiness(animal);
}
//2. 用一个对象初始化另一个对象
void test02(){
	Animal animal;
	Animal animal2 = animal;
	Animal animal3(animal);
}
//3. 函数返回局部对象
Animal MyBussiness(){
	//局部对象
	Animal animal;
	cout << &animal << endl;
	return animal;
}

void test03(){
	Animal animal = MyBussiness();
	cout << &animal << endl;
}


int main(){

	//test01();
	//test02();
	test03();


	system("pause");
	return EXIT_SUCCESS;
}