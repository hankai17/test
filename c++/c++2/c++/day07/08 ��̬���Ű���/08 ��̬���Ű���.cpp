#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;


//在开发中，一个类没有这么简单 
//开闭原则 对修改关闭，对扩展开放，通过增加代码来增加代码来增加新的功能，而不是修改源代码
class Caculator{
public:
	void setLeft(int val){
		mLeft = val;
	}
	void setRight(int val){
		mRight = val;
	}
	void setOperator(string oper){
		mOperator = oper;
	}
	int getResult(){
		
		if (mOperator == "+"){
			return mLeft + mRight;
		}
		else if (mOperator == "-"){
			return mLeft - mRight;
		}
		else if (mOperator == "*"){
			return mLeft * mRight;
		}
		else if (mOperator == "/"){
			return mLeft / mRight;
		}
	}
public:
	int mLeft;
	int mRight;
	string mOperator;
};

void DoBussiness1(){

}

//----------------------------------------------
class AbstractCaculator{
public:
	virtual int getResult() = 0; //纯虚函数

	void setLeft(int val){
		mLeft = val;
	}
	void setRight(int val){
		mRight = val;
	}
	void setOperator(string oper){
		mOperator = oper;
	}
public:
	int mLeft;
	int mRight;
	string mOperator;
};

//加法计算器
class Plus : public AbstractCaculator{
public:
	virtual int getResult(){
		return this->mLeft + this->mRight;
	}
};

//减法计算器
class Minus : public AbstractCaculator{
public:
	virtual int getResult(){
		return this->mLeft - this->mRight;
	}
};

//乘法计算器
class Multiplies : public AbstractCaculator{
public:
	virtual int getResult(){
		return this->mLeft * this->mRight;
	}
};

//除法计算器
class Divide : public AbstractCaculator{
public:
	virtual int getResult(){
		return this->mLeft / this->mRight;
	}
};

//取模计算器
class Module : public AbstractCaculator{
public:
	virtual int getResult(){
		return this->mLeft % this->mRight;
	}
};

//上层业务
void DoBussiness(AbstractCaculator* caculator){
	cout << caculator->getResult() << endl;
}

void test01(){
	//父类指针指向子类对象
	AbstractCaculator* caculator = new Minus;
	caculator->setLeft(10);
	caculator->setRight(20);
	DoBussiness(caculator);
}


//多态，多态实现原理

int main(){

	test01();

	AbstractCaculator* a = new Plus;

	system("pause");
	return EXIT_SUCCESS;
}