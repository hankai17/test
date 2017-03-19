#define _CRT_SECURE_NO_WARNINGS
#ifndef MYSTRING_H
#define MYSTRING_H

#include<iostream>

//封装一些c字符串操作函数
class MyString{
public:
	MyString();  //空字符串c MyString str;
	MyString(const char* str); //用c风格字符串初始化字符串  MyString str = "abcdefg";
	MyString(const MyString& str);

	//字符串操作
	const char* c_str(); //将MyString字符串转换成c-style字符串
	int size();  //获得字符串的长度

	//赋值操作
	MyString& operator=(const MyString& str);
	MyString& operator=(const char* p);

	//重载+
	MyString operator+(MyString& str);
	MyString operator+(const char* str);

	// ==, != 重载
	bool operator==(const char* p);
	bool operator!=(const char* p);
	bool operator==(MyString& obj);
	bool operator!=(MyString& obj);

	//重载左移操作符
	friend std::ostream& operator<<(std::ostream& out, MyString& str);

	~MyString();
private:
	char* pAddress; //指向一个字符串
	int mLength;
};

#endif