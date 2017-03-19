#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

//1.基础数据类型
void test01(){
	int a = 10;
	char b = static_cast<char>(a);
}

//父子关系指针或者引用转换
class Animal{};
class Dog : public Animal{};
class Other{};

void test02(){

	Animal* animal = NULL;
	Dog* dog = NULL;

	//从小转到大 下行转换
	Dog* d = static_cast<Dog*>(animal);
	Animal* a = static_cast<Animal*>(dog);

	//转换成无关类型的指针
	//Other* o = static_cast<Other*>(animal);
}


int main(){



	system("pause");
	return EXIT_SUCCESS;
}