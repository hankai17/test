#define D_SCL_SECURE_NO_WARNINGS
#include <iostream>
using namespace std;
#include "string"//不用考虑内存
#include "algorithm"

void main21()//字符串初始化
{
	string s1 = "aaaa";
	string s2("2222");
	string s3 = s2;//调用拷贝构造函数 初始化
	string s4(10, 'a');

	cout << "s1 " << s1 << endl;
	cout << "s2 " << s2 << endl;
	cout << "s3 " << s3 << endl;
	cout << "s4 " << s4<< endl;
}

void main22()//字符串遍历
{
	string s1 = "abcdef";

	//数组方式
	for (int i = 0; i < s1.length(); i++)
	{
		cout << s1[i] << " ";
	}
	//迭代器法

	for (string::iterator it = s1.begin(); it != s1.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;

	//
	for (int i = 0; i < s1.length(); i++)
	{
		cout << s1.at(i) << " ";//抛出异常 没有看懂！ 哦 有异常可以抛出 而数组不向外部抛出异常 会中断
	}

}

//字符指针和string 的转换
void main23()
{
	string s1 = "aaabbb";
	//s1===>char *
	printf("s1:%s\n", s1.c_str());
	//char *===>string
	//s1的内容拷贝到buf中
	char buf1[128];
	//s1.copy(buf1,3,0);//只是单纯的拷贝3个字符 没有/0
	cout << "buf1:" << buf1 << endl;
}


//字符串连接
void main24()
{
	string s1 = "aaa";
	string s2 = "bbb";
	s1 = s1 + s2;//法1
	s1.append(s2);//法2
}
//字符串查找和替换
void main25()
{
	string s1 = "wbm hello wbm 111 wbm 222 wbm 333";
	//第一次出现wbm的下表
	int index=s1.find("wbm", 0);//位置下表从0开始
	cout << "下表：" << index;
	//求出现次数 出现下标
	int offindex = s1.find("wbm", 0);
	while (offindex != string::npos)
	{
		cout << offindex << endl;
		offindex = offindex + 1;
		offindex=s1.find("wbm", offindex);
	}
	//替换
	int offindex1 = s1.find("wbm", 0);
	while (offindex1 != string::npos)
	{
		cout << offindex1 << endl;
		s1.replace(offindex1, 3, "WBM");
		offindex1 = offindex1 + 1;
		offindex1 = s1.find("wbm", offindex1);
	}
	cout << s1;
}

void main26()//截断和插入
{
	string s1 = "hello1 hello2 hello3";
	string::iterator it=find(s1.begin(), s1.end(), 'l');
	if (it != s1.end())
	{
		s1.erase(it);
	}
	cout << s1 << endl;

	//s1.erase(s1.begin(), s1.end());
	//cout << "s1长度" << s1.length();
	//cout << s1 << endl;

	string s2 = "BBB";
	s2.insert(0, "AAA");//头插法
	s2.insert(s2.length(), "AAA");
	cout << s2 << endl;
}

void main27()
{
	string s1 = "AAAbbb";
	transform(s1.begin(), s1.end(), s1.begin(),toupper);//
	cout << s1 << endl;

	string s2 = "AAAbbb";
	transform(s2.begin(), s2.end(), s2.begin(), tolower);//
}

void main()
{
	main26();
	system("pause");
	return;
}







//错误	3	error C4996: 'std::basic_string<char,std::char_traits<char>,std::allocator<char>>::copy': Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'	h:\c++\1.stl\1.stl\2stl的string.cpp	52	1	1.stl
