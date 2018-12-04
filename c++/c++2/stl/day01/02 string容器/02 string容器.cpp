#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<string>
using namespace std;


//1. string初始化
/*
string();//创建一个空的字符串 例如: string str;
string(const string& str);//使用一个string对象初始化另一个string对象
string(const char* s);//使用字符串s初始化
string(int n, char c);//使用n个字符c初始化
*/
void test01(){

	
	string s1; //创建空字符串，调用无参构造函数
	string s2(s1); //调用拷贝构造函数
	//string s3 = s1; //调用拷贝构造函数
	const char* str = "hello world!";
	string s4(str); //把c_string转换成了string
	string s5(10, 'a');

	cout << s4 << endl;
	cout << s5 << endl;

}

//2. 赋值操作
/*
string& operator=(const char* s);//char*类型字符串 赋值给当前的字符串
string& operator=(const string &s);//把字符串s赋给当前的字符串
string& operator=(char c);//字符赋值给当前的字符串
string& assign(const char *s);//把字符串s赋给当前的字符串
string& assign(const char *s, int n);//把字符串s的前n个字符赋给当前的字符串
string& assign(const string &s);//把字符串s赋给当前字符串
string& assign(int n, char c);//用n个字符c赋给当前字符串
string& assign(const string &s, int start, int n);//将s从start开始n个字符赋值给字符串
*/
void test02(){

	string s = "abcd";
	s = "hello world!"; //赋值
	s = 'a';

	string s2;
	s2.assign(s);

	cout << s << endl;
	cout << s2 << endl;
}

//3. 字符存取
void test03(){
	
	string s = "hello world!";

	//[]和at 返回的是元素的引用

	for (int i = 0; i < s.size();i++){
		cout << s[i] << " ";
	}
	cout << endl;

	for (int i = 0; i < s.size(); i ++){
		cout << s.at(i) << " ";
		//s.at(i) = 100;
	}
	cout << endl;

	//[]和at区别。[]访问越界，直接挂掉。at访问越界会抛出一个out_of_range异常
	try{
		//cout << s[100] << endl;
		cout << s.at(100) << endl;
	}
	catch (...){
		cout << "访问越界!" << endl;
	}

}

//4.字符串拼接
/*
string& operator+=(const string& str);//重载+=操作符
string& operator+=(const char* str);//重载+=操作符
string& operator+=(const char c);//重载+=操作符
string& append(const char *s);//把字符串s连接到当前字符串结尾
string& append(const char *s, int n);//把字符串s的前n个字符连接到当前字符串结尾
string& append(const string &s);//同operator+=()
string& append(const string &s, int pos, int n);//把字符串s中从pos开始的n个字符连接到当前字符串结尾
string& append(int n, char c);//在当前字符串结尾添加n个字符c
*/
void test04(){

	string s1 = "hello";
	string s2 = " world";

	s1 += s2;

	cout << s1 << endl;
	cout << s2 << endl;

	string s3 = s1 + s2;
	cout << s3 << endl;

	//string + c_string
	string s4 = s1 + " aaaaa";
	cout << s4 << endl;


	s4.append(" bbbbbb");
	cout << s4 << endl;
	s4.append(s1);
	cout << s4 << endl;

}

//5. 字符串查找
/*
int find(const string& str, int pos = 0) const; //查找str第一次出现位置,从pos开始查找
int find(const char* s, int pos = 0) const;  //查找s第一次出现位置,从pos开始查找
int find(const char* s, int pos, int n) const;  //从pos位置查找s的前n个字符第一次位置
int find(const char c, int pos = 0) const;  //查找字符c第一次出现位置
int rfind(const string& str, int pos = npos) const;//查找str最后一次位置,从pos开始查找
int rfind(const char* s, int pos = npos) const;//查找s最后一次出现位置,从pos开始查找
int rfind(const char* s, int pos, int n) const;//从pos查找s的前n个字符最后一次位置
int rfind(const char c, int pos = 0) const; //查找字符c最后一次出现位置
string& replace(int pos, int n, const string& str); //替换从pos开始n个字符为字符串str
string& replace(int pos, int n, const char* s); //替换从pos开始的n个字符为字符串s
*/
void test05(){

	//如果没有查找到返回-1
	string s = "abcdefgth";
	int pos = s.find("dep");
	if (pos == -1){
		cout << "没有查找到!" << endl;
	}
	else{
		cout << "查找到:" << pos << endl;
	}

	s.replace(1, 3, "111");
	cout << s << endl;
}

//6. 字符串比较
/*

compare函数在>时返回 1，<时返回 -1，==时返回 0。
比较区分大小写，比较时参考字典顺序，排越前面的越小。
大写的A比小写的a小。

int compare(const string &s) const;//与字符串s比较
int compare(const char *s) const;//与字符串s比较
*/
void test06(){

	string s1 = "hello";
	string s2 = "hello";
	const char* str = "hello";
	if (s1.compare(s2) == 0){
		cout << "相等!" << endl;
	}

	if (s1.compare(str) == 0){
		cout << "相等!" << endl;
	}

}

//7. 子串
/*
	string substr(int pos = 0, int n = npos) const;//返回由pos开始的n个字符组成的字符串
*/

void test07(){
	
	string email = "hello@hotmal.com";
	int pos = email.find("@");
 	string username =  email.substr(0, pos);
	cout << "username:" << username << endl;
}

//8.插入
/*
string& insert(int pos, const char* s); //插入字符串
string& insert(int pos, const string& str); //插入字符串
string& insert(int pos, int n, char c);//在指定位置插入n个字符c
string& erase(int pos, int n = npos);//删除从Pos开始的n个字符 
*/

void test08(){

	string s = "hello";
	s.insert(1,"111");
	cout << s << endl;

	s.erase(1,3);  //从1号位置开始3个字符
	cout << s << endl;
}

//9. c_string和string转换
void test09(){
	
	//c_string -> string
	const char* str = "hello world";
	string s(str); //调用string的带参数的构造函数

	const char* str2 = s.c_str(); //string->c_string

}

//10.
void test10(){

	string s1 = "hello world!";
	cout << (int*)s1.c_str() << endl;

	char& a = s1[1];
	char& b = s1[3];

	a = '0';
	b = '1';

	cout << s1 << endl;

	s1 = "pppppppppppppppppppp";


	a = '0';
	b = '1';

	cout << (int*)s1.c_str() << endl;
}
int main(){

	//test01();
	//test02();
	//test03();
	//test04();
	//test05();
	//test06();
	//test07();
	//test08();
	test10();

	system("pause");
	return EXIT_SUCCESS;
}