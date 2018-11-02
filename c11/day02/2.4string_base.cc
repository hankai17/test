#include <iostream>
#include <vector>
#include <string.h>

class MyString {
  private:
	char* m_data;
	size_t m_len;
	void copy_data(const char* s) {
	  m_data = new char[m_len + 1];
	  memcpy(m_data, s, m_len);
	  m_data[m_len] = '\0';
	}

  public:
	MyString() {
	  m_data = NULL;
	  m_len = 0;
	}

	MyString(const char* p) {
	  m_len = strlen(p);
	  copy_data(p);
	}

	MyString(const MyString& str) {
	  m_len = str.m_len;
	  copy_data(str.m_data);
	  std::cout << "Copy Constructor is called! source:" << str.m_data << std::endl;
	}

	MyString& operator=(const MyString& str) {
	  if (this != &str) {
		m_len = str.m_len;
		copy_data(str.m_data);
	  }
	  std::cout << "Copy Assignment is called! source:" << str.m_data << std::endl;
	  return *this;
	}

	virtual ~MyString() {
	  if (m_data != NULL) {
		delete[] m_data;
	  }
	}
};

int main(void)
{
  //MyString a = MyString("Hello"); //有参构造函数
  MyString a;
  a = MyString("Hello");  //拷贝分配

  std::vector<MyString> vec;
  vec.push_back(MyString("World")); //push_back的时候得拷贝构造
  return 0;
}
