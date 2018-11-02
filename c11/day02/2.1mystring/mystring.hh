#ifndef MYSTRING_H
#define MYSTRING_H

#include<iostream>
#include<string.h>

//封装一些c字符串操作函数//首先确定有两个属性：char*字符串 和 其长度
class mystring {
  public:
	mystring();  //空字符串c mystring str;
	mystring(const char* str); //用c风格字符串初始化字符串  mystring str = "abcdefg";
	mystring(const mystring& str);

	//字符串操作
	const char* c_str(); //将mystring字符串转换成c-style字符串
	int size();  //获得字符串的长度

	//赋值操作
	mystring& operator=(const mystring& str);
	mystring& operator=(const char* p);

	//重载+
	mystring operator+(mystring& str);
	mystring operator+(const char* str);

	// ==, != 重载
	bool operator==(const char* p);
	bool operator!=(const char* p);
	bool operator==(mystring& obj);
	bool operator!=(mystring& obj);

	//重载左移操作符
	friend std::ostream& operator<<(std::ostream& out, mystring& str);

	~mystring();

  private:
	char* pAddress; //指向一个字符串
	int mLength;
};

#endif
