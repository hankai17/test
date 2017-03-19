//构造函数没有返回类型
#include <iostream>
#include <string.h>
using namespace std;

class Test
{
public:
	Test()//可有参数 可无参数 无返回值
	{
		a = 10; //完成对属性的初始化工作
		p = (char *)malloc(100);
		strcpy(p, "aaaasd");
		cout << "构造" << endl;
	}
	void printP()
	{
		cout << p << endl;
		cout << a << endl;
	}
	~Test(){
		if (p!=NULL)
		{
			free(p);
		}
		cout << "析构" << endl;
	}
private:
	int a;
	char *p;
};

void objplay()//给对象搭建一个舞台 研究对象的行为
{              //若两个对象 构造 构造 析构 析构 （先创建的对象后释放）
	Test t1,t2;//调用此句时 调用构造函数 

	t1.printP();
	
	cout << "分割" << endl;

	t2.printP();//此句完调用析构函数
}
void main()
{
	objplay();

	cout << "" << endl;
	system("pause");
	return;
}