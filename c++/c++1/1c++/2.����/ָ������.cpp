#include <iostream>
using namespace std;

struct Teacher
{
	char name[64];
	int age;
};

int getTeacher(Teacher **p)//在被调用函数里面 获取资源（内存块）
{
	Teacher *tmp = NULL;
	if (p == NULL)
	{
		return -1;
	}
	tmp = (Teacher *)malloc(sizeof(Teacher));
	if (tmp == NULL)
	{
		return -2;
	}
	tmp->age = 33;
	*p = tmp;//p是实参的地址 *实参的地址 去间接的修改实参的值
}

int getTeacher2(Teacher * &myp)//指针的引用
{
	myp = (Teacher *)malloc(sizeof(Teacher));//给myp赋值相当于给main中的pT1赋值
	//通过指针的引用达到二级指针的效果
	if (myp == NULL)
	{
		return -1;
	}
	myp->age = 38;
}

void FreeTeacher(Teacher *pT1)
{
	if (pT1 == NULL)
	{
		return;
	}
	free(pT1);
}
void main()
{
	//c中的二级指针
	Teacher *pT1 = NULL;
	getTeacher(&pT1);//pT1是*类型，&pT1是**类型
	cout << "age: " << pT1->age << endl;
	FreeTeacher(pT1);

	//c++中的二级指针
	getTeacher2(pT1);//my就是pT1
	cout << "age: " << pT1->age << endl;
	FreeTeacher(pT1);


	system("pause");
}