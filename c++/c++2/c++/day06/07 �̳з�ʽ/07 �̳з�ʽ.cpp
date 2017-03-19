#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

//基类
class A{
public:
	int mA;
protected:
	int mB;
private:
	int mC;
};

//1. 公有(public)继承
class B : public A{
public:
	void PrintB(){
		cout << mA << endl; //可访问基类public属性
		cout << mB << endl; //可访问基类protected属性
		//cout << mC << endl; //不可访问基类private属性
	}
};
class SubB : public B{
	void PrintSubB(){
		cout << mA << endl; //可访问基类public属性
		cout << mB << endl; //可访问基类protected属性
		//cout << mC << endl; //不可访问基类private属性
	}
};


//2. 私有(private)继承
class C : private A{
public:
	void PrintC(){
		cout << mA << endl; //可访问基类public属性
		cout << mB << endl; //可访问基类protected属性
		//cout << mC << endl; //不可访问基类private属性
	}
};
class SubC : public C{
	void PrintSubC(){
		//cout << mA << endl; //不可访问基类public属性
		//cout << mB << endl; //不可访问基类protected属性
		//cout << mC << endl; //不可访问基类private属性
	}
};
void test02(){
	C c;
	//cout << c.mA << endl; //不可访问基类public属性
	//cout << c.mB << endl; //不可访问基类protected属性
	//cout << c.mC << endl; //不可访问基类private属性
}






//3. 保护(protected)继承
class D : protected A{
public:
	void PrintD(){
		cout << mA << endl; //可访问基类public属性
		cout << mB << endl; //可访问基类protected属性
		//cout << mC << endl; //不可访问基类private属性
	}
};
class SubD : public D{
	void PrintD(){
		cout << mA << endl; //可访问基类public属性
		cout << mB << endl; //可访问基类protected属性
		//cout << mC << endl; //不可访问基类private属性
	}
};
void test03(){
	D d;
	//cout << d.mA << endl; //不可访问基类public属性
	//cout << d.mB << endl; //不可访问基类protected属性
	//cout << d.mC << endl; //不可访问基类private属性
}




//基类
class A{
public:
	int mA;
protected:
	int mB;
private:
	int mC;
};

class B : public A{

protected:
	int mA;
protected:
	int mB;
};


int main(){



	system("pause");
	return EXIT_SUCCESS;
}