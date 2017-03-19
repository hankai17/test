#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
using namespace std;

class Daguniang{
public:
	void test(){
		mName;
		mMoney;
		mAge;
	}
public:
	string mName; //大姑娘名字好听，希望所有人知道
protected:
	int mMoney; //大姑娘知道干儿子孝顺，自己有多少钱只能让儿子知道
private:
	int mAge; //年龄不要外人知道
};

void test01(){

	//对于public,private，protected在类的内部都可访问，没有区别
	//对于类的外部，public可访问，protected 和private不可访问
	Daguniang bigGirl;
	bigGirl.mName;
	//bigGirl.mAge;
	//bigGirl.mMoney;
}

class Daguniangdeerzi : public Daguniang{
public:
	void Erzi(){
		mName;
		//mAge;
		mMoney;
	}
};

//struct和class区别
struct A{
	int age;
};
class B{
	int age;
};

void test02(){

	A a;
	a.age;

	B b;
	b.age;
}

int main(){



	system("pause");
	return EXIT_SUCCESS;
}