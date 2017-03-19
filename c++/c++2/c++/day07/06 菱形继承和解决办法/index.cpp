#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class Animal{
public:
	int mID;
	int mAge;
	int mSex;
};

//虚继承  虚基类
class Sheep : virtual public Animal{};
class Tuo : virtual public Animal{};
//羊驼
class SheepTuo : public Sheep, public Tuo{};


void test01(){

	SheepTuo st;
	//1. 多继承所产生第一个问题，二义性
	st.Sheep::mID;
	st.Tuo::mID;

	//2. 
	cout << "sheeptuo size:" << sizeof(SheepTuo) << endl;

}

void test02(){
	
	SheepTuo st;
	st.Sheep::mAge = 100;
	st.Tuo::mAge = 200;

	cout << st.Sheep::mAge << endl;
	cout << st.Tuo::mAge << endl;
}

void test03(){

	SheepTuo st;
	st.Sheep::mAge = 100;
	st.Tuo::mAge = 200;

	cout << st.Sheep::mAge << endl;
	cout << st.Tuo::mAge << endl;
}

int main(){

	test01();
	//test02();
	//test03();

	system("pause");
	return EXIT_SUCCESS;
}