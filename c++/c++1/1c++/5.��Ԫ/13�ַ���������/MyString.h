#include <iostream>
using namespace std;
class MyString
{
	friend ostream &operator<<(ostream & out, MyString &s);
public:
	MyString();
	MyString(const char * p);
	MyString(const MyString &S);
	~MyString();
public:
	MyString& operator=(const char * p);
	MyString& operator=(const MyString & s);
	char & operator[](int index);
public:
	bool operator==(const MyString & s) const; 
	bool operator==(const char * p) const;
	bool operator!=(const MyString & s) const;
	bool operator!=(const char * p) const;
public:
	int operator<(const char * p);
	int operator>(const char * p);
	int operator>(const MyString & s);
	int operator<(const MyString & s);
public:
	char *c_str()//把指针露出来（内联函数）
	{
		return m_p;
	}
	const char * c_str2()// 只读属性的露（内联函数）
	{
		return m_p;
	}
	int length()
	{
		return m_len;
	}
private:

	int m_len;
	char * m_p;

};
	
	