#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <vector>

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
      std::cout<<"default constructor"<<std::endl;
      m_data = NULL;
      m_len = 0;
    }

    MyString(const char* p) {
      std::cout<<"char* constructor"<<std::endl;
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

    MyString(MyString&& str) {
      std::cout << "Move Constructor is called! source:" << str.m_data << std::endl;
      m_len = str.m_len;
      m_data = str.m_data;
      str.m_len = 0;
      str.m_data = NULL;
    }

    MyString& operator=(MyString&& str) {
      std::cout << "Move Assignment is called! source:" << str.m_data << std::endl;

      if (this != &str) {
        m_len = str.m_len;
        m_data = str.m_data;
        str.m_len = 0;
        str.m_data = NULL;
      }

      return *this;
    }

    virtual ~MyString() {
      std::cout<<"~construct"<<std::endl;
      if (m_data != NULL) {
        delete[] m_data;
      }
    }
};

void test01() {
    std::vector<MyString*> v; //vector不能盛放引用 一般都是放指针
    MyString a("hello");
    v.push_back(&a);
}

void test02() {
    std::vector<MyString> v;
    //MyString a("hello"); //调char* construct
    //v.push_back(a); //调copy construct
    v.push_back(MyString("hello")); //先调char* construct 再调move construct //析构两次
}

int test03(void) {
#if 0
  MyString a;
  a = MyString("Hello"); //move assignment //如果没有实现move则 先调char* 再!调赋值构造
#else
  std::vector<MyString> vec;
  std::cout<<"-----------"<<std::endl;
  MyString a; //default
  a = MyString("Hello"); //char* & move assignment & ~construct
  vec.push_back(a); //copy constructor
  std::cout<<"-----------"<<std::endl;
  std::vector<MyString> vec1;
  vec1.push_back(MyString("World")); //char* & move Constructor & ~construct
  std::cout<<"-----------"<<std::endl;
  vec.push_back(MyString("World")); //char* & move Constructor & copy??? & ~construct
  std::cout<<"-----------"<<std::endl;
  
#endif 
  return 0;
}

int main() {
    test03();
    //test02();
}


//move语意 即对临时(右值)对象进行浅拷贝
//push_back时 如果push一个左值则一定会调用拷贝构造 如果push一个右值时则该对象得支持move语义
//这个例子好奇怪啊
