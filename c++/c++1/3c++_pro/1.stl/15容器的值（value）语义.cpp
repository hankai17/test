#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
#include "vector"
class Teacher
{
public:
	Teacher(char *name, int age)
	{
		m_pname = new char[strlen(name) + 1];
		strcpy(m_pname, name);
		m_age = age;
	}
	~Teacher()
	{
		delete[] m_pname;
		m_pname = NULL;
		m_age = 0;
	}
	void printT()
	{
		cout << m_pname << "\t" << m_age << endl;
	}
	Teacher(const Teacher &obj)
	{
		m_pname = new char[strlen(obj.m_pname + 1)];
		strcpy(m_pname, obj.m_pname);
		m_age = obj.m_age;

	}

	//重载等号操作符
	Teacher& operator=(const Teacher &obj)
	{
		//先把t2的旧内存释放掉
		if (m_pname != NULL)
		{
			delete[]m_pname;
			m_pname = NULL;
			m_age = 0;
		}
		//根据t1大小分配内存
		m_pname = new char[strlen(obj.m_pname + 1)];
		//拷贝t1数据
		strcpy(m_pname, obj.m_pname);
		m_age = obj.m_age;
		return *this;
		
	}
protected:
private:
	char *m_pname;
	int m_age;
};

void main1301()
{
	Teacher t1("t1", 41);
	vector<Teacher> v1;
	v1.push_back(t1);//把t1拷贝了一份 存到容器中了//会出现浅拷贝(两个指针同时指向一块内存空间) 解决方法自己写拷贝构造函数 重载等号操作符
}



void main()
{
	system("pause");
	return;
}

//错误	1	error C4996: 'strcpy': This function or variable may be unsafe. Consider using strcpy_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS. See online help for details.	h:\c++\1.stl\1.stl\15容器的值（value）语义.cpp	11	1	1.stl
