#include <iostream>
using namespace std;
//new 基础类型变量 分配数组变量 分配类对象
//malloc free 是c的函数 new delete是c++操作符
class
{
public:
private:

};

void main()
{
	int * p = (int *)malloc(sizeof(int));//c中
	*p = 10;
	free(p);

	int *p2 = new int;//用法比malloc简单
	*p2 = 20;
	free(p);

	int * p3 = new int(30);
	printf("*p3:%d\n", *p3);
	delete p3;

	system("pause");
	return;
}
void main2()
{
	int * p = (int *)malloc(sizeof(int)*10);//int array[10] c分配数组
	p[0] = 1;
	free(p);

	int *pArray = new int[10];//c++分配数组
	pArray[1] = 2;
	delete [] pArray;


	system("pause");
	return;
}
class Test
{
public:
	Test(int _a)
	{
		a = _a;
		cout << "构造" << endl;
	}
	~Test()
	{
		cout << "析构" << endl;;
	}
private:
	int a;
};

void main4()//new自动执行类的构造函数 delete能执行类的析构函数
{
	Test *pT1 = (Test *)malloc(sizeof(Test));//c中分配对象
	free(pT1);

    Test *pT2=new Test(10);
	delete pT2;

	char * pArray2 = new char[25];
	delete[] pArray2;


	system("pause");
	return;
}