#include <iostream>
using namespace std;

class Animal
{
public:
	virtual void cry() = 0;
};

class Dog :public Animal
{
public:
	virtual void cry()
	{
		cout << "wang!!!" << endl;
	}
	void doHome()
	{
		cout << "看家！" << endl;
	}

};

class Cat :public Animal
{
public:

	virtual void cry()
	{
		cout << "miaomiao" << endl;
	}
	void doThing()
	{
		cout << "捉老鼠" << endl;
	}
};

void playObj(Animal * base)
{
	base->cry();//继承 虚函数重写 父类指针指向子类对象=发生多态
	//要求能识别子类对象
	//dynamic_cast//运行时类的识别
	Dog *pDog = dynamic_cast<Dog*>(base);//让狗做特有工种
		if (pDog != NULL)
		{
			pDog->doHome();
		}

		Cat *pCat = dynamic_cast<Cat *>(base);//把老子转成小子
		if (pCat != NULL)
		{
			pCat->doThing();
		}

}

class Tree {};



void main2()
{
	Dog d1;
	Cat c1;
	Animal *pBase = NULL;
	pBase = &d1;
	
	pBase = static_cast<Animal *>(&d1);
	pBase = reinterpret_cast<Animal*>(&d1);

	
	{
		Tree t1;

		//pBase = static_cast<Animal *>(&t1);编译器会做类型检查  转不了
		pBase = reinterpret_cast<Animal*>(&t1);//可以转
	}

	playObj(&d1);
	playObj(&c1);

	system("pause");
	return;
}


void main1()
{
	double Pi = 3.1415926;
	int num1 =(int) Pi;//c中的转换

	int num2 = static_cast<int>(Pi);//c++中是静态类型转换
	//c中 隐式类型转换的地方都可以使用 static_cast<>转换
	//char *---->int *
	char * p1 = "hello...itcast";
	int *p2 = NULL;
	//p2 = static_cast<int *>(p1);  转换不了只能用 rein
	p2 = reinterpret_cast<int *>(p1);
	cout << p1 << endl;//%s 因为是char 返回字符
	cout << p2 << endl;//%d 因为返回的是int  所以输出地址


	system("pause");
	return;
}
//static cast是静态类型转换 int 转换char
//rein cast 重新类型解释
//dynamic cast 父子类转换
//const cast 去const只读属性

void printB(const char * p)//p指向的内容不能改
{
	cout << p << endl;
	char * p1 = NULL;
	//p1[0] = 'Z';
	p1 = const_cast<char *>(p);//const char *------> char *
	p1[0] = 'Z';
	cout << p << endl;
}

void main()
{
	char buf[] = "aaaaaaaaaaaaa";
	char * myp = "aaaaaaaaaaaaaa";
	printB(buf);
	//printB(myp);  因为没有分配内存空间 所以用之前要确定p所指向的内存空间能被修改
	system("pause");
	return;
}