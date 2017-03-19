#include <iostream>
using namespace std;

class Test
{
public:
	Test(int a, int b)// c++编译器这样做 Test(Test * this, int a,int b)  this 就是t1的地址 谁调它 它就是谁的地址
	{
		this->a = a;//否则会产生歧义
		this->b = b;
	}
	void printT()
	{
		cout << "a: " << a << endl;
		cout << "b: " << this->b << endl;
	}
	void OpVar(int a, int b) const      //const写在哪儿 没有关系 const修饰的不是形参a 修饰的是
		                                //修饰的是属性this->a 和 this->b
										//编译成 void opvar（const Test *  this,int a,int b）修饰this指针（不是this） 而是this指针指向的内存空间不能改变
	{                                   //进一步编译成 void opvar（const Test * const  this,int a,int b） 即this本身也不能被修改
		cout << "a: " << a << endl;
		cout << "b: " << this->b << endl;
	}
private:
	int a;
	int b;
};

void main()
{
	Test t1(1, 2);
	t1.printT();//编译器 printT(&t1)

	system("pause");
	return;
}