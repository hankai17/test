#include <iostream>
using namespace std;

class ABCD
{
public:
	ABCD(int a, int b, int c)
	{
		this->a = a;
		this->b = b;
		this->c = c;
		printf("ABCD构造函数：a%d ,b%d ,c%d \n", this->a, this->b, this->c);
	}
	~ABCD()
	{
		printf("ABCD析构函数：a%d ,b%d ,c%d \n", this->a, this->b, this->c);
	}
	int getA()
	{
		return this->a;
	}
private:
	int a;
	int b;
	int c;

};
class MyE
{
public:
	MyE() :abcd1(1, 2, 3), abcd2(4, 5, 6), m(100)
	{
		cout << "MyD()" << endl;
	}
	~MyE()
	{
		cout << "~MyD" << endl;
	}
	MyE(const MyE & obj) :abcd1(7, 8, 9), abcd2(10, 11, 12), m(100)
	{
		printf("MyD(const MyD & obj)\n");

	}

public:
	ABCD abcd1;
	ABCD abcd2;
	const int m;
};

int doThing(MyE mye1)
{
	printf("doThing() mye1.abcd1.a:%d\n", mye1.abcd1.getA());
	return 0;
}

int run2()
{
	MyE myE;
	doThing(myE);
	return 0;
}
int run3()
{
	
	printf("run3 start..\n");
	//ABCD(400, 500, 600);//匿名对象的生命周期 此句话立即产生一个构造 一个析构
	ABCD abcd = ABCD(400, 500, 600);//匿名对象转正了 不会立即析构

	printf("run3 end\n");
	return 0;
}
void main()
{
	run3();
	system("pause");
	return;
}