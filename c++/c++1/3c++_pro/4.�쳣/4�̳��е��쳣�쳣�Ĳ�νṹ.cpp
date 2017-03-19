#include <iostream>
using namespace std;

class MyArray
{
public:
	MyArray(int len);
	int  getLen();
	~MyArray();
public:
	class eSize
	{
	public:
		eSize(int size)
		{
			m_size = size;
		}
		virtual void printErr()
		{
			cout << "size:" << m_size << " ";
		}
		
	protected:
		int m_size;//希望被继承的都是protected
	};//写在最前面
	class eNegative:public eSize
	{
	public:
		eNegative(int size) :eSize(size)
		{
			;
		}
		virtual void printErr()
		{
			cout << "eNegative类 size:" << m_size << " ";
		}
	};
	class eTooBig :public eSize
	{
	public:
		eTooBig(int size) :eSize(size)
		{
			;
		}
		virtual void printErr()
		{
			cout << "eTooBig类 size:" << m_size << " ";
		}
	};
	class eZero :public eSize
	{
	public:
		eZero(int size) :eSize(size)
		{
			;
		}
		virtual void printErr()
		{
			cout << "eZero类 size:" << m_size << " ";
		}
	};
	class eTooSmall :public eSize
	{
	public:
		eTooSmall(int size) :eSize(size)
		{
			;
		}
		virtual void printErr()
		{
			cout << "eTooSmall类 size:" << m_size << " ";
		}
	};
	

	int &operator[](int index);
protected:
private:
	int * m_space;
	int m_len;

};

MyArray::MyArray(int len)
{
	if (len < 0)
	{
		throw  eNegative(len);
	}
	else if (len == 0)
	{
		throw eZero(len);
	}
	else if (len > 1000)
	{
		throw eTooBig(len);
	}
	else if (len < 3)
	{
		throw eTooSmall(len);
	}
	m_space = new int[len];
	m_len = len;
}

int & MyArray::operator[](int index)
{
	return m_space[index];
}
int  MyArray::getLen()
{
	return m_len;
}
MyArray::~MyArray()
{
	if (m_space != NULL)
	{
		delete[] m_space;
		m_space = NULL;
		m_len = 0;
	}
}
/*
void main1()//一个一个的抛出太麻烦 则使用继承
{
	try
	{
		MyArray a(-5);
		for (int i = 0; i < a.getLen(); i++)
		{
			a[i] = i + 1;
			printf("%d ", a[i]);

		}
	}
	catch (MyArray::eNegative e)
	{
		cout << "负数类异常" << endl;

	}
	catch (MyArray::eTooBig e)
	{
		cout << "负数类异常" << endl;

	}
	catch (MyArray::eTooSmall e)
	{
		cout << "负数类异常" << endl;

	}
	catch (MyArray::eZero e)
	{
		cout << "负数类异常" << endl;

	}
	catch (...)
	{

	}
	system("pause");
	return;
}
*/
void main()//一个一个的抛出太麻烦 则使用继承
{
	try
	{
		MyArray a(-5);
		for (int i = 0; i < a.getLen(); i++)
		{
			a[i] = i + 1;
			printf("%d ", a[i]);

		}
	}
	catch (MyArray::eSize &e)
	{
		e.printErr();

	}
	catch (...)
	{

	}
	
	system("pause");
	return;
}











/*
#include <iostream>
using namespace std;

class MyArray
{
public:
	MyArray(int len);
	int  getLen();
	~MyArray();
public:
	int &operator[](int index);
protected:
private:
	int * m_space;
	int m_len;

};

MyArray::MyArray(int len)
{
	m_space = new int[len];
		m_len = len;
}

int & MyArray::operator[](int index)
{
	return m_space[index];
}
int  MyArray::getLen()
{
	return m_len;
}
MyArray::~MyArray()
{
	if (m_space != NULL)
	{
		delete[] m_space;
		m_space = NULL;
		m_len = 0;
	}
}
void main()
{
	MyArray a(5);

	for (int i = 0; i < a.getLen(); i++)
	{
		a[i]=i + 1;
		printf("%d ", a[i]);

	}



	system("pause");
	return;
}
*/