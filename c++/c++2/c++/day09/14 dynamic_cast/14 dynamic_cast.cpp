#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

//dynamic_cast转换具有父子关系的指针或者引用
//只能将子类指针转换成父类指针，不能将父类指针转换成子类指针

class Animal{};
class Dog : public Animal{};
void test01(){
	
	Animal* animal = NULL;
	Dog* dog = NULL;

	//父类转成子类，从小转到大
	//Dog* d = dynamic_cast<Dog*>(animal);

	//子类转成父类，从大到小
	Animal* a = dynamic_cast<Animal*>(dog);

	//int a = 10;
	//char b = dynamic_cast<char>(a);

}

int main(){



	system("pause");
	return EXIT_SUCCESS;
}