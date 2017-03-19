#include <iostream>
using namespace std;

template <typename T>
class MyVector
{
	friend ostream & operator<< <T>(ostream &out, const MyVector &obj);//friend operator<<(ostream &out, const MyVector &obj);
public:
	MyVector(int size = 0);
	MyVector(const MyVector &obj);
	~MyVector();
public:
	T & operator[](int index);//返回引用既可以当左值又可以。。。
	MyVector & operator=(const MyVector &obj);//const只读

public:
	int getLen()
	{
		return m_len;
	}
private:
protected:
	T *m_space;
	int m_len;


};//此时的vector 相当于一个容器