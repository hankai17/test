#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;

class Car{
public:
	Car(string name){
		cout << "Car构造函数!" << endl;
		mName = name;
	}
	void run(){
		cout << "汽车启动..." << endl;
	}
	~Car(){
		cout << "Car析构函数!" << endl;
	}
public:
	string mName;
};

class Tuolaji{
public:
	Tuolaji(string name){
		cout << "Tuolaji构造函数!" << endl;
		mName = name;
	}
	void run(){
		cout << "拖拉机启动..." << endl;
	}
	~Tuolaji(){
		cout << "Tuolaji析构函数!" << endl;
	}
public:
	string mName;
};

class Person{
public:
	Person(string CarName, string TLJName, int a) : mTuolaji(TLJName), car(CarName), a(a){
		cout << "Person构造函数!" << endl;
	}
	void GoToWork(){
		car.run();
		cout << "开开心心" << car.mName << "去上班..." << endl;
	}
	~Person(){
		cout << "Person析构函数!" << endl;
	}
public:
	Car car;
	Tuolaji mTuolaji;
	int a;
};

void test(){
	Person person("别摸我","拖拉机",10);
	person.GoToWork();
}

int main(){

	test();

	system("pause");
	return EXIT_SUCCESS;
}