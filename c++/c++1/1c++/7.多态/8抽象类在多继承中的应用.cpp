//多继承二义性：一个老祖宗virtual就可以解决 若没有老祖宗只能域作用符解决

#include <iostream>
using namespace std;

class B
{
public:
	int b;
protected:
private:

};

class B1:virtual public B
{
public:
	int b1;
protected:
private:

};

class B2 :virtual public B
{
public:
	int b2;
protected:
private:

};

class C :public B1, public B2//多继承
{
public:
	int c;
protected:
private:

};


/*  这种多继承不适用
void main()
{
	C myc;
	myc.c = 10;
	//myc.b=2 会产生二义性 加上virtual就可以构造一次
	myc.b = 2;
	system("pause");
	return;
}
*/