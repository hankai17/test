#include "MyArray.h"


Array::Array(int length)
{
	if (length < 0)
	{
		m_length = -1;
		m_space = new int[m_length];
	}
	m_length = length;
	m_space = new int[m_length];
}

Array::~Array()
{
	if (m_space != NULL)
	{
		delete[] m_space;
		m_length = 0;
	}
}
Array::Array(const Array & obj)
{
	this->m_length = obj.m_length;
	this->m_space = new int[this->m_length];//分配给内存空间 深拷贝

	for (int i = 0; i < obj.m_length; i++)
	{
		this->m_space[i] = obj.m_space[i];
	}
}

int Array::length()
{
	return m_length;
}
void Array::setData(int index, int valude)
{
	m_space[index] = valude;
}
int Array::getData(int index)
{
	return m_space[index];
}