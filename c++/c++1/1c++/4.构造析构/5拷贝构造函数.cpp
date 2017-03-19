#include <iostream>
using namespace std;

class Location
{
public:
	Location(int xx=0,int yy=0)
	{
		X = xx; Y = yy; cout << "构造函数" << endl;
	}
	~Location()
	{
		cout << X << "," << Y << "析构函数" << endl;
	}
	Location(const Location & obj)
	{
		cout << "拷贝构造函数" << endl;
		X = obj.X;
		Y = obj.Y;
	}
	int GetX()
	{
		return X;
	}
	int GetY()
	{
		return Y;
	}
private:
	int X, Y;

};

void f(Location p)//业务函数
{
	cout << p.GetX()<< endl;

}
void playboj()
{
	Location a(1, 2);
	Location b = a;//拷贝构造函数 a初始化b

	f(b);//b实参初始化形参b 也会调用拷贝构造函数

	cout << "b初始化完毕" << endl;
}
void main1()
{
	playboj();

	system("pause");
	return;
}

//这样写代码 编译器设计者大牛们 就返回一个新对象（匿名对象）
Location g()//返回一个匿名对象， 因为局部变量返回不了 所以返回一个新的类型即匿名对象
{
	Location A(1, 2);//构造函数
	return A;//f11  进入拷贝构造函数（即用A这个对象创建（拷贝出）了一个匿名对象） 析构函数 析构原来的A，而非匿名对象
}                //函数的返回值是一个元素（复杂类型的）  
void  objplay2()
{
	g();//不接 会被析构
}

void  objplay3()//用m接 不会被析构只有在main结束才被析构
{

	Location m=g();//用匿名对象初始化m 此时编译器将匿名对象转成m（有名字了）
	cout << "匿名对象被扶正 不会被析构\n" << endl;

}

void  objplay4()
{
	Location m2(1, 2);
	m2 = g();//匿名对象等号给m2 不是初始化 所以析构了
	cout << m2.GetX() << endl;
	cout<<m2.GetY()<<endl;
}

void main()
{
	objplay4();
	system("pause");
	return;

}
//匿名对象的去留:如果用匿名对象初始化另外一个同类型的对象 那么匿名对象直接转成有名对象 
//如果用匿名对象赋值给另一个同类型对象 匿名对象被析构
