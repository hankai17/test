
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;

class Name
{

public:

	Name(const char * myp)
	{
		m_len = strlen(myp);
		m_p = (char *)malloc(m_len + 1);//!!
		strcpy(m_p, myp);
	}
	~Name()
	{
		if (m_p != NULL)
		{
			cout << "类用完毕 析构" << endl;
			free(m_p);
			m_p = NULL;
			m_len = 0;
		}
	}
	Name(const Name & obj1)//手工编写拷贝构造函数 使用深拷贝
	{
		m_len = obj1.m_len;
		m_p = (char *)malloc(m_len + 1);
		strcpy(m_p, obj1.m_p);
	}
	Name & operator=(Name &obj1 )//obj3 = obj1;

	{
		if (this->m_p != NULL)//先释放旧的内存
		{
			delete[] m_p;
			m_len = 0;
		}
		//根据obj1分配内存大小
		this->m_len = obj1.m_len;
		this->m_p = new char[m_len + 1];
		//把obj1复制
		strcpy(m_p, obj1.m_p);
		return *this;
	}
private:
	char * m_p;
	int m_len;

};




void objplaymain()
{
	Name obj1("abcdefg");//调用有参构造函数
	//Name obj2 = obj1;//调用拷贝构造函数 若没有写 编译器自动调用浅拷贝（栈里的值又拷贝了一份 还是指向原来的堆 堆里的abcdefg没有重新拷贝）
	//释放的时候 先析构obj2（属性 变为NULL 0） 当再析构obj1时 就错了
	//所以引入深拷贝 必须自己写拷贝构造函数 编译器太懒
	Name obj3("obj3");
	obj3 = obj1;
	//obj3 = obj1;//等号操作 对象1的属性拷贝给对象3的属性 浅拷贝（c++编译器提供的等号操作属于浅拷贝）会crimedown
	//operator=()//左操作数obj3通过指针进行传递相当于 obj3.operator=（obj1）
	//operator=（Name & obj1）
	//operator=（Name & obj1）;

}

void main()
{
	objplaymain();

	system("pause");
	return;
}